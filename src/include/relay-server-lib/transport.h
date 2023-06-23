/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_TRANSPORT_H
#define RELAY_SERVER_TRANSPORT_H

#include <stddef.h>
#include <stdint.h>

#include <guise-sessions-client/address.h>

typedef int (*relayServerSendFn)(void* self, const GuiseSclAddress* address, const uint8_t* data, size_t size);
typedef struct RelayServerSendDatagram {
    relayServerSendFn send;
    void* self;
} RelayServerSendDatagram;

#endif
