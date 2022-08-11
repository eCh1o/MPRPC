#include <iostream>
#include "mprpcapplication.h"
#include "friend.pb.h"

int main(int argc, char **argv)
{
    //整个程序启动以后，想使用mprpc框架来享受rpc服务调用一定要先调用框架的初始化函数
    MprpcApplication::Init(argc, argv);
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    //演示调用远程发布的rpc方法login
    fixbug::GetFriendListRequest request;
    request.set_userid(1000);
    // rpc方法的响应
    fixbug::GetFriendListResponse response;
    //发起rpc方法的调用，同步的调用过程，mprpcchannel::callmethod
    MprpcController controller;
    stub.GetFriendsList(&controller, &request, &response, nullptr); // RpcChannel->RpcChannel::callMethod集中来做所有rpc方法调用的参数序列化和网络发送

    //一次rpc调用完成，读调用结果response
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode())
        {
            std::cout << "rpc GetFriendsList response: " << std::endl;
            int size = response.friends_size();
            for (int i = 0; i < size; ++i)
            {
                std::cout << "index: " << (i + 1) << "name: " << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc GetFriendsList response error : " << response.result().errmsg() << std::endl;
        }
    }

    return 0;
}