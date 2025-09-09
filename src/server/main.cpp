#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>
using namespace std;

// 处理服务器ctrl+C结束后，重置user的状态信息state
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " <ip> <port>" << endl;
        cerr << "Example: " << argv[0] << " 192.168.10.128 6010" << endl;
        return -1;
    }

    // signal()：系统调用，用于设置信号处理函数
    signal(SIGINT, resetHandler);  // SIGINT：中断信号（通常由Ctrl+C产生）

    EventLoop loop;  // epoll
    InetAddress addr(argv[1], atoi(argv[2]));  // 服务器监听的地址+端口：IP + Port
    ChatServer server(&loop, addr, "ChatServer");

    
    server.start();  // 启动服务，listenfd epoll_ctl = epoll
    loop.loop();     // epoll_wait 以阻塞的方式等待新用户连接，已连接用户的读写事件等


    return 0;
}
