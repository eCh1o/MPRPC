#include <iostream>
#include <string>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <vector>
#include "logger.h"

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendsList(uint32_t userid)
    {
        std::cout << "do GetFriendsList service!"
                  << "userid: " << userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("zhai chen");
        vec.push_back("li chao");
        vec.push_back("dai hong");
        return vec;
    }

    //重写基类方法
    void GetFriendsList(::google::protobuf::RpcController *controller,
                        const ::fixbug::GetFriendListRequest *request,
                        ::fixbug::GetFriendListResponse *response,
                        ::google::protobuf::Closure *done)
    {
        //获取request参数
        uint32_t userid = request->userid();
        //执行本地业务
        std::vector<std::string> friendList = GetFriendsList(userid);
        //把结果写入response
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for (std::string &name : friendList)
        {
            std::string *p = response->add_friends();
            *p = name;
        }
        done->Run();
    }
};

int main(int argc, char **argv)
{
    LOG_INFO("first log message!");
    LOG_ERROR("fuck you shit");
    MprpcApplication::Init(argc, argv);
    RpcProvider provider;
    provider.NotifyService(new FriendService());
    provider.Run();
}