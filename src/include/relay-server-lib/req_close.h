/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/relay-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_LIB_REQ_CLOSE_H
#define RELAY_SERVER_LIB_REQ_CLOSE_H

struct RelayServer;
struct RelayUserSession;
struct FldInStream;
struct FldOutStream;

int relayReqClose(struct RelayServer* self, const struct RelayUserSession* userSession, struct FldInStream* inStream,
                  struct FldOutStream* outStream);

#endif
