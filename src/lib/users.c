/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <relay-serialize/serialize.h>
#include <relay-server-lib/address.h>
#include <relay-server-lib/user.h>
#include <relay-server-lib/users.h>
#include <flood/in_stream.h>

/*
int usersCreate(Users* sessions, User* user, const RelayAddress* address, User** outSession)
{
    for (size_t i = 0; i < sessions->userSessionCapacity; ++i) {
        User* user = &sessions->users[i];
        if (user->name == 0) {
            user->name = tc_str_dup()
            user->user = user;
            *outSession = user;
            return i;
        }
    }
    *outSession = 0;
    return -1;
}

static int usersFind(const Users* self, uint32_t id, const RelayAddress* addr, User** outSession)
{
    if (id >= self->capacity) {
        return -2;
    }

    User* foundSession = &self->users[id];
    if (!nimbleAddressEqual(addr, &foundSession->address)) {
        char addrTemp[64];
        CLOG_C_SOFT_ERROR("wrong address %s vs %s", nimbleAddressToString(addr, addrTemp, 64),
            nimbleAddressToString(&foundSession->address, addrTemp, 64));
        *outSession = 0;
        return -3;
    }

    *outSession = foundSession;

    return 0;
}

 int usersReadAndFind(const Users* self, const RelayAddress* address, FldInStream* stream,
    User** outSession)
{
    uint32_t userId;
    fldInStreamReadUInt32(stream, &userId);

    int errorCode = usersFind(self, userId, address, outSession);
    if (errorCode < 0) {
        CLOG_C_WARN("couldn't find session %d", sessionId);
        return errorCode;
    }

    CLOG_C_DEBUG("found user session %d", sessionId);

    return 0;
}

*/

int relayUsersReadLogin(const RelayUsers* self, const char* nameBuf, RelayUser** outUser)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        RelayUser* user = &self->users[i];
        if (user->name == 0) {
            user->name = tc_str_dup(nameBuf);
            *outUser = user;
            return i;
        }
    }

    *outUser = 0;

    return 0;
}

void relayUsersInit(RelayUsers* self, Clog log)
{
    self->log = log;
    self->capacity = 1024;
    self->users = tc_malloc_type_count(RelayUser, self->capacity);
    tc_mem_clear_type_n(self->users, self->capacity);
}

void relayUsersReset(RelayUsers* self)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        RelayUser* user = &self->users[i];
        if (user->name) {
            free((void*) user->name);
        }
        user->name = 0;
    }
}

void relayUsersDestroy(RelayUsers* self)
{
    tc_free(self->users);
}
