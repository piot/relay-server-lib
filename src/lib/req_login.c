/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "cipher.h"
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <relay-serialize/server_in.h>
#include <relay-serialize/server_out.h>
#include <relay-server-lib/req_user_login.h>
#include <relay-server-lib/server.h>
#include <relay-server-lib/user.h>
#include <relay-server-lib/user_session.h>

/// Try to login a user and creates a user session on success
/// @param self
/// @param address
/// @param data
/// @param len
/// @param outStream
/// @return
int relayReqUserLogin(RelayServer* self, const RelayAddress* address, const uint8_t* data, size_t len,
                      struct FldOutStream* outStream)
{
    if (len == 0) {
        return -2;
    }

    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    RelaySerializeClientNonce clientNonce;
    RelaySerializeServerChallenge serverChallengeFromClient;
    char username[32];

    relaySerializeServerInLogin(&inStream, &clientNonce, &serverChallengeFromClient, username, 32);

    // Challenge is done to avoid at least the simplest forms of IP spoofing
    uint64_t calculatedClientNonce = extremelyUnsecureCipher(serverChallengeFromClient, self->secretChallengeKey);
    if (calculatedClientNonce != clientNonce) {
        CLOG_C_SOFT_ERROR(&self->log, "challenge didnt work out")
        return -2;
    }

    CLOG_C_DEBUG(&self->log, "challenge was approved from client nonce %016lX", clientNonce);

    RelayUser* foundUser;
    int errorCode = relayUsersReadLogin(&self->users, username, &foundUser);
    if (errorCode < 0) {
        return errorCode;
    }

    RelayUserSession* foundSession;
    int err = relayUserSessionsCreate(&self->userSessions, foundUser, address, &foundSession);
    if (err < 0) {
        return err;
    }

    CLOG_C_DEBUG(&self->log, "logged in user '%s' and created user session %016lX", foundUser->name,
                 foundSession->userSessionId);

    relaySerializeServerOutLogin(outStream, clientNonce, foundSession->userSessionId);

    return 0;
}
