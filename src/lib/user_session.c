/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <relay-server-lib/user_session.h>

/// Initialize a user session
/// @param self
/// @param id
/// @param address
/// @param owner
/// @param log
void relayUserSessionInit(RelayUserSession* self, RelaySerializeUserSessionId id, const RelayAddress* address, const struct RelayUser* owner, Clog log)
{
    self->log = log;
    self->userSessionId = id;
    self->address = *address;
    self->user = owner;
}
