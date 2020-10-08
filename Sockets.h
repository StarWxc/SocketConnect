#pragma once

#include "headerFiles.h"
#include "InetAddr.h"

namespace net
{
    namespace sockets
    {
        struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);
        void setNonBlockAndCloseOnExec(SOCKFD sockfd);
        SOCKFD creatNoBlocksockfd(bool on);

        void getLocalName(SOCKFD sockfd, InetAddr *addr);
        void getPeerName(SOCKFD sockfd, InetAddr *addr);
    }
}