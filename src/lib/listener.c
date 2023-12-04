/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/relay-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <relay-server-lib/listener.h>

/// Initializes a listener
/// @param self listener
/// @param providingUserSession the user session that is listening
/// @param listenerId the listener id
/// @param applicationId application id
/// @param channelId channel id
void relayListenerInit(RelayListener* self, const struct GuiseSclUserSession* providingUserSession,
                       RelaySerializeListenerId listenerId, RelaySerializeApplicationId applicationId,
                       RelaySerializeChannelId channelId)
{
    self->providingUserSession = providingUserSession;
    self->id = listenerId;
    self->channelId = channelId;
    self->applicationId = applicationId;
}
