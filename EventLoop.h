#pragma once

#include "headerFiles.h"

namespace net
{   
    class Channel;
    class EpollPoll;
    class TimeNode;

    //中介者模式---中介者
    class EventLoop
    {
    public:
        typedef std::function<void()>  Func;

        EventLoop();
        ~EventLoop();

        //循环运行
        void loop();
        void quit();

        //添加执行函数
        void runInLoop(Func&& fun);
        //更新监听事件
        void updateChannel(Channel* channel);
        void removeChannel(Channel* channel);
    private:
        //唤醒
        void wakeup();
        void handleRead();
        void runExecQueues();
        /* data */
        bool                            quit_;

        std::unique_ptr<EpollPoll>      poll_;
        
        std::unique_ptr<Channel>        wakeupChannel_;
        SOCKFD                          wakeupfd_;

        std::vector<Func>               execQueues_;

        std::mutex                      mutex_;

        std::vector<Channel*>           activeChannel_;
        Channel*                        currentChannel_;
        TimeNode                        time_;
    };
    
    
}