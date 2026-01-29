#include <iostream>
#include <string>
#include "../user.pb.h"
#include "Krpcapplication.h"
#include "Krpcprovider.h"

// UseService原来是一个本地服务, 提供了两个进程内的本地方法, Login和GetFriendLists 
class UserService : public fixbug::UserServiceRpc // 使用在rpc服务器发布端(rpc服务提供者)
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }

    // 重写基类UserServiceRpc的虚函数 下面这些方法都是框架直接调用的
    // 1. caller  ===>  Login(LoginRequest)  => muduo =>  callee
    // 2. callee  ===>  Login(LoginRequest)  => 交到下面重写的这个Login方法上了, 之后执行回调, 序列化与网络发送给caller
    void Login(::google::protobuf::RpcController* controller, // controller不重要, 关注后面三个参数就好
                        const ::fixbug::LoginRequest* request,
                        ::fixbug::LoginResponse* response,
                        ::google::protobuf::Closure* done)
    {
        // 框架给业务上报了请求参数LoginRequest
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入 包括错误码、错误消息、返回值
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);
    
        // 执行回调操作 执行响应消息对象的序列化与网络发送给caller (都是框架完成)
        done->Run();
    }
};

int main(int argc, char **argv)
{
    // 框架调用的初始化操作。 例如端口号、IP、配置文件等等通过argv传递。 ./server -i ./tets.conf
    KrpcApplication::Init(argc, argv);

    // 创建一个 RPC 服务提供者对象
    KrpcProvider provider;

    // 将 UserService 对象发布到 RPC 节点上，使其可以被远程调用
    provider.NotifyService(new UserService());
    // provider.NotifyService(new ProductService()); // 可以有多个

    // 启动 RPC 服务发布节点，进入阻塞状态，等待远程的 RPC 调用请求
    provider.Run();

    return 0;
}