#include "EventLoop.h"
#include <sys/eventfd.h>

#include "Channel.h"
#include "EpollPoll.h"
#include "TimeNode.h"

using namespace net;

const int EPollTimeMs = 10000000;

EventLoop::EventLoop(/* args */)
:quit_(false)
{
   
   
   wakeupfd_ = ::eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);

   wakeupChannel_.reset(new Channel(this, wakeupfd_));
   wakeupChannel_->setName("wakeup");

   poll_.reset(new EpollPoll(this));

    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableRead();
} 
EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupfd_);
}

void EventLoop::loop()
{
    quit_ = false;
    
    while(!quit_)
    {
        activeChannel_.clear();

        time_ = poll_->poll(EPollTimeMs, &activeChannel_);
        if(!activeChannel_.empty())
        {
            for(const auto& it : activeChannel_)
            {
                currentChannel_ = it;
                currentChannel_->handleEvent(time_);
            }
            currentChannel_ = nullptr;
        }
        runExecQueues();
    }

    runExecQueues();
   

}

void EventLoop::quit()
{
    quit_ = true;
}

void EventLoop::runInLoop(Func&& fun)
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        execQueues_.push_back(fun);
    }
    wakeup();
}

void EventLoop::runExecQueues()
{
    std::vector<Func> funcs;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        funcs.swap(execQueues_);
    }
    for(auto i : funcs)
    {
        i();
    }
}

void EventLoop::updateChannel(Channel* channel)
{
    poll_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
    poll_->removeChannel(channel);
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int32_t n = ::write(wakeupfd_, &one, sizeof(one));
    if(n < 0)
    {
        LOGE("EventLoop::wakeup errno\n");
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    int32_t n = ::write(wakeupfd_, &one, sizeof(one));
    if( n != sizeof(one))
    {
        LOGE("EventLoop::handleRead errno\n");
    }
}

