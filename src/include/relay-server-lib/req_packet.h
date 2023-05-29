/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_REQ_PACKET_H
#define RELAY_SERVER_REQ_PACKET_H

#include <stddef.h>
#include <stdint.h>

struct RelayServer;
struct FldOutStream;
struct FldInStream;
struct RelayUserSession;
struct RelayServerResponse;

int relayReqPacket(struct RelayServer* self, const struct RelayUserSession* userSession, struct FldInStream* inStream,
                 struct RelayServerResponse* response);

#endif
