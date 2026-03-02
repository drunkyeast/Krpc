#设置最低版本和项目名称
cmake_minimum_required(VERSION 3.10)
project(Krpc)

#设置全局的C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# muduo-cpp17 第三方依赖 (头文件 + 静态库)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/muduo-cpp17/include)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/muduo-cpp17/lib)

# Krpc 自身头文件
include_directories(${CMAKE_SOURCE_DIR}/include)

#设置全局链接库
set(LIBS
    protobuf
    pthread
    zookeeper_mt
    muduo_core
    glog
)
#添加子目录
add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(example2)
