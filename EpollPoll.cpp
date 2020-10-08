#include "EpollPoll.h"
#include "Channel.h"
#include "EventLoop.h"
#include "TimeNode.h"

using namespace net;

EpollPoll::EpollPoll(EventLoop* loop)
:loop_(loop), events_(eventsInitNum)
{
    epollfd_ = ::epoll_create1(EPOLL_CLOEXEC);
    if(epollfd_ < 0)
    {
        LOGE("::epoll_cread\n");
    }

}
    
EpollPoll::~EpollPoll()
{
    ::close(epollfd_);
}

void EpollPoll::updateChannel(Channel* channel)
{
    SOCKFD fd = channel->fd();

    if(channel->state_ == newChannl)
    {
        if(channelMap_.find(fd) != channelMap_.end())
        {
            LOGF("%d channel must not exist in channleMap", fd);
        }

        channel->state_ = existChannel;
        channelMap_[fd] = channel;

        update(EPOLL_CTL_ADD, channel);
    }
    else if(channel->state_ == existChannel)
    {
        if(channelMap_.find(fd) == channelMap_.end() or channelMap_[fd] != channel)
        {
            LOGF("fd = %d exist channle errno\n");
        }

        update(EPOLL_CTL_MOD, channel);
        
    }
}

TimeNode EpollPoll::poll(int timeOutMs, ChannelList* activeChannel)
{
    
    int nums = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeOutMs);
   
    if(nums > 0)
    {
        time_.setTime(TimeNode::now());
        for(int i = 0; i < nums; ++i)
        {
            Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
            std::map<SOCKFD,Channel*>::iterator it = channelMap_.find(channel->fd());
            if(it == channelMap_.end() or it->second != channel)
            {
                LOGF("EpollPoll::poll \n");
            
            }
            channel->set_revents(events_[i].events);
            channel->setTimeNode(time_);
            activeChannel->push_back(channel);
        }

    }
    return time_.setTime(TimeNode::now());
}

void EpollPoll::removeChannel(Channel* channel)
{
    SOCKFD fd = channel->fd();
    if(channelMap_.find(fd) == channelMap_.end() or channelMap_[fd] != channel)
    {
        return;
    }
    channel->state_ = newChannl;
    channelMap_.erase(fd);
    update(EPOLL_CTL_DEL, channel);
}

void EpollPoll::update(int opt, Channel* channel)
{
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = channel->events();
    event.data.ptr = channel;

    if(::epoll_ctl(epollfd_, opt, channel->fd(), &event))
    {
        LOGE("epoll update errno\n");
        return;
    }

    printf("update %s :success\n", channel->name().c_str());
}