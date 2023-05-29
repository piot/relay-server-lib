/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_CONNECTION_H
#define RELAY_SERVER_CONNECTION_H

#include "unique_id.h"
#include <relay-serialize/types.h>

struct RelayUserSession;

typedef enum RelayServerConnectionPhase {
    RelayServerConnectionPhaseConnecting,
    RelayServerConnectionPhaseAcceptedByListener,
    RelayServerConnectionPhaseClosedByListener,
} RelayServerConnectionPhase;

typedef RelayUniqueId RelayServerConnectionId;

typedef struct RelayServerConnection {
    RelayServerConnectionId id;
    RelaySerializeApplicationId applicationId;
    RelaySerializeChannelId channelId;
    struct RelayUserSession* initiator;
    struct RelayUserSession* listener;
    RelayServerConnectionPhase phase;
} RelayServerConnection;

#endif
