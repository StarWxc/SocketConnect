
#include "TcpConnection.h"

using namespace net;


TcpConnection::TcpConnection(EventLoop* loop, SOCKFD fd, const std::string& name, InetAddr& localAddr, InetAddr& peerAddr)
:loop_(loop),
name_(name),
fd_(new SockFd(fd)),
channel_(new Channel(loop, fd)), 
localAddr_(localAddr), 
peerAddr_(peerAddr)
{
    printf("new TcpConnect \n");
    channel_->setName("TcpConnect");
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
    channel_->enableRead();
    
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    
    fd_->setKeepAlive(true);
   
}
    
TcpConnection::~TcpConnection()
{
}

void TcpConnection::send(const void* message, size_t len)
{
    const char* buf = static_cast<const char*>(message);
    loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, buf, len));
}

void TcpConnection::sendInLoop(const char* message, size_t len)
{
    outputBuffer_.write(message, len);
    if(!channel_->isWriting())
    {
        channel_->enableWrite();
    }
}

void TcpConnection::shutdown()
{
    loop_->runInLoop(bind(&TcpConnection::shutdownInLoop, this));
}

void TcpConnection::shutdownInLoop()
{
    fd_->shutdownWrite();
}

void TcpConnection::conntionDestroyed()
{
    channel_->disableAll();
    channel_->remove();
}

void TcpConnection::handleRead(TimeNode time)
{
    
    char buf[65536];
    int32_t n = ::read(fd_->fd(), buf, sizeof buf);
    
    inputBuffer_.write(buf, n);
    if(n == 0)
    {
        channel_->disableAll();
    }
    else if(n > 0 and messageCb_)
    {
        messageCb_(shared_from_this(), &inputBuffer_, time);
    }
}

void TcpConnection::handleWrite()
{
    if(channel_->isWriting())
    {
        size_t n = ::write(fd_->fd(),outputBuffer_.readPeek(), outputBuffer_.readableSize());
        if(n > 0)
        {
            outputBuffer_.moveReadIndex(n);
            if(outputBuffer_.readableSize() == 0)
            {
                channel_->disableWrite();
            }
        }
    }
}