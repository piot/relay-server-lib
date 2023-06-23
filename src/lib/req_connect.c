/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
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
/// @param self
/// @param userSession
/// @param inStream
/// @param outStream
/// @return
int relayReqConnect(RelayServer* self, const struct GuiseSclUserSession* userSession, FldInStream* inStream,
                    FldOutStream* outStream, RelayServerResponse* response)
{
    (void) outStream;

    RelaySerializeConnectRequestFromClientToServer request;

    int err = relaySerializeServerInRequestConnect(inStream, &request);
    if (err < 0) {
        return err;
    }

    RelayListener* listener = relayListenersFindUsingUserId(&self->listeners, request.appId, request.channelId,
                                                            request.connectToUserId);

    if (listener == 0) {
        // TODO: Send error to initiator
        return -4;
    }

    RelayServerConnection* connection = relayServerConnectionsFindOrCreateConnection(
        &self->connections, userSession, request.connectToUserId, request.appId, request.channelId);

    return relayServerSendConnectRequestToListener(connection, response);
}
