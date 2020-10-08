#pragma once

#include "headerFiles.h"
#include "EventLoop.h"
#include "ThreadPool.h"
#include "Acceptor.h"
#include "CallBack.h"
#include "Sockets.h"

#include <atomic>

namespace net
{
    class TcpServer
    {
    public:
        typedef std::function<void(EventLoop*)>   ThreadInitCallback;

        TcpServer(EventLoop* loop, std::string& name, InetAddr& listenAddr);
        ~TcpServer();

        void setThreadInitCallback(const ThreadInitCallback& cb){ threadInit_ = cb; }
        void setMessageCallback(const messageCallback& cb){ messageCallback_ = cb; }

        void start(int threadNums = 4);
        void stop();

        void connDestroyed();
        void removeConnection(const TcpConnectionPtr& conn);

        
    private:
        void newConnectCallback(SOCKFD fd, InetAddr& peeraddr);
        
        void removeConnectionInLoop(const TcpConnectionPtr& conn);
        
        /* data */
        typedef std::map<std::string, TcpConnectionPtr>  ConnectMap;

        EventLoop*      loop_;
        const std::string   name_;

        std::unique_ptr<Acceptor>   acceptor_;
        std::unique_ptr<ThreadPool> threadPool_;

        messageCallback             messageCallback_;

        ThreadInitCallback          threadInit_;

        std::atomic<bool>           started_;
        int                         nextConnId_;
        ConnectMap                  connectMap_;
    };

}
