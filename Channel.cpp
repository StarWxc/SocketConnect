#include "Channel.h"
#include "EventLoop.h"
#include "TimeNode.h"

using namespace net;

const int Channel::readEvent = EPOLLIN | EPOLLPRI;
const int Channel::writeEvent = EPOLLOUT;


Channel::Channel(EventLoop* loop, SOCKFD fd)
:loop_(loop),fd_(fd),state_(0),events_(0),revents_(0),channelName_("")
{
}

Channel::~Channel()
{
}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::remove()
{
    loop_->removeChannel(this);
}

void Channel::enableRead()
{
    events_ |= readEvent;
    printf("read \n");
    update();
}

void Channel::enableWrite()
{
    events_ |= writeEvent;
    update();
}

void Channel::disableRead()
{
    events_ &= ~readEvent;
    update(); 
}

void Channel::disableWrite()
{
    events_ &= ~writeEvent;
    update();
}

void Channel::disableAll()
{
    events_ = 0;
    update();
}

void Channel::handleEvent(TimeNode timenode)
{
    if(revents_ & readEvent)
    {
        if(readCallback_)
            readCallback_(timenode);
    }

    if(revents_ & writeEvent)
    {
        if(writeCallback_)
            writeCallback_();
    }
}
