#pragma once

#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<vector>
#include<map>
#include"parser.h"

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
        std::string account;
        std::string pwd;
        std::vector<User*> friends; // 好友列表
        // std::vector<Group*> groups; // 群列表

        // std::map<std::string, User> mfriend;
        // std::map<std::string, Group> mgroups;
        // TODO:应该还要有个消息缓冲区，用以存放离线时的消息
        int fd; // 这个账号登录时使用的是哪个文件描述符
};

class Group{
    public:
        Group() = default;
        ~Group() = default;

        std::string account; // 群的账号
        std::vector<std::string> members; // 群成员
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

    void processRecv(int fd, int i);// i是套接字在connections里的下标
    void process(int fd, int i, Parser parser);
    void process10(int fd, Parser parser); // 用于处理登录
    void process2(int fd, Parser parser); // 用于处理消息单发

private:
    char buffer[MAX_BUFFER];
    int connections[MAX_CONNECTIONS];// 已经连接上的fd
    // int m_maxConnections = 10;
    fd_set m_readSet;
    fd_set tem_set;
    int m_passivePort = 6666;
    

    std::map<std::string, User> allUsers;
    std::map<std::string, int> account2fd;
    // std::vector<User*> allUser; // 系统中所有的用户
    // std::vector<Group*> allGroup; // 系统中所有的群聊
};