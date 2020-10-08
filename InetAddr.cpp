#include "InetAddr.h"

using namespace net;



InetAddr::InetAddr(uint16_t port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = ::htonl(INADDR_ANY);
    addr_.sin_port = ::htons(port);
}

InetAddr::InetAddr(std::string addr, uint16_t port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_addr = sockets::inet_pton(AF_INET, addr);
    addr_.sin_port = ::htons(port);
}

uint16_t InetAddr::toPort()
{
    return ::ntohs(addr_.sin_port);
}

std::string InetAddr::toIp()
{
    return sockets::inet_ntop(AF_INET, addr_.sin_addr);
}

socklen_t InetAddr::getAddrlen()
{
    return static_cast<socklen_t>(sizeof addr_);
}









struct in_addr sockets::inet_pton(int family, std::string& addr)
{
    struct in_addr addrs;
    ::inet_pton(family, addr.c_str(), &addrs);
    return addrs;
}
std::string sockets::inet_ntop(int family, struct in_addr& addr)
{
    char buff[INET_ADDRSTRLEN];
    ::inet_ntop(family, &addr, buff, sizeof(buff));
    return buff;
}