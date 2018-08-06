
#include "SocketsOps.h"

using SA = struct sockaddr;

const SA* sockaddr_cast(const struct sockaddr_in* addr)
{
    return static_cast<const SA*>(implicit_cast<const void*>(addr));
}

SA* sockaddr_cast(struct sockaddr_in* addr)
{
    return static_cast<SA*> (implicit_cast<void*>(addr));
}

