#pragma once

#include "Buffer.h"
#include "TcpConnection.h"
#include "TimeNode.h"

typedef int SOCKFD;

namespace net
{   
    
    typedef std::shared_ptr<TcpConnection>  TcpConnectionPtr;

    typedef std::function<void(const TcpConnectionPtr&)> handleEndCallback;
    typedef std::function<void(const TcpConnectionPtr&, Buffer*,TimeNode)> messageCallback;
}