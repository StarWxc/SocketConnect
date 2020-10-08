#pragma once

#include "headerFiles.h"
#include "EventLoop.h"
#include "Channel.h"
#include "SockFd.h"
#include "CallBack.h"
#include "InetAddr.h"

namespace net
{
    class TcpConnection: public std::enable_shared_from_this<TcpConnection>
    {
    public:
        typedef std::shared_ptr<TcpConnection>  TcpConnectionPtr;
        typedef std::function<void(TcpConnectionPtr&&)> handleEndCallback;
        typedef std::function<void(TcpConnectionPtr&&, Buffer*,TimeNode)> messageCallback;


        TcpConnection(EventLoop* loop, SOCKFD fd, const std::string& name, InetAddr& localAddr, InetAddr& peerAddr);
        ~TcpConnection();

        std::string name(){ return name_; };
        EventLoop* getLoop(){ return loop_; };

        void send(const void* message, size_t len);

        void shutdown();

        //设置数据处理函数
        void setMessageCallback(const messageCallback& cb)
        { 
            messageCb_ = cb;
        }

        Buffer* getInputBuffer(){ return &inputBuffer_;}
        Buffer* getOutputBuffer(){ return &outputBuffer_; }

        void conntionDestroyed();
    private:
        void handleRead(TimeNode time);
        void handleWrite();

        void sendInLoop(const char* message, size_t len);
        void shutdownInLoop();
        /* data */
        EventLoop* loop_;
        std::string     name_;
        std::unique_ptr<SockFd>     fd_;
        std::unique_ptr<Channel>    channel_;

        const InetAddr    localAddr_;
        const InetAddr    peerAddr_;

        messageCallback     messageCb_;
        
        Buffer      inputBuffer_;
        Buffer      outputBuffer_;
    };


}