#pragma once

#include "parser.h"
#include <errno.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>

#define MAX_CONNECTIONS 1000
// #define MAX_BUFFER 4095
#define MAX_BUFFER 1118192

#define XSTR(s) STR(s)
#define STR(s) #s

class Group;

class User {
public:
    User() = default;
    User(const std::string& ac, const std::string& pw);
    ~User() = default;

    void bindFd(int f);

    bool                         online = false; // 登录状态
    std::string                  account;
    std::string                  pwd;
    std::map<std::string, User>  friends; // 好友列表
    std::map<std::string, Group> groups;

    // TODO:应该还要有个消息缓冲区，用以存放离线时的消息
    int fd; // 这个账号登录时使用的是哪个文件描述符
};

class Group {
public:
    Group()  = default;
    ~Group() = default;

    Group(const std::string& groupName);

    std::string                 account; // 群的账号
    std::map<std::string, User> members; // 群成员
};

class Server {
public:
    Server()  = default;
    ~Server() = default;

    void init();
    int  createSocket();
    int  getPassivePort();

    void run(int serverFd);

    void processRecv(int* connections, int i); // i是套接字在connections里的下标
    void process(int fd, int i, Parser parser);

    void process10(int fd, Parser parser); // 用于处理登录， 报文操作码为10
    void process2(int fd, Parser parser);  // 用于处理消息单发
    void process3(int fd, Parser parser);  // 用于处理群发消息
    void process4(int fd, Parser parser);  // 用于处理文件发送
    void process6(int fd, Parser parser);  // 用于处理文件重传

private:
    char   buffer[MAX_BUFFER];
    int    connections[MAX_CONNECTIONS]; // 已经连接上的fd
    fd_set m_readSet;
    fd_set tem_set;
    int    m_passivePort = 6666;

    std::map<std::string, User>  allUsers;
    std::map<std::string, Group> allGroups;

    std::map<std::string, int> account2fd;
};