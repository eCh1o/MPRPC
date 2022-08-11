#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <semaphore.h>
#include <iostream>

//全局的watcher观察器  zkserver给zkclient的通知 单独的线程
void global_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT) //回调的消息类型是和会话相关的消息类型
    {
        if (state == ZOO_CONNECTED_STATE) //连接成功
        {
            sem_t *sem = (sem_t *)zoo_get_context(zh); // zoo_set_context(m_zhandle, &sem)
            sem_post(sem);                             //信号量资源加一
        }
    }
}

ZkClient::ZkClient() : m_zhandle(nullptr) {}

ZkClient::~ZkClient()
{
    if (m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle); //关闭句柄，释放资源
    }
}

//连接zkserver
void ZkClient::Start()
{
    std::string host = MprpcApplication::GetInstance().Getconfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().Getconfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;
    /*
    zookeeper_mt 多线程版本
    zookeeper的API客户端程序提供了三个线程
    API调用线程
    网络IO线程  pthread_create  poll
    当接收到响应时 watcher回调线程
    */
    //异步
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    //返回值不空并不代表连接成功，只能代表创建成功
    if (nullptr == m_zhandle)
    {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    sem_t sem;
    sem_init(&sem, 0, 0);
    //给句柄设置上下文
    zoo_set_context(m_zhandle, &sem);
    //等待信号量获取到资源，才表示连接成功
    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}

void ZkClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
    //先判断path表示的znode是否存在，如果存在，就不再重复创建了
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if (ZNONODE == flag) //表示path的znode节点不存在
    {
        flag = zoo_create(m_zhandle, path, data, datalen,
                          &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if (flag == ZOK)
        {
            std::cout << "znode create success .... path:" << path << std::endl;
        }
        else
        {
            std::cout << "flag :" << flag << std::endl;
            std::cout << "znode create error...path: " << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

//根据指定的path，获取znode节点的值
std::string ZkClient::GetData(const char *path)
{
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);
    if (flag != ZOK)
    {
        std::cout << "get znode error... path: " << path << std::endl;
        return "";
    }
    else
    {
        return buffer;
    }
}