#pragma once

#include "headerFiles.h"
#include <sys/epoll.h>
#include "TimeNode.h"

namespace net
{
    
    class EventLoop;
    class TimeNode;

    typedef std::function<void()> EventCallback;
    typedef std::function<void(TimeNode)> ReadCallback;
    class Channel
    {
    public:
        typedef std::vector<Channel*> ChannelList;
        

        Channel(EventLoop* loop, SOCKFD fd);
        ~Channel();

        //设置触发event
        void set_revents( uint32_t events) { revents_ = events;}
        void add_revents(uint32_t events) { revents_ |= events;}

        uint32_t events() const {return events_;}
        SOCKFD fd(){ return fd_;}

        void setWriteCallback(EventCallback&& cb){ writeCallback_ = cb;}
        void setReadCallback(ReadCallback&& cb){ readCallback_ = cb;}
        void remove();

        //设置监听event
        void enableWrite();
        void disableWrite();
        void disableRead();
        void enableRead();
        void disableAll();

        bool isWriting() const { return events_ & writeEvent; }
        //处理事件函数
        void handleEvent(TimeNode timenode);

        void setName(std::string name){ channelName_ = name;}
        std::string name(){ return channelName_;}

        void setTimeNode(const TimeNode& time){ time_ = time};
        TimeNode getTimeNode(){ return time_};
        
    public:
        int state_;
    private:
        
        void update();
        
        static const int writeEvent;
        static const int readEvent;

        EventLoop*           loop_;
        EventCallback       writeCallback_;
        ReadCallback        readCallback_;
    
        SOCKFD fd_;

        uint32_t revents_;
        uint32_t events_;

        std::string     channelName_;

        TimeNode time_;
    };


}