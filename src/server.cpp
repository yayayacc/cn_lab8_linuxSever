#include <string>

#include "package_factory.h"
#include "parser.h"
#include "server.h"
#include "util/json_serializer.h"

// User类成员函数
User::User(const std::string& ac, const std::string& pw) :
    account(ac), pwd(pw) {
}

void User::bindFd(int f) {
    fd = f;
}

// Group类成员函数
Group::Group(const std::string& groupName) :
    account(groupName) {
}

// Server类成员函数
void Server::init() {
    rapidjson::Document data_info;
    JsonSerialzer::deserialze(data_info, std::filesystem::path(XSTR(ROOT_DIR)) / "data/data.json");

    auto Users  = data_info["Users"].GetArray();
    auto Groups = data_info["Groups"].GetArray();

    for (int i = 0; i < Users.Size(); ++i) {
        auto user_info         = Users[i].GetObject();
        auto user_account      = user_info["Account"].GetString();
        auto user_pwd          = user_info["Pwd"].GetString();
        allUsers[user_account] = User{user_account, user_pwd};
    }

    for (int i = 0; i < Users.Size(); ++i) {
        auto user_info    = Users[i].GetObject();
        auto user_account = user_info["Account"].GetString();
        auto user_friends = user_info["Friends"].GetArray();
        for (int j = 0; j < user_friends.Size(); ++j)
            allUsers[user_account].friends[user_friends[j].GetString()] = allUsers[user_account];
    }

    for (int i = 0; i < Groups.Size(); ++i) {
        auto  group_info = Groups[i].GetObject();
        Group group{group_info["Name"].GetString()};
        auto  group_members = group_info["Members"].GetArray();
        for (int j = 0; j < group_members.Size(); ++j) {
            group.members[group_members[j].GetString()] = allUsers[group_members[j].GetString()];
        }
        allGroups[group_info["Name"].GetString()] = group;
    }
}

int Server::getPassivePort() {
    return m_passivePort;
}

int Server::createSocket() {
    struct sockaddr_in saddr;
    int                fd, ret_val;

    // 创建套接字
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        std::cout << "server creation failed" << std::endl;
        return -1;
    }
    std::cout << "server fd is : " << fd << std::endl;

    // 绑定套接字
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(getPassivePort());
    saddr.sin_addr.s_addr = INADDR_ANY; // 代表0.0.0.0
    ret_val               = bind(fd, (struct sockaddr*)&saddr, sizeof(struct sockaddr_in));
    if (ret_val != 0) {
        std::cout << "bind failed" << std::endl;
        close(fd);
        return -1;
    }

    ret_val = listen(fd, MAX_CONNECTIONS); // 10是最大连接数
    if (ret_val != 0) {
        std::cout << "listen failed" << std::endl;
        close(fd);
    }
    return fd;
}

void Server::run(int serverFd) {
    // 初始化文件描述符数组

    for (int i = 1; i < MAX_CONNECTIONS; i++) {
        connections[i] = -1;
    }
    connections[0] = serverFd;

    struct sockaddr_in newAddr;
    socklen_t          newLen = sizeof(struct sockaddr);
    // 循环工作
    std::cout << "开始循环工作" << std::endl;
    while (true) {
        // 清理并且对readSet重新赋值
        memset(buffer, 0, MAX_BUFFER);

        FD_ZERO(&m_readSet);
        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            if (connections[i] >= 0) {
                FD_SET(connections[i], &m_readSet);
            }
        }

        // 开始用select进行监听,num为每次监听到的个数
        int num = select(MAX_CONNECTIONS + 1, &m_readSet, NULL, NULL, NULL);

        // select函数里面后面三个为NULL，代表对输入、异常不感兴趣，以及时间为无限等待
        if (num >= 0) {
            std::cout << num << " event(s)  occur" << std::endl;
            // 如果是服务器fd则代表有新的连接请求
            if (FD_ISSET(serverFd, &m_readSet)) {
                std::cout << "a new connection occurs" << std::endl;

                int newFd = accept(serverFd, (struct sockaddr*)&newAddr, &newLen);
                if (newFd >= 0) {
                    std::cout << "accept a new connection with fd " << newFd << std::endl;

                    // exit(1);

                    for (int i = 0; i < MAX_CONNECTIONS; i++) { // 把第一个空闲的位置给新来的fd
                        if (connections[i] < 0) {
                            connections[i] = newFd;
                            break;
                        }
                    }
                }
                else {
                    std::cout << "accept failed" << std::endl;
                }
                num--;
                if (num == 0) {
                    continue;
                }
            }

            // 如果不是服务器fd那么就代表已有连接中有新的请求
            for (int i = 1; i < MAX_CONNECTIONS; i++) {
                if (FD_ISSET(connections[i], &m_readSet) && (connections[i] > 0)) {
                    // int res = recv(connections[i], buffer, MAX_BUFFER, 0);
                    std::cout << "fd :" << connections[i] << " has sth occurs" << std::endl;
                    processRecv(connections, i);

                    // if(read(connections[i], buffer, MAX_BUFFER) == 0){
                    //     close(connections[i]);
                    //     std::cout<<"delete fd:"<<connections[i]<<std::endl;
                    //     connections[i] = -1;
                    // }
                }
            }
            num--;
            if (!num) {
                continue;
            }

        } // if(num >= 0)

    } // while

    // 关闭所有sockets
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (connections[i] > 0) {
            close(connections[i]);
        }
    }
}

void Server::processRecv(int* connections, int i) {
    Parser parser;

    int readSize;
    // if((readSize = read(fd, buffer, MAX_BUFFER) == 0)){
    //     close(fd);
    //     connections[i] = -1;
    //     std::cout<<"delete fd: "<<fd<<std::endl;
    //     return;
    // }
    // std::cout<<"读成功了"<<std::endl;
    int sum = read(connections[i], buffer, MAX_BUFFER);
    std::cout << "we have read " << sum << "byte(s)" << std::endl;
    if (sum == 0) {
        close(connections[i]);
        std::cout << "fd : " << connections[i] << " has been disconnected!" << std::endl;
        connections[i] = -1;
    }

    parser.parsePkgHead(buffer);
    parser.parseMsg(buffer);
    process(connections[i], i, parser);
}

void Server::process(int fd, int i, Parser parser) {
    switch (int(parser.info.opcode)) {
        case 10: {
            std::cout << "parse  pkg10" << std::endl;
            process10(fd, parser);
        } break;
        case 2: {
            std::cout << "parse  pkg2" << std::endl;
            process2(fd, parser);
        } break;
        case 3: {
            std::cout << "parse  pkg3" << std::endl;
            process3(fd, parser);
        } break;
        case 4: {
            std::cout << "parse  pkg4" << std::endl;
            process4(fd, parser);
        } break;
        case 6: {
            std::cout << "parse  pkg6" << std::endl;
            process6(fd, parser);
        } break;
    }
}

void Server::process10(int fd, Parser parser) {
    auto iter = allUsers.find(parser.info.account);
    if (iter == allUsers.end()) {
        // 查无此人
        auto pkg =
            PackageFactory::getInstance().createPackage1("0000000000", 'c'); // 十个0表示用户不存在
        // sleep(0.5);
        write(fd, pkg.start, pkg.size);
        std::cout << "this account does not exist!"
                  << "   " << pkg.size << std::endl;
        return;
    }

    else {
        parser.msg.resize(parser.info.msglen);

        if (strcmp(iter->second.pwd.c_str(), parser.msg.c_str()) == 0) {
            // 登录成功
            // TODO: 还需要写加载缓存区消息
            std::cout << iter->first << " has logged in successfully!" << std::endl;
            iter->second.online = true;
            auto pkg =
                PackageFactory::getInstance().createPackage1(iter->first.c_str(), 'a');
            // sleep(0.5);
            account2fd.insert(std::pair<std::string, int>(iter->first, fd));
            write(fd, pkg.start, pkg.size);
            // std::cout<<"登录反馈报文发送成功"<<"   "<<pkg.size<<std::endl;
        }

        else {
            auto pkg =
                PackageFactory::getInstance().createPackage1(iter->first.c_str(), 'b');
            // sleep(0.5);
            write(fd, pkg.start, pkg.size);
            std::cout << iter->first << " failed in logging in!" << std::endl;
        }
    }

    std::cout << "get out of process10" << std::endl;
}

void Server::process2(int fd, Parser parser) {
    std::string account = parser.info.account;
    std::string target  = parser.info.target;

    auto iter = account2fd.find(target);
    if (iter == account2fd.end()) {
        // 目标账户名不存在，   UI界面不应有这种情况出现
        return;
    }
    else {
        // std::cout<<"account: "<<parser.info.account<<std::endl;
        // std::cout<<"target: "<<parser.info.target<<std::endl;
        // std::cout<<"msglen: "<<parser.info.msglen<<std::endl;
        // std::cout<<"msg: "<<parser.msg<<std::endl;
        auto pkg =
            PackageFactory::getInstance().createPackage2(parser.info.account, parser.info.target, parser.msg);
        // sleep(0.5);
        int targetFd = iter->second;
        write(targetFd, pkg.start, pkg.size);
        std::cout << "私信发送成功" << std::endl;
        //exit(0);
    }
}

void Server::process3(int fd, Parser parser) {
    std::string account     = parser.info.account;
    std::string groupTarget = parser.info.target;

    auto groupIter = allGroups.find(groupTarget);

    if (groupIter == allGroups.end()) {
        // 目标组不存在，   UI界面不应该有这种情况
        return;
    }
    else {
        auto temGroup = groupIter->second;
        auto pkg =
            PackageFactory::getInstance().createPackage3(parser.info.account, parser.info.target, parser.msg);
        // sleep(0.5);
        for (auto& user : temGroup.members) {
            auto iter = account2fd.find(user.first);
            if (iter == account2fd.end()) {
                // 目标用户不在此组内，   UI界面不应该有这种情况
            }
            else {
                int targetFd = iter->second;
                write(targetFd, pkg.start, pkg.size);
            }
        }
        std::cout << "群消息发送成功" << std::endl;
    }
}

void Server::process4(int fd, Parser parser) {
    std::string account   = parser.info.account;
    std::string target    = parser.info.target;
    uint32_t    fileIndex = parser.info.msgindex;

    auto iter = account2fd.find(target);
    if (iter == account2fd.end()) {
        // 目标账户名不存在，   UI界面不应有这种情况出现
        return;
    }
    else {
        // std::cout<<"account: "<<parser.info.account<<std::endl;
        // std::cout<<"target: "<<parser.info.target<<std::endl;
        // std::cout<<"msglen: "<<parser.info.msglen<<std::endl;
        // std::cout<<"msg: "<<parser.msg<<std::endl;
        auto pkg =
            PackageFactory::getInstance().createPackage4(parser.info.account, parser.info.target,
                                                         parser.info.msgindex, parser.info.filename, parser.msg);
        // sleep(0.5);
        int targetFd = iter->second;
        write(targetFd, pkg.start, pkg.size);
        //exit(0);
    }
}

void Server::process6(int fd, Parser parser) {
    std::string account   = parser.info.account;
    std::string target    = parser.info.target;
    uint32_t    fileIndex = parser.info.msgindex;

    auto iter = account2fd.find(target);
    if (iter == account2fd.end()) {
        // 目标账户名不存在，   UI界面不应有这种情况出现
        return;
    }
    else {
        // std::cout<<"account: "<<parser.info.account<<std::endl;
        // std::cout<<"target: "<<parser.info.target<<std::endl;
        // std::cout<<"msglen: "<<parser.info.msglen<<std::endl;
        // std::cout<<"msg: "<<parser.msg<<std::endl;
        auto pkg =
            PackageFactory::getInstance().createPackage6(parser.info.account, parser.info.target,
                                                         parser.info.filename, parser.info.msgindex);
        // sleep(0.5);
        int targetFd = iter->second;
        write(targetFd, pkg.start, pkg.size);
        //exit(0);
    }
}
