#pragma once

#include "headerFiles.h"
#include "EventLoop.h"
#include "EventLoopThread.h"



namespace net
{
    class ThreadPool
    {
    public:
        typedef std::function<void(EventLoop*)>   ThreadInitCallback;

        ThreadPool();
        ~ThreadPool();

        void Init(EventLoop* baseloop, int numsThreads);
        void start(const ThreadInitCallback& cb = NULL);
        void stop();

        //获取下一个线程
        EventLoop* getNext();
        //获取指定线程
        EventLoop* getLoopForHash(size_t code);

        bool getStrate() const
        {
            return start_;
        }

        const std::string getName() const
        {
            return name_;
        }

        std::string threadName()
        {
            return threads_[next_]->threadNmae();
        }
    private:
        EventLoop*          baseloop_;
        std::string         name_;
        bool                start_;
        int                 numsThread_;
        int                 next_;

        std::vector<std::unique_ptr<EventLoopThread>>     threads_;
        std::vector<EventLoop*>                           loops_;

    };



}