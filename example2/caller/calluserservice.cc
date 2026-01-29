#include <iostream>
#include "Krpcapplication.h"
#include "../user.pb.h"
#include "Krpcchannel.h"

int main(int argc, char **argv)
{
    // 整个程序启动以后, 像使用Krpc框架来享受rpc服务调用, 一定需要先调用框架的初始化函数(只初始化一次)
    KrpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new KrpcChannel(false)); // stub施磊老师又称为"代理对象"
    // 发现一个点: 服务端, CallMethod方法是在OnMessage中由service(Service UserServiceRpc UserService的对象)调用的.
    //            客户端, CallMethod方法是channel调用的.

    fixbug::LoginRequest request; // 这里用的LoginRequest和LoginResponse, 但服务端用的Message*来创建request和response, 因为OnMessage是统一框架要泛型/动态处理, 但传到Login时又用的LoginResponse接受.
    request.set_name("zhangsan");
    request.set_pwd("123456");

    fixbug::LoginResponse response;
    // 发起rpc方法调用, 同步的rpc调用过程. 这函数里面是channel_->CallMethod(...), KrpcChannel::CallMethod
    // 注意服务端和客户端stub, 都有Login和CallMethod, 且参数都是一样的.  前者4个, 后者5个.
    // CallMethod多了一个method参数, 用于找到调用哪个方法. 剩下四个参数都是controller/request/response/done
    stub.Login(nullptr, &request, &response, nullptr); // 这里会阻塞. 服务端高并发才需要改成非阻塞来提高效率, 这个客户端stub不需要这么麻烦, 没必要改成非阻塞.
    // 操, 乐, 客户端没有使用muduo, 直接用套接字收发!!!!! 这个阻塞, 说到底就是, Login里面的CallMethod里面, send只会循环recv导致的阻塞.

    // 一次rpc调用
    if (response.result().errcode() == 0)
    {
        std::cout << "rpc login response success: " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
    }
    return 0;
}