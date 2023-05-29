/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_SERVER_H
#define CONCLAVE_SERVER_SERVER_H

#include <clog/clog.h>
#include <relay-serialize/types.h>
#include <relay-server-lib/address.h>
#include <relay-server-lib/connections.h>
#include <relay-server-lib/listeners.h>
#include <relay-server-lib/transport.h>
#include <relay-server-lib/user_sessions.h>
#include <relay-server-lib/users.h>
#include <stdarg.h>

struct ImprintAllocator;
struct RelayServerResponse;

typedef struct RelayServer {
    RelayUsers users;
    RelayUserSessions userSessions;
    RelayServerConnections connections;
    RelayListeners listeners;
    Clog log;
    RelaySerializeServerChallenge secretChallengeKey;
} RelayServer;

int relayServerInit(RelayServer* self, struct ImprintAllocator* memory, Clog log);
void relayServerDestroy(RelayServer* self);
void relayServerReset(RelayServer* self);
int relayServerFeed(RelayServer* self, const RelayAddress* address, const uint8_t* data, size_t len,
                    struct RelayServerResponse* response);

#endif
