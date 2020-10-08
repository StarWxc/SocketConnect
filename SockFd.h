#pragma once

#include "headerFiles.h"
#include "InetAddr.h"


namespace net
{
    class SockFd
    {
    public:
        SockFd(SOCKFD sockfd):sockfd_(sockfd){};
        ~SockFd(){ ::close(sockfd_);}

        SOCKFD fd()
        {
            return sockfd_;
        }
        void bind(InetAddr &addr);
        void listen();
        
        SOCKFD accept(InetAddr &peerAddr);

        //socket 属性设置函数
        void setReusePortAndAddr(bool portOn, bool addrOn);
        void setKeepAlive(bool on);
        void shutdownWrite();
    private:
        SOCKFD sockfd_;

    };


}