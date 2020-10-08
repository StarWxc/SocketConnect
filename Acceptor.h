#pragma once

#include "headerFiles.h"
#include "SockFd.h"
#include "Channel.h"
#include "InetAddr.h"
#include "EventLoop.h"
#include "Sockets.h"


namespace net
{
    class Acceptor
    {
    public:
        typedef std::function<void(SOCKFD, InetAddr&)> newConnectCallback;

        Acceptor(EventLoop* loop, InetAddr& listenAddr);
        ~Acceptor();
        void setNewCallback(const newConnectCallback& cb)
        {
            callback_ = cb;
        }

        void listen();
        bool isListn() const { return listenning_; };

        void deBug();
    private:
        void handleRead();

        /*date*/
        EventLoop*  loop_;
        SockFd      fd_;
        Channel     channel_;
        bool        listenning_;
        newConnectCallback  callback_;
    };






}