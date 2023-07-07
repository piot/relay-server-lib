/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <guise-sessions-client/client.h>
#include <guise-sessions-client/user_session.h>
#include <relay-server-lib/connections.h>
#include <inttypes.h>

void relayServerConnectionsInit(RelayServerConnections* self, size_t maxCount)
{
    // self->log = log;
    self->capacityCount = maxCount;
    self->connections = tc_malloc_type_count(RelayServerConnection, self->capacityCount);
    tc_mem_clear_type_n(self->connections, self->capacityCount);
}

void relayServerConnectionsDestroy(RelayServerConnections* self)
{
    (void) self;
}

/// Looks up an existing connection from a connection ID
/// @note needs optimizing
/// @param self relay server connections
/// @param id connection id to find
/// @return the found connection or NULL
struct RelayServerConnection* relayServerConnectionsFindConnection(RelayServerConnections* self,
                                                                   RelayServerConnectionId id)
{
    size_t index = relayUniqueIdGetIndex(id);
    if (index >= self->capacityCount) {
        CLOG_SOFT_ERROR("illegal id %" PRIX64, id)
        return 0;
    }

    RelayServerConnection* connection = &self->connections[index];

    if (connection->id != id) {
        return 0;
    }

    return connection;
}

/// Checks if a connection already exists between initiator and listener, if not it creates a new one
/// @param self server connections
/// @param initiatorUserSession user session that wants to create a connection
/// @param targetUserId the user id that it wants to connect to
/// @param applicationId the application id
/// @param channelId the channel
/// @return NULL if it couldn't be find or created
struct RelayServerConnection* relayServerConnectionsFindOrCreateConnection(
    RelayServerConnections* self, const struct GuiseSclUserSession* initiatorUserSession,
    RelaySerializeUserId targetUserId, struct RelayListener* listener, RelaySerializeApplicationId applicationId,
    RelaySerializeChannelId channelId)
{
    (void) listener;
    for (size_t i = 0; i < self->capacityCount; ++i) {
        RelayServerConnection* connection = &self->connections[i];
        if (connection->initiatorUserSession == 0) {
            continue;
        }

        if (connection->initiatorUserSession != initiatorUserSession) {
            continue;
        }

        if (connection->channelId == channelId && connection->applicationId == applicationId &&
            connection->listenerUserSession->userId == targetUserId) {
            return connection;
        }
    }

    return 0;
}
