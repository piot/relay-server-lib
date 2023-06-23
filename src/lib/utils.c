/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <flood/out_stream.h>
#include <relay-serialize/server_out.h>
#include <relay-server-lib/connection.h>
#include <guise-sessions-client/user_session.h>
#include <relay-server-lib/utils.h>


/// Send a connect request from an initiator to a listener
/// @param foundConnection
/// @param response
/// @return
int relayServerSendConnectRequestToListener(RelayServerConnection* foundConnection, RelayServerResponse* response)
{
    FldOutStream outStream;
    uint8_t buf[512];
    fldOutStreamInit(&outStream, buf, 512);

    RelaySerializeConnectRequestFromServerToListener data;
    data.debugRequestId = 0;
    data.fromUserId = foundConnection->initiator->userId;
    data.channelId = foundConnection->channelId;
    data.appId = foundConnection->applicationId;
    data.assignedConnectionId = foundConnection->id;

    int err = relaySerializeServerOutConnectRequestToListener(&outStream, data);
    if (err < 0) {
        return err;
    }
    return response->sendDatagram.send(response->sendDatagram.self, &foundConnection->listener->address, buf,
                                       outStream.pos);
}
