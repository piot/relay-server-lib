/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <relay-serialize/serialize.h>
#include <relay-serialize/server_in.h>
#include <relay-serialize/server_out.h>
#include <relay-server-lib/connection.h>
#include <relay-server-lib/listener.h>
#include <relay-server-lib/req_listen.h>
#include <relay-server-lib/server.h>

/// A user listens for incoming connections (usually hosting a game)
/// A listener is for a specific application and channel.
/// @param self server
/// @param userSession user session that wants to listn
/// @param inStream stream to read listen request from
/// @param outStream response stream
/// @return negative on error
int relayReqListen(RelayServer* self, const struct GuiseSclUserSession* userSession, FldInStream* inStream,
                   FldOutStream* outStream)
{
    RelaySerializeListenRequestFromClientToServer listenRequest;

    int err = relaySerializeServerInRequestListen(inStream, &listenRequest);
    if (err < 0) {
        return err;
    }

    RelayListener* listener = relayListenersFindOrCreate(&self->listeners, userSession, listenRequest.appId,
                                                         listenRequest.channelId);

    RelaySerializeListenResponseFromServerToListener response;
    response.requestId = listenRequest.requestId;
    response.listenerId = listener->id;
    response.channelId = listener->channelId;
    response.appId = listener->applicationId;

    relaySerializeServerOutListenResponseToListener(outStream, response);

    return 0;
}
