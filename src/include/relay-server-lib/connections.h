/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_CONNECTIONS_H
#define RELAY_SERVER_CONNECTIONS_H

#include "connection.h"
#include <stdlib.h>

struct RelayServerConnection;
struct RelayUserSession;

typedef struct RelayServerConnections {
    struct RelayServerConnection* connections;
    size_t connectionCount;
    size_t capacityCount;
} RelayServerConnections;

void relayServerConnectionsInit(RelayServerConnections* self, size_t maxCount);
void relayServerConnectionsDestroy(RelayServerConnections* self);
struct RelayServerConnection* relayServerConnectionsFindConnection(RelayServerConnections* self,
                                                                   RelayServerConnectionId id);
struct RelayServerConnection* relayServerConnectionsFindOrCreateConnection(
    RelayServerConnections* self, const struct RelayUserSession* initiatorUserSession,
    RelaySerializeUserId targetUserId, RelaySerializeApplicationId applicationId, RelaySerializeChannelId channelId);
#endif
