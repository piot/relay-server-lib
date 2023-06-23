/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <imprint/allocator.h>
#include <relay-serialize/serialize.h>
#include <relay-server-lib/address.h>
#include <relay-server-lib/listener.h>
#include <relay-server-lib/listeners.h>
#include <relay-server-lib/unique_id.h>
#include <guise-sessions-client/client.h>
#include <guise-sessions-client/user_session.h>
#include <inttypes.h>

/// Initialize the listener collection
/// @param self
/// @param allocator
/// @param maxCount
void relayListenersInit(RelayListeners* self, ImprintAllocator* allocator, size_t maxCount)
{
    // self->log = log;
    self->capacityCount = maxCount;
    self->listeners = IMPRINT_ALLOC_TYPE_COUNT(allocator, RelayListener, self->capacityCount);
}

void relayListenersReset(RelayListeners* self)
{
    for (size_t i = 0; i < self->capacityCount; ++i) {
        RelayListener* listener = &self->listeners[i];
        listener->providingUserSession = 0;
    }
}

void relayListenersDestroy(RelayListeners* self)
{
    self->capacityCount = 0;
}


/// Creates a new listener
/// @param self
/// @param userSession
/// @param applicationId
/// @param channelId
/// @param outListener
/// @return
int relayListenersCreate(RelayListeners* self, const struct GuiseSclUserSession* userSession,
                         RelaySerializeApplicationId applicationId, RelaySerializeChannelId channelId,
                         RelayListener** outListener)
{
    for (size_t i = 0; i < self->capacityCount; ++i) {
        RelayListener* listener = &self->listeners[i];
        if (listener->providingUserSession == 0) {
            // Clog userSessionLog;
            // userSessionLog.config = listener->log.config;
            // tc_snprintf(session->prefix, 32, "%s/%zu", sessions->log.constantPrefix, i);
            // userSessionLog.constantPrefix = session->prefix;
            RelaySerializeListenerId uniqueSessionId = relayGenerateUniqueIdFromIndex(i);
            relayListenerInit(listener, userSession, uniqueSessionId, applicationId, channelId);
            *outListener = listener;
            return 0;
        }
    }
    *outListener = 0;
    return -1;
}

/// Find an existing listener using a listener ID
/// @param self
/// @param uniqueId
/// @param outSession
/// @return
int relayListenersFind(const RelayListeners* self, RelaySerializeListenerId uniqueId, RelayListener** outSession)
{
    size_t index = relayUniqueIdGetIndex(uniqueId);
    if (index >= self->capacityCount) {
        return -2;
    }

    RelayListener* foundListener = &self->listeners[index];
    if (foundListener->id != uniqueId) {
        CLOG_C_SOFT_ERROR(&self->log, "wrong user session id, got %" PRIx64 " but wanted %" PRIx64, uniqueId, foundListener->id)
    }

    *outSession = foundListener;

    return 0;
}

/// Finds a listener from a userId, applicationId and channelId
/// @param self
/// @param applicationId
/// @param channelId
/// @param userId
/// @return
RelayListener* relayListenersFindUsingUserId(const RelayListeners* self, RelaySerializeApplicationId applicationId,
                                             RelaySerializeChannelId channelId, RelaySerializeUserId userId)
{
    for (size_t i = 0; i < self->capacityCount; ++i) {
        RelayListener* listener = &self->listeners[i];
        if (listener->providingUserSession == 0) {
            continue;
        }

        if (listener->providingUserSession->userId != userId) {
            continue;
        }

        if (listener->channelId == channelId && listener->applicationId == applicationId) {
            return listener;
        }
    }

    return 0;
}

/// Finds an existing listener from userSession, applicationId and channelId
/// @param self
/// @param userSession
/// @param applicationId
/// @param channelId
/// @return
struct RelayListener* relayListenersFindOrCreate(RelayListeners* self, const struct GuiseSclUserSession* userSession,
                                                 RelaySerializeApplicationId applicationId,
                                                 RelaySerializeChannelId channelId)
{
    RelayListener* listener = relayListenersFindUsingUserId(self, applicationId, channelId, userSession->userId);
    if (listener != 0) {
        return listener;
    }

    int err = relayListenersCreate(self, userSession, applicationId, channelId, &listener);
    if (err < 0) {
        return 0;
    }

    return listener;
}
