/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <guise-sessions-client/client.h>
#include <guise-sessions-client/user_session.h>
#include <inttypes.h>
#include <relay-serialize/serialize.h>
#include <relay-serialize/server_in.h>
#include <relay-server-lib/connection.h>
#include <relay-server-lib/listener.h>
#include <relay-server-lib/listeners.h>
#include <relay-server-lib/req_connect.h>
#include <relay-server-lib/server.h>
#include <relay-server-lib/utils.h>

/// A client wants to connect to another listener user (usually a host)
/// Receiver must answer with accept (or send a packet on that connection) or close directly
/// @param self server
/// @param userSession user session that wants to connect
/// @param inStream stream to read connect command from
/// @param outStream response stream
/// @return negative on error
int relayReqConnect(RelayServer* self, const struct GuiseSclUserSession* userSession, FldInStream* inStream,
                    FldOutStream* outStream, RelayServerResponse* response)
{
    (void) outStream;

    RelaySerializeConnectRequestFromClientToServer request;

    int err = relaySerializeServerInRequestConnect(inStream, &request);
    if (err < 0) {
        return err;
    }
    CLOG_C_VERBOSE(&self->log, "reqConnect: %" PRIX64 " appId: %" PRIX64 " requestId: %02X", request.connectToUserId,
                   request.appId, request.channelId)

    RelayListener* listener = relayListenersFindUsingUserId(&self->listeners, request.appId, request.channelId,
                                                            request.connectToUserId);

    if (listener == 0) {
        // TODO: Send error to initiator
        CLOG_C_SOFT_ERROR(&self->log, "there is no listener for userID: %" PRIX64, request.connectToUserId)
        return -4;
    }

    RelayServerConnection* connection = relayServerConnectionsFindOrCreateConnection(
        &self->connections, userSession, request.connectToUserId, listener, request.appId, request.channelId,
        request.requestId);

    CLOG_EXECUTE(char tempAddrString[32];)
    CLOG_C_DEBUG(&self->log, "sending connect request to listener %" PRIX64 " on IP:%s", listener->id,
                 guiseSclAddressToString(&listener->providingUserSession->address, tempAddrString, 32))

    return relayServerSendConnectRequestToListener(listener, connection, response);
}
