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

    static Package createLoginPackage(const char* account /* 10 byte */, const std::string& password);
    static Package createPackage1(const char* account, char flag); // 用以回应登录报文
    static Package createPackage2(std::string account, std::string target, std::string msg ); // 单发的包
    static Package createPackage3(std::string account, std::string target, std::string msg ); // 群发的包


    static void releasePackage(Package pkg);
};
