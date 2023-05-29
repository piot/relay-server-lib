/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_USER_SESSIONS_H
#define RELAY_SERVER_USER_SESSIONS_H

#include <relay-server-lib/address.h>
#include <stdlib.h>

struct RelayUserSession;
struct sockaddr_in;
struct FldInStream;
struct RelayUser;

typedef struct RelayUserSessions {
    struct RelayUserSession* userSessions;
    size_t userSessionCapacity;
    size_t userSessionCount;
    Clog log;
    char prefix[32];
} RelayUserSessions;

void relayUserSessionsInit(RelayUserSessions* self, Clog log);
void relayUserSessionsDestroy(RelayUserSessions* self);
void relayUserSessionsReset(RelayUserSessions* self);
int relayUserSessionsCreate(RelayUserSessions* sessions, struct RelayUser* user, const RelayAddress* address,
                          struct RelayUserSession** outSession);
int relayUserSessionsReadAndFind(const RelayUserSessions* self, const RelayAddress* address, struct FldInStream* stream,
                               const struct RelayUserSession** outSession);

#endif
