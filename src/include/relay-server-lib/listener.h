/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_LISTENER_H
#define RELAY_LISTENER_H

struct RelayUserSession;

#include <relay-serialize/types.h>

typedef struct RelayListener {
    const struct RelayUserSession* providingUserSession;
    RelaySerializeListenerId id;
    RelaySerializeApplicationId applicationId;
    RelaySerializeChannelId channelId;
} RelayListener;

void relayListenerInit(RelayListener* self, const struct RelayUserSession* providingUserSession,
                       RelaySerializeListenerId listenerId, RelaySerializeApplicationId applicationId,
                       RelaySerializeChannelId channelId);

#endif
