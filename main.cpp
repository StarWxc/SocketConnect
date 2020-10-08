#include "headerFiles.h"
#include "TcpServer.h"

using namespace std;

// typedef std::function<void(TcpConnectionPtr&&, Buffer*,TimeNode)> messageCallback;

void handle(net::TcpConnectionPtr&& connPtr, net::Buffer* inputbuf, net::TimeNode time)
{
    char buf[300];

    inputbuf->read(buf, sizeof buf);
    string str(buf);
    
    printf("%s\n", buf);
};


int main()
{
    net::EventLoop* baseloop(new net::EventLoop());
    net::InetAddr localAddr(10000);
    string Mname("TCP:");

    net::TcpServer server(baseloop, Mname, localAddr);
    
    server.setMessageCallback(bind(&handle, placeholders::_1, placeholders::_2, placeholders::_3));
    
    server.start();

    baseloop->loop();
}