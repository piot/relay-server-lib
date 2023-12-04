/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/relay-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_LIB_UTILS_H
#define RELAY_SERVER_LIB_UTILS_H

#include <relay-server-lib/transport.h>

struct RelayServerConnection;
struct RelayListener;

typedef struct RelayServerResponse {
    RelayServerSendDatagram sendDatagram;
} RelayServerResponse;

int relayServerSendConnectRequestToListener(const struct RelayListener* listener,
                                            struct RelayServerConnection* foundConnection,
                                            struct RelayServerResponse* response);

#endif
