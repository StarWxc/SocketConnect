#include "SockFd.h"
#include "Sockets.h"
#include "InetAddr.h"

using namespace net;

void SockFd::bind(InetAddr &addr)
{
    struct sockaddr_in addr_in = addr.getAddr();
    if(::bind(sockfd_, sockets::sockaddr_cast(&addr_in), static_cast<socklen_t>(sizeof(addr.getAddr()) ) ) < 0)
    {
        LOGE("bind errno \n");
    }
}

void SockFd::listen()
{
    printf("listening \n");
    if( ::listen(sockfd_, SOMAXCONN) < 0)
    {
        LOGE("listen errno\n");
    }
}

SOCKFD SockFd::accept(InetAddr &peerAddr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof(peerAddr.getAddr()));
    struct sockaddr_in addr_in = peerAddr.getAddr();
    //SOCKFD connfd = ::accept4(sockfd_, sockets::sockaddr_cast(&addr_in), &addrlen, SOCK_CLOEXEC|SOCK_NONBLOCK);
    SOCKFD connfd = ::accept(sockfd_, sockets::sockaddr_cast(&addr_in), &addrlen);
    if(connfd < 0)
    {
        int savedErrno = errno;
        switch (savedErrno)
        {
        case EAGAIN:
            break;
        case ECONNABORTED:
        case EINTR:
        case EPROTO: 
        case EPERM:
        case EMFILE: 
        case EBADF:
        case EFAULT:
        case EINVAL:
        case ENFILE:
            errno = savedErrno;
            break;
        case ENOBUFS:
        case ENOMEM:
        case ENOTSOCK:
        case EOPNOTSUPP:
            LOGE("unexpected error of ::accept %d", savedErrno);
            break;
        default:
            LOGE("unknown error of ::accept %d", savedErrno);
            break;
        }
    }
    return connfd;
}

void SockFd::setReusePortAndAddr(bool portOn, bool IpOn)
{
    int port = portOn ? 1 : 0;
    int ip   = IpOn   ? 1 : 0;

    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &port, static_cast<socklen_t>(sizeof port));
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &ip, static_cast<socklen_t>(sizeof ip));
}

void SockFd::setKeepAlive(bool on)
{
    int opt = on? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &opt, static_cast<socklen_t>(sizeof opt));
}
void SockFd::shutdownWrite()
{
    if(::shutdown(sockfd_, SHUT_WR) < 0)
    {
        LOGE("shutdown Write");
    };
}