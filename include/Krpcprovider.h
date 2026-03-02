#ifndef _Krpcprovider_H__
#define _Krpcprovider_H__
#include "google/protobuf/service.h"
#include "zookeeperutil.h"
#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpConnection.h"
#include <google/protobuf/descriptor.h>
#include <functional>
#include <string>
#include <unordered_map>

class KrpcProvider
{
public:
    void NotifyService(google::protobuf::Service* service);
    ~KrpcProvider();
    void Run();
private:
    EventLoop event_loop;
    struct ServiceInfo
    {
        google::protobuf::Service* service;
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> method_map;
    };
    std::unordered_map<std::string, ServiceInfo>service_map;
    
    void OnConnection(const TcpConnectionPtr& conn);
    void OnMessage(const TcpConnectionPtr& conn, Buffer* buffer, Timestamp receive_time);
    void SendRpcResponse(const TcpConnectionPtr& conn, google::protobuf::Message* response);
};
#endif 







