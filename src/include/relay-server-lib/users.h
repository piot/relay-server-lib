/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_USERS_H
#define CONCLAVE_SERVER_USERS_H

#include <stdlib.h>

#include <relay-server-lib/address.h>

struct FldInStream;
struct RelayUser;

typedef struct RelayUsers {
    struct RelayUser* users;
    size_t capacity;
    size_t count;
    char prefix[32];
    Clog log;
} RelayUsers;

void relayUsersInit(RelayUsers* self, Clog log);
void relayUsersReset(RelayUsers* self);
void relayUsersDestroy(RelayUsers* self);
int relayUsersCreate(RelayUsers* sessions, struct RelayUser* user, const RelayAddress* address, struct RelayUser** outUser);
int relayUsersReadAndFind(const RelayUsers* self, const RelayAddress* address, struct FldInStream* stream,
                        struct RelayUser** outUser);
int relayUsersReadLogin(const RelayUsers* self, const char* name, struct RelayUser** outUser);

#endif
