/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <imprint/allocator.h>
#include <relay-serialize/commands.h>
#include <relay-server-lib/address.h>
#include <relay-server-lib/req_challenge.h>
#include <relay-server-lib/req_connect.h>
#include <relay-server-lib/req_listen.h>
#include <relay-server-lib/req_packet.h>
#include <relay-server-lib/req_user_login.h>
#include <relay-server-lib/server.h>
#include <relay-server-lib/user.h>
#include <relay-server-lib/user_session.h>
#include <relay-server-lib/utils.h>
#include <secure-random/secure_random.h>

/// Handle incoming datagrams
/// @param self
/// @param address
/// @param data
/// @param len
/// @param response
/// @return
int relayServerFeed(RelayServer* self, const RelayAddress* address, const uint8_t* data, size_t len,
                    RelayServerResponse* response)
{
    // CLOG_C_VERBOSE("relayServerFeed: feed: %s octetCount: %zu", relaySerializeCmdToString(data[0]), len)
#define UDP_MAX_SIZE (1200)
    static uint8_t buf[UDP_MAX_SIZE];
    FldOutStream outStream;
    fldOutStreamInit(&outStream, buf, UDP_MAX_SIZE);
    int result = -1;
    switch (data[0]) {
        case relaySerializeCmdChallenge:
            result = relayReqChallenge(self, address, data + 1, len - 1, &outStream);
            break;
        case relaySerializeCmdLogin:
            result = relayReqUserLogin(self, address, data + 1, len - 1, &outStream);
            break;
        default: {
            FldInStream inStream;
            fldInStreamInit(&inStream, data + 1, len - 1);
            const RelayUserSession* foundUserSession;
            int err = relayUserSessionsReadAndFind(&self->userSessions, address, &inStream, &foundUserSession);
            if (err < 0) {
                return err;
            }
            switch (data[0]) {
                case relaySerializeCmdPacket:
                    return relayReqPacket(self, foundUserSession, &inStream, response);
                case relaySerializeCmdConnectRequestToServer:
                    result = relayReqConnect(self, foundUserSession, &inStream, &outStream, response);
                    break;
                case relaySerializeCmdListenRequestToServer:
                    result = relayReqListen(self, foundUserSession, &inStream, &outStream);
                    break;
                default:
                    CLOG_C_SOFT_ERROR(&self->log, "relayServerFeed: unknown command %02X", data[0]);
                    return 0;
            }
            break;
        }
    }

    if (result < 0) {
        return result;
    }

    return response->sendDatagram.send(response->sendDatagram.self, address, outStream.octets, outStream.pos);
}

/// Initialize the relay server
/// @param self
/// @param memory
/// @param log
/// @return
int relayServerInit(RelayServer* self, struct ImprintAllocator* memory, Clog log)
{
    self->log = log;

    self->secretChallengeKey = secureRandomUInt64();

    Clog subLog;
    subLog.config = log.config;

    tc_snprintf(self->userSessions.prefix, 32, "%s/usersessions", log.constantPrefix);
    subLog.constantPrefix = self->userSessions.prefix;
    relayUserSessionsInit(&self->userSessions, subLog);

    tc_snprintf(self->users.prefix, 32, "%s/users", log.constantPrefix);
    subLog.constantPrefix = self->users.prefix;
    relayUsersInit(&self->users, subLog);

    relayServerConnectionsInit(&self->connections, 128);
    relayListenersInit(&self->listeners, memory, 128);

    return 0;
}

void relayServerReset(RelayServer* self)
{
    relayUserSessionsReset(&self->userSessions);
    relayUsersReset(&self->users);
}

void relayServerDestroy(RelayServer* self)
{
    relayUserSessionsDestroy(&self->userSessions);
    relayUsersDestroy(&self->users);
}
