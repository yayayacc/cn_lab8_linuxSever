#pragma once

#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<vector>

#define MAX_CONNECTIONS 10
#define MAX_BUFFER 4095

class Group;

class User{
    public:
        User() = default;
        User(char const* ac, char const* pw);
        ~User() = default;
        
        void bindFd(int f);

        bool online = false; // 登录状态
        char account[10];
        char pwd[10];
        std::vector<User*> friends; // 好友列表
        std::vector<Group*> groups; // 群列表
        // TODO:应该还要有个消息缓冲区，用以存放离线时的消息
        int fd; // 这个账号登录时使用的是哪个文件描述符
};

class Group{
    public:
        Group() = default;
        ~Group() = default;

        char account[10]; // 群的账号
        std::vector<User*> members; // 群成员
};



class Server {
public:
    Server() = default;
    ~Server() = default;

    // int getDataBuffer();
    // int getMaxConnections();
    void init();
    int creatSocket();
    int getPassivePort();

    void run(int serverFd);

    void processRecv();

private:
    int buffer[MAX_BUFFER];
    // int m_maxConnections = 10;
    fd_set m_readSet;

    int m_passivePort = 6666;
    
    std::vector<User*> allUser; // 系统中所有的用户
    std::vector<Group*> allGroup; // 系统中所有的群聊
};