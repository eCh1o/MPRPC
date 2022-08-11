#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"
int main(int argc, char **argv)
{
    //整个程序启动以后，想使用mprpc框架来享受rpc服务调用一定要先调用框架的初始化函数
    MprpcApplication::Init(argc, argv);
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    //演示调用远程发布的rpc方法login
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    // rpc方法的响应
    fixbug::LoginResponse response;
    //发起rpc方法的调用，同步的调用过程，mprpcchannel::callmethod
    stub.Login(nullptr, &request, &response, nullptr); // RpcChannel->RpcChannel::callMethod集中来做所有rpc方法调用的参数序列化和网络发送

    //一次rpc调用完成，读调用结果response

    if (0 == response.result().errcode())
    {
        std::cout << "rpc login response: " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }
    //演示调用register方法
    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("mprpc");
    req.set_pwd("123456");
    fixbug::RegisterResponse rsp;

    //以同步的方式发送rpc请求，等待返回结果
    stub.Register(nullptr, &req, &rsp, nullptr);
    if (0 == rsp.result().errcode())
    {
        std::cout << "rpc login response: " << rsp.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error : " << rsp.result().errmsg() << std::endl;
    }
    return 0;
}