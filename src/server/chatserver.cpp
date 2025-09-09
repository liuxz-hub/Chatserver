#include "chatserver.hpp"
#include "chatservice.hpp"
#include "json.hpp"

#include <iostream>
#include <functional>
#include <string.h>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       const string& nameArg)
    : _server(loop, listenAddr ,nameArg), _loop(loop)
{
    // 给服务器注册用户连接的创建和断开回调
        /* 当有新的连接建立或有连接断开时，调用 ChatServer::onConnection;
            setConnectionCallback() 是 TcpServer 的方法，用于设置链接状态变化的回调函数;
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


// 启动服务
void ChatServer::start(){
    _server.start();
    cout << "server.start!!!" << endl;
}


// 上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // 客户端断开链接
    if(conn->connected())
    {
        cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state : online" << endl;
    } 
    else 
    {
        // 客户端异常关闭处理
        ChatService::instance()->clientCloseException(conn);

        cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state : offlone" << endl;
        conn->shutdown();
    }
}

// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    // 数据表反序列化
    json js = json::parse(buf);
    
    /* 目的：完全解耦网络模块的代码和业务模块的代码 */
    // 通过 js["msgid"] 获取->业务事件处理器 MsgHandler
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());    // instance 获取单例对象的接口函数
    // 回调消息绑定的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}