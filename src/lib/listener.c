/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <relay-server-lib/listener.h>

/// Initializes a listener
/// @param self
/// @param providingUserSession
/// @param listenerId
/// @param applicationId
/// @param channelId
void relayListenerInit(RelayListener* self, const struct RelayUserSession* providingUserSession,
                       RelaySerializeListenerId listenerId, RelaySerializeApplicationId applicationId,
                       RelaySerializeChannelId channelId)
{
    self->providingUserSession = providingUserSession;
    self->id = listenerId;
    self->channelId = channelId;
    self->applicationId = applicationId;
}
