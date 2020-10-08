#include "EventLoopThread.h"
#include "EventLoop.h"

using namespace net;

EventLoopThread::EventLoopThread(const EventLoopInitThread cb, std::string name = "")
:callback_(cb),loop_(nullptr),threadName_(name)
{

}

EventLoopThread::~EventLoopThread()
{
    if(loop_ != nullptr)
    {
        loop_->quit();
        thread_->join();
    }
}

EventLoop* EventLoopThread::startLoop()
{
    thread_.reset(new std::thread(std::bind(&EventLoopThread::threadFun, this)));

    {
        std::unique_lock<std::mutex> lock(mutex_);
        while( !loop_)
        {
            cv_.wait(lock);
        }
    }

    return loop_;
}

void EventLoopThread::stopLoop()
{
    if(loop_ != NULL)
        loop_->quit();
    thread_->join();
}

void EventLoopThread::threadFun()
{
    EventLoop loop;
    if(callback_)
        callback_(&loop);

    {
        std::unique_lock<std::mutex> lock(mutex_);

        loop_ = &loop;
        cv_.notify_all();
    }
    loop.loop();

    loop_ = NULL;
}