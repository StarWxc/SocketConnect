#include "ThreadPool.h"


using namespace net;

ThreadPool::ThreadPool()
:start_(false),numsThread_(0),next_(0),name_("thread:"),baseloop_(nullptr)
{

}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::Init(EventLoop* baseloop, int numsThreads)
{
    numsThread_ = numsThreads;
    baseloop_ = baseloop;
}

void ThreadPool::start(const ThreadInitCallback& cb)
{
    if(baseloop_ == NULL)
        return;
    if(start_)
        return;

    for(int i = 0; i < numsThread_; ++i)
    {
        printf("thread %d : start\n", i);
        char buf[128];
        snprintf(buf, sizeof(buf), "%s%d", name_.c_str(), i);
        std::unique_ptr<EventLoopThread> t(new EventLoopThread(cb, buf));
        loops_.push_back(t->startLoop());
        printf("thread eventloop name : %s, %d\n", buf, t->eventidNull());
        threads_.push_back(std::move(t));
    }
    for(auto& it: loops_)
    {
        printf("%d\n", it != nullptr);
    }

    if(numsThread_ == 0 and cb)
    {
        cb(baseloop_);
    }
    start_ = true;
}

void ThreadPool::stop()
{
    for(auto& it : threads_)
    {
        it->stopLoop();
    }
}

EventLoop* ThreadPool::getNext()
{
    if(! start_)
        return NULL;
    EventLoop* loop = baseloop_;

    if(! loops_.empty())
    {
        loop = loops_[next_];
        ++next_;
        if(static_cast<size_t>(next_) == loops_.size())
            next_ = 0;
    }
    return loop;
}

EventLoop* ThreadPool::getLoopForHash(size_t code)
{
    EventLoop* loop = baseloop_;

    if(! start_)
        return NULL;

    if(! loops_.empty())
    {
        loop = loops_[code % loops_.size()];
    }
    return loop;
}

