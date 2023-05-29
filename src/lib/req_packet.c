/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <relay-serialize/commands.h>
#include <relay-serialize/serialize.h>
#include <relay-serialize/server_out.h>
#include <relay-server-lib/connection.h>
#include <relay-server-lib/req_packet.h>
#include <relay-server-lib/req_user_login.h>
#include <relay-server-lib/server.h>
#include <relay-server-lib/user.h>
#include <relay-server-lib/user_session.h>
#include <relay-server-lib/utils.h>

/// A packet is requested to be sent to the remote for that connection
/// @param self
/// @param userSession
/// @param inStream
/// @param response
/// @return
int relayReqPacket(RelayServer* self, const RelayUserSession* userSession, FldInStream* inStream,
                   RelayServerResponse* response)
{
    RelaySerializeConnectionId connectionId;
    relaySerializeReadConnectionId(inStream, &connectionId);

    RelayServerConnection* foundConnection = relayServerConnectionsFindConnection(&self->connections, connectionId);
    if (foundConnection == 0) {
        return -4;
    }

    const RelayUserSession* target = 0;
    if (foundConnection->initiator == userSession) {
        target = foundConnection->listener;
        if (foundConnection->phase == RelayServerConnectionPhaseConnecting) {
            // listener has not accepted the connection, so send request
            return relayServerSendConnectRequestToListener(foundConnection, response);
        }
    } else if (foundConnection->listener == userSession) {
        // If listener is sending to an initiator and knows the connection handle, the listener has
        // implicitly accepted it
        target = foundConnection->initiator;
        foundConnection->phase = RelayServerConnectionPhaseAcceptedByListener;
    } else {
        CLOG_C_NOTICE(&self->log, "not allowed to send packets on this connection")
    }

    uint16_t packetOctetCount;
    fldInStreamReadUInt16(inStream, &packetOctetCount);

    uint8_t buf[1200];

    FldOutStream outStream;
    fldOutStreamInit(&outStream, buf, 1200);

    relaySerializeWriteCommand(&outStream, relaySerializeCmdPacketToClient, "prefix");
    RelaySerializeServerPacketFromServerToClient packetHeader;
    packetHeader.packetOctetCount = packetOctetCount;
    packetHeader.connectionId = foundConnection->id;
    relaySerializeServerOutPacketToClientHeader(&outStream, packetHeader);

    fldOutStreamWriteOctets(&outStream, inStream->p, packetOctetCount);
    inStream->pos += packetOctetCount;
    inStream->p += packetOctetCount;

    RelayServerSendDatagram* sendDatagram = &response->sendDatagram;
    sendDatagram->send(sendDatagram->self, &target->address, buf, outStream.pos);

    return 0;
}
