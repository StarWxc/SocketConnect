#include "TcpServer.h"


using namespace net;

TcpServer::TcpServer(EventLoop* loop, std::string& name, InetAddr& listenAddr)
:loop_(loop),
name_(name),
acceptor_(new Acceptor(loop, listenAddr)),
nextConnId_(1),
started_(false)
{
    acceptor_->setNewCallback(std::bind(&TcpServer::newConnectCallback, this, std::placeholders::_1, std::placeholders::_2));

}
    
TcpServer::~TcpServer()
{
    stop();
}

void TcpServer::start(int threadNums /* = 4*/)
{
    if(!started_)
    {
        threadPool_.reset(new ThreadPool());

        threadPool_->Init(loop_, threadNums);
        threadPool_->start();

        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));

        started_ = true;
    }
}

void TcpServer::stop()
{
    if(!started_)
    {
        return;
    }
    for( auto& it : connectMap_)
    {
        it.second->getLoop()->runInLoop(std::bind(&TcpConnection::conntionDestroyed, it.second));
        it.second.reset();
    }
    threadPool_->stop();
    started_ = false;
    
}

void TcpServer::newConnectCallback(SOCKFD fd, InetAddr& peerAddr)
{
    EventLoop* ioLoop = threadPool_->getNext();
    if(ioLoop == nullptr)
    {
        printf("ioloop is null\n");
    }
    
    char buf[32];
    snprintf(buf, sizeof buf, "%d", nextConnId_);
    std::string connName = name_ + buf;
    nextConnId_++;
    
    //debug
    std::cout<<"thread name:"<<threadPool_->threadName()<<std::endl;

    InetAddr localAddr(0);
    sockets::getLocalName(fd, &localAddr);

    TcpConnectionPtr connPtr(new TcpConnection(ioLoop, fd, connName, localAddr, peerAddr));
    connectMap_[connName] = connPtr;

    connPtr->setMessageCallback(messageCallback_);
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}
void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    connectMap_.erase(conn->name());
    EventLoop* loop = conn->getLoop();
    loop->runInLoop(std::bind(&TcpConnection::conntionDestroyed, conn));
}