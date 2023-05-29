/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <relay-server-lib/address.h>
#include <relay-server-lib/unique_id.h>
#include <relay-server-lib/user_session.h>
#include <relay-server-lib/user_sessions.h>
#include <flood/in_stream.h>
#include <relay-serialize/serialize.h>

/// Initialize the user session collection
/// @param self
/// @param log
void relayUserSessionsInit(RelayUserSessions* self, Clog log)
{
    self->log = log;
    self->userSessionCapacity = 1024;
    self->userSessions = tc_malloc_type_count(RelayUserSession, self->userSessionCapacity);
    tc_mem_clear_type_n(self->userSessions, self->userSessionCapacity);
}

void relayUserSessionsReset(RelayUserSessions* self)
{
    for (size_t i = 0; i < self->userSessionCapacity; ++i) {
        RelayUserSession* session = &self->userSessions[i];
        session->user = 0;
    }
}

void relayUserSessionsDestroy(RelayUserSessions* self)
{
    self->userSessionCapacity = 0;
    tc_free(self->userSessions);
}

int relayUserSessionsCreate(RelayUserSessions* sessions, struct RelayUser* user, const RelayAddress* address,
                          RelayUserSession** outSession)
{
    for (size_t i = 0; i < sessions->userSessionCapacity; ++i) {
        RelayUserSession* session = &sessions->userSessions[i];
        if (session->user == 0) {
            Clog userSessionLog;
            userSessionLog.config = sessions->log.config;
            tc_snprintf(session->prefix, 32, "%s/%zu", sessions->log.constantPrefix, i);
            userSessionLog.constantPrefix = session->prefix;
            RelaySerializeUserSessionId uniqueSessionId = relayGenerateUniqueIdFromIndex(i);
            relayUserSessionInit(session, uniqueSessionId, address, user, userSessionLog);
            *outSession = session;
            return 0;
        }
    }
    *outSession = 0;
    return -1;
}

static int userSessionsFind(const RelayUserSessions* self, RelaySerializeUserSessionId uniqueId, const RelayAddress* addr,
                            const RelayUserSession** outSession)
{
    size_t index = relayUniqueIdGetIndex(uniqueId);
    if (index >= self->userSessionCapacity) {
        return -2;
    }

    RelayUserSession* foundSession = &self->userSessions[index];
    if (foundSession->userSessionId != uniqueId) {
        CLOG_C_SOFT_ERROR(&self->log, "wrong user session id, got %016X but wanted %016X", uniqueId, foundSession->userSessionId);
    }
    if (!relayAddressEqual(addr, &foundSession->address)) {
        char addrTemp[64];
        CLOG_C_SOFT_ERROR(&self->log, "wrong address %s vs %s", relayAddressToString(addr, addrTemp, 64),
                          relayAddressToString(&foundSession->address, addrTemp, 64));
        *outSession = 0;
        return -3;
    }

    *outSession = foundSession;

    return 0;
}

int relayUserSessionsReadAndFind(const RelayUserSessions* self, const RelayAddress* address, FldInStream* stream,
                               const RelayUserSession** outSession)
{

    RelaySerializeUserSessionId userSessionId;
    relaySerializeReadUserSessionId(stream, &userSessionId);

    int errorCode = userSessionsFind(self, userSessionId, address, outSession);
    if (errorCode < 0) {
        CLOG_C_WARN(&self->log, "couldn't find user session %d", userSessionId);
        return errorCode;
    }

    return 0;
}
