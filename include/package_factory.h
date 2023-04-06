#pragma once

#include <stdint.h>
#include <string>
#include<stdlib.h>
#include<string.h>
using Byte = char;

struct Package {
    Byte*    start{nullptr};
    uint32_t size;
};
// Singleton & Factory
class PackageFactory {
    

public:
    static PackageFactory& getInstance(){
        static PackageFactory instance;
        return instance;
    }

    // 创建登录时客户端发给服务器的报文
    static Package createLoginPackage(const char* account /* 10 byte */, const std::string& password);
    // 创建登录时候服务器给客户端的反馈报文
    static Package createPackage1(const char* account, char flag);
    // 创建单发消息的报文
    static Package createPackage2(std::string account, std::string target, std::string msg );
    // 创建群发消息的报文
    static Package createPackage3(std::string account, std::string groupTarget, std::string msg );
    // 创建发送文件所需要的报文
    static Package createPackage4(std::string account, std::string target, int mstIndex, std::string filename, std::string msg);
    // 创建重传所需要的报文
    static Package createPackage6(std::string account, std::string target, std::string filename, int fileIndex);
    static void releasePackage(Package pkg);
};
