/*
    muduo网络库给用户提供了两个主要的类
    TcpServer : 用于编写服务器程序
    TcpClient : 用于编写客户端程序

    epoll + 线程池
    优势 : 能够把网络I/O的代码和业务代码区分开
    网络I/O的代码由网络库封装，重点关注业务代码
    业务代码主要暴露点：1、用户的连接和断开  2、用户的可读写事件
    至于什么时候发生和监听由网络库检测
*/

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

/*基于muduo网络库开发服务器程序
    1、组合TcpServer对象
    2、创建EventLoop事件循环对象的指针
    3、明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
    4、在服务器类的构造函数中注册处理连接断开、读写事件的回调函数
    5、设置合适的服务端线程数量，muduo会自动划分I/O线程和woker线程
*/
class ChatServer{
    public:
        ChatServer(EventLoop *loop,                 // 事件循环对象的指针
                const InetAddress &listenAddr,      // 服务器监听的 地址+端口：IP + Port
                const string &nameArg)              // 服务器名字
            : _server(loop, listenAddr, nameArg),   // 成员初始化列表
              _loop(loop)                           // 将传入的 EventLoop 指针保存到成员变量 _loop
        {
            // 给服务器注册用户连接的创建和断开回调
                /* 当有新的连接建立或现有连接断开时，调用 ChatServer::onConnection;
                    setConnectionCallback() 是 TcpServer 的方法，用于设置连接状态变化的回调函数;
                    std::bind(&ChatServer::onConnection, this, _1) 是绑定机制，将成员函数适配为回调函数;
                        this：绑定当前对象实例（this 指针），确保回调时能访问成员变量
                    _1：参数占位符，表示回调时将传入第一个参数
                */
            _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1)); 


            // 给服务器注册用户读写事件回调
            _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3)); 

            // 设置服务器的线程数量，其中1个I/O线程，用于main Reactor监控新用户的连接事件；3个工作线程，用户的断开是在工作线程中
            _server.setThreadNum(4);
        }

        // 开启事件循环
        void start()
        {
            _server.start();
        }


    private:
        // 专门处理用户的连接创建与断开   epoll  listenfd  accept
        void onConnection(const TcpConnectionPtr &conn){
            if(conn->connected()){
                cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state : online" << endl;
            } else {
                cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state : offlone" << endl;
                conn->shutdown();  // close(fd)
            }
        }

        // 专门处理用户的读写事件
        void onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time){  // 连接状态，缓冲区，接收到数据的时间信息
            string buf = buffer->retrieveAllAsString();
            cout << "recv data:" << buf << "time:" << time.toString() << endl;
            conn->send(buf);
        }

        TcpServer _server;  // #1
        EventLoop *_loop;   // #2 epoll

};



int main(){
    EventLoop loop;  // epoll
    InetAddress addr("192.168.10.128", 6010);  // 服务器监听的 地址+端口：IP + Port
    
    ChatServer server(&loop, addr, "ChatServer"); 

    server.start();  // 启动服务，listenfd epoll_ctl = epoll
    loop.loop();     // epoll_wait 以阻塞的方式等待新用户连接，已连接用户的读写事件等

    return 0;
}