/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <relay-serialize/serialize.h>
#include <relay-server-lib/connection.h>
#include <relay-server-lib/req_close.h>
#include <relay-server-lib/server.h>
#include <relay-server-lib/user.h>
#include <relay-server-lib/user_session.h>

/// Listener wants the connection to be closed
/// @param self
/// @param userSession
/// @param inStream
/// @param outStream
/// @return
int relayReqClose(RelayServer* self, const RelayUserSession* userSession, FldInStream* inStream,
                  FldOutStream* outStream)
{

    return 0;
}
