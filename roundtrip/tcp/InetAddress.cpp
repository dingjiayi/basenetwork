#pragma once

#include "InetAddress.h"

#include <cassert>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

static const in_addr_t kInAddrAny = INADDR_ANY;

assert(sizeof(InetAddress) == sizeof(struct sockaddr_in));

InetAddress::InetAddress(uint16_t port)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_familiy = PF_INET:
    addr_.sin_addr.s_addr = 
}