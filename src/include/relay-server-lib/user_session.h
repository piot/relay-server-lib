/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_USER_SESSION_H
#define RELAY_SERVER_USER_SESSION_H

#include <relay-server-lib/address.h>
#include <clog/clog.h>
#include <relay-serialize/types.h>

struct RelayUser;

typedef struct RelayUserSession {
    RelaySerializeUserSessionId userSessionId;
    RelayAddress address;
    const struct RelayUser* user;
    Clog log;
    char prefix[32];
} RelayUserSession;

void relayUserSessionInit(RelayUserSession* self, RelaySerializeUserSessionId userSessionId, const RelayAddress* address, const struct RelayUser* owner, Clog log);

#endif
