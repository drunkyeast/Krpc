#设置最低版本和项目名称
cmake_minimum_required(VERSION 3.0)
project(Krpc)

#设置全局的C++标准
set(CMAKE_CXX_STANDARD 11)#在这里我设置了11，代表使用c++11标准
set(CMAKE_CXX_STANDARD_REQUIRED True)#CMAKE_CXX_STANDARD_REQUIRED表示强制使用指定的标准

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 我添加的
set(CMAKE_BUILD_TYPE Debug) # 我添加的
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0") # 我添加的

#设置头文件目录，供所有子项目使用
#include_directories(${CMAKE_SOURCE_DIR}/src/include) #其中CMAKE_SOURCE_DIR表示当前目录

#查找porotbuf包, 我注释掉了, 感觉没必要啊, 会自动从/usr/include/protobuf目录下找到protobuf头文件
# find_package(Protobuf REQUIRED)
# include_directories(${Protobuf_INCLUDE_DIRS})#Protobuf_INCLUDE_DIRS表示protobuf头文件目录

#设置全局链接库, 注意顺序, 先net再base, 但是是net依赖base!!!
set(LIBS
    protobuf
    pthread
    zookeeper_mt
    muduo_net
    muduo_base
    glog
)
#添加子目录
add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(example2)
