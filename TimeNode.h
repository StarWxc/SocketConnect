
#pragma once

#include "headerFiles.h"
#include <time.h>

namespace net
{
    struct tm;

    class TimeNode
    {
    public:
        TimeNode(time_t time = 0)
        :time_(time)
        {};
        ~TimeNode();

        static time_t now()
        {
            return ::time(nullptr);
        }

        time_t getTime()
        {
            return time_;
        }

        void setTime(time_t time)
        {
            time_ = time;
        }

        struct tm getTm()
        {
            return ::localtime(&time_);
        }
    private:
        /* data */
        time_t      time_;

    };
    
    
}
