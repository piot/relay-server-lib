/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_REQ_LISTEN_H
#define RELAY_SERVER_REQ_LISTEN_H

#include <relay-server-lib/address.h>

struct RelayServer;
struct FldOutStream;
struct FldInStream;
struct RelayUserSession;

int relayReqListen(struct RelayServer* self, const struct GuiseSclUserSession* userSession, struct FldInStream* inStream,
                   struct FldOutStream* outStream);

#endif
