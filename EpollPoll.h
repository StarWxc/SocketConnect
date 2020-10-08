#pragma once

#include "headerFiles.h"
#include <sys/epoll.h>
#include "TimeNode.h"

struct epoll_event;
namespace net
{
    enum {newChannl, existChannel};
    
    class Channel;
    class EventLoop;
    class TimeNode;

    class EpollPoll
    {
    public:
        typedef std::vector<Channel*> ChannelList;

        EpollPoll(EventLoop* loop);
        ~EpollPoll();

        TimeNode poll(int timeOutMs, ChannelList* activeChannel);
        
        //更新监听
        void updateChannel(Channel* channel);
        void removeChannel(Channel* channel);
        
    private:
        void update(int opt, Channel* channel);
    private:
        static const int eventsInitNum = 20;

        int epollfd_;
        EventLoop* loop_;
        std::map<SOCKFD, Channel*> channelMap_;
        std::vector<struct epoll_event> events_;

        TimeNode    time_;
    };
    
}

