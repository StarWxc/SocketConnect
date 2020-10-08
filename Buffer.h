#pragma once 

#include "headerFiles.h"


namespace net
{
    
	/// +-------------------+------------------+------------------+
	/// | prependable bytes |  readable bytes  |  writable bytes  |
	/// |                   |     (CONTENT)    |                  |
	/// +-------------------+------------------+------------------+
	/// |                   |                  |                  |
	/// 0      <=      readerIndex   <=   writerIndex    <=     size

    class Buffer
    {
    public:
        static const size_t initSize_s = 1024;

        explicit Buffer(size_t initSize = initSize_s)
        :buffer_(initSize),readIndex_(0),writeIndex_(0)
        {
        };
        ~Buffer()
        {};

        //可读数据大小
        size_t readableSize() const
        {
            return writeIndex_ - readIndex_;
        }
        //可写空间大小
        size_t writeableSize() const
        {
            return buffer_.size() - writeIndex_;
        }
        //用完的数据
        size_t uselessSize()
        {
            return readIndex_;
        }
        //可读数据起始指针
        char* readPeek()
        {
            return &*buffer_.begin() + readIndex_;
        }

        //可写位置指针
        char* writePeek()
        {
            return &*buffer_.begin() + writeIndex_;
        }
        //移动readIndex 位置
        void moveReadIndex(size_t n)
        {
            readIndex_ += n;
        }
        
        void write(const char* buf, size_t len)
        {
            if(len > uselessSize() + writeableSize())
                buffer_.resize(buffer_.size() + len);
            
            if(len > writeableSize())
                clearUselessBytes();
            std::copy(buf,buf + len, writePeek());
            writeIndex_ += len;
        }

        size_t read(char* buf, size_t len)
        {
            size_t readsize = readableSize();
            len = len < readsize? len : readsize;
            ::memcpy(buf, readPeek(), len);
            readIndex_ += len;
            return len;
        }



    private:
        //清理已使用空间
        void clearUselessBytes()
        {
            size_t len = readableSize();
            std::copy(&*buffer_.begin() + readIndex_, &*buffer_.begin() + writeIndex_, &*buffer_.begin());
            readIndex_ = 0;
            writeIndex_ = len;
        };
        
        
        /*Date*/
        std::vector<char> buffer_;
        size_t            readIndex_;
        size_t            writeIndex_;
    };

}