#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug; // 方便代码阅读, 实际开发不这样干.
int main()
{
    LoginResponse rsp;
    ResultCode* rc = rsp.mutable_result(); // A* a;是Google推荐的风格, A *a是linux内核风格, 也是一些老程序员的风格.
    // proto文件中定义的result对应多个方法, result()是const的, 所以要用muteble_result(); 
    rc->set_errcode(1);
    rc->set_errmsg("登录处理失败了");

    GetFriendListsResponse rsp2;
    ResultCode* rc2 = rsp2.mutable_result();
    rc2->set_errcode(0); // 没有错误, 就不设置errmsg了.
    User* user1 = rsp2.add_friend_list();
    user1->set_name("张三");
    user1->set_age(20);
    user1->set_gender(User_Gender_MAN); // 或者User::MAN, 但不能是User::Gender::MAN

    User* user2 = rsp2.add_friend_list();
    user2->set_name("李四");
    user2->set_age(21);
    user2->set_gender(User::WOMAN);

    std::cout << rsp2.friend_list_size() << std::endl;

    return 0;
}

int main1()
{
    // 封装了login请求对象的数据
    fixbug::LoginRequest req;
    req.set_name("zhangsan");
    req.set_pwd("123456");

    // 对象数据序列化 =》 char*
    std::string send_str;
    if (req.SerializeToString(&send_str)) // 这个输入输出参数, 用的T*    这是Google的代码规范
    {
        std::cout << send_str.c_str() << std::endl;
    }

    // 从send_str反序列化一个login请求对象
    fixbug::LoginRequest req2;
    if (req2.ParseFromString(send_str)) // 这个属于输入参数, 用的cosnt T&  这是Google的代码规范
    {
        std::cout << req2.name() << std::endl;
        std::cout << req2.pwd() << std::endl;
    }


    return 0;
}