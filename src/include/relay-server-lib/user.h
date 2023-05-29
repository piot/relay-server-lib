/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_USER_H
#define RELAY_SERVER_USER_H

typedef struct RelayUser {
    const char* name;
    const RelaySerializeUserId id;
} RelayUser;

#endif
