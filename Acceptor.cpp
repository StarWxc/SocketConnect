#include "Acceptor.h"

using namespace net;

Acceptor::Acceptor(EventLoop* loop, InetAddr& listenAddr)
:loop_(loop),fd_(sockets::creatNoBlocksockfd(true)),channel_(loop,fd_.fd()),listenning_(false)
{
    fd_.setReusePortAndAddr(true, true);
    fd_.bind(listenAddr);
   

    channel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
    channel_.setName("accetp");
}

Acceptor::~Acceptor()
{
    channel_.disableAll();
    channel_.remove();
}

void Acceptor::listen()
{
    listenning_ = true;
    fd_.listen();
    channel_.enableRead();
    
}

void Acceptor::handleRead()
{
    InetAddr peerAddr(0);
    
    SOCKFD connfd = fd_.accept(peerAddr);
    if(connfd > 0)
    {
       
        if(callback_)
        {
            callback_(connfd, peerAddr);
        }
        else
        {
            ::close(connfd);
        }
    } 
}

void Acceptor::deBug()
{
    while (true)
    {
        InetAddr    peeraddr;
        SOCKFD conn = fd_.accept(peeraddr);
        printf("new connection : %d \n", conn);
    }
    
}