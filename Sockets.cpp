#include "Sockets.h"
#include "InetAddr.h"
using namespace net;
    
        struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in* addr)
        {
            
            return  static_cast<struct sockaddr*>(static_cast<void*>(addr));
        }


        void sockets::setNonBlockAndCloseOnExec(SOCKFD sockfd)
        {
            int flags = ::fcntl(sockfd, F_GETFL, 0);
            flags |= O_NONBLOCK;
            int ret = ::fcntl(sockfd, F_SETFL, flags);
            
            flags = ::fcntl(sockfd, F_GETFD, 0);
            flags |= FD_CLOEXEC;
            ret = ::fcntl(sockfd, F_SETFD, flags);
            

            (void)ret;
        }

        SOCKFD sockets::creatNoBlocksockfd(bool on)
        {
            SOCKFD fd = ::socket(AF_INET, SOCK_STREAM, 0);
            if(on)
            {
                setNonBlockAndCloseOnExec(fd);
            }
            return fd;
        }

        void sockets::getLocalName(SOCKFD sockfd, InetAddr *addr)
        {
            socklen_t addrlen = addr->getAddrlen();
            sockaddr_in addr_ = addr->getAddr();
            ::getsockname(sockfd, sockaddr_cast(&addr_),&addrlen);
        }

        void sockets::getPeerName(SOCKFD sockfd, InetAddr *addr)
        {
            socklen_t addrlen = addr->getAddrlen();
            sockaddr_in addr_ = addr->getAddr();
            ::getpeername(sockfd, sockaddr_cast(&addr_),&addrlen);
        }

