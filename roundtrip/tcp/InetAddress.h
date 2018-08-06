#pragma once

#include "Common.h"
#include <string>
#include <netinet/in.h>

class InetAddress : public copyable
{
public:
    explicit InetAddress(uint16_t port);

    InetAddress(const std::string& p, uint_16 port);

    InetAddress(const struct sockaddr_in& addr)
        : addr_(addr)
    {}

    std::string toHostPort() const;

    const struct sockaddr_in& getSockAddrInet() const {return addr_;}
    void setSockAddrInet(const struct sockaddr_in& addr ) { addr_ = addr; }

private:
    struct sockaddr_in addr_;
}