#pragma once
#include <string>
#include <functional>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/EventLoop.h>
#include <unordered_map>

#include <google/protobuf/descriptor.h>
#include "google/protobuf/service.h"

//框架提供的专门负责发布rpc服务的网络对象类
class RpcProvider
{
public:
    //框架提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);
    //启动rpc服务节点，开始提供rpc远程调用服务
    void Run();

private:
    struct ServiceInfo
    {
        google::protobuf::Service *m_service;                                                    //保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> m_methodMap; //保存服务的方法
    };
    
    //存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
    //新的socket连接回调
    void onConnection(const muduo::net::TcpConnectionPtr &);
    void onMessage(const muduo::net::TcpConnectionPtr &,
                   muduo::net::Buffer *,
                   muduo::Timestamp);
    muduo::net::EventLoop m_eventLoop;
    //Closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr &, google::protobuf::Message*);
};