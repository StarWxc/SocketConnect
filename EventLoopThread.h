#pragma once

#include "headerFiles.h"

namespace net
{
    class EventLoop;

    class EventLoopThread
    {
    public:
        typedef std::function<void(EventLoop*)> EventLoopInitThread;

        EventLoopThread(const EventLoopInitThread cb, std::string name);
        ~EventLoopThread();

        EventLoop* startLoop();
        void stopLoop();

        bool eventidNull(){
            return loop_ == nullptr;
        }
        std::string threadNmae()
        {
            return threadName_;
        }
    private:
        //线程函数
        void threadFun();
        /*data*/
        std::mutex      mutex_;
        std::condition_variable cv_;

        std::unique_ptr<std::thread>    thread_;
        EventLoop*      loop_;

        EventLoopInitThread     callback_;

        std::string         threadName_;
    };
}