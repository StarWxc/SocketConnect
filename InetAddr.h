#pragma once

#include "./headerFiles.h"
#include <string>

namespace net
{
    class InetAddr
    {
    public:
        InetAddr()
        {
            memset(&addr_, 0, sizeof addr_);
        };
        InetAddr(uint16_t port);
        InetAddr(std::string addr, uint16_t port);

        struct sockaddr_in getAddr(){ return addr_ ;};
        socklen_t getAddrlen();
        void setAddr(struct sockaddr_in &addr){ addr_ = addr ;};

        std::string toIp();
        uint16_t toPort();
    private:
        struct sockaddr_in addr_;
    };


    namespace sockets
    {
        struct in_addr inet_pton(int family, std::string &addr);
        std::string inet_ntop(int family, struct in_addr &addr);
    }
}