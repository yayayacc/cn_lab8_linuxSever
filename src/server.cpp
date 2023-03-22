#include "server.h"

Server::Server() {
}

Server::~Server() {
}

int Server::getPassivePort() {
    return m_passivePort;
}

int Server::creatSocket() {
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
    int connections[MAX_CONNECTIONS];
    for (int i = 1; i < MAX_CONNECTIONS; i++) {
        connections[i] = -1;
    }
    connections[0] = serverFd;

    struct sockaddr_in newAddr;
    socklen_t          newLen = sizeof(struct sockaddr);
    // 循环工作
    while (true) {
        // 清理并且对readSet重新赋值
        FD_ZERO(&m_readSet);
        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            if (connections[i] >= 0) {
                FD_SET(connections[i], &m_readSet);
            }
        }

        // 开始用select进行监听,num为每次监听到的个数
        int num = select(MAX_CONNECTIONS, &m_readSet, NULL, NULL, NULL);
        // select函数里面后面三个为NULL，代表对输入、异常不感兴趣，以及时间为无限等待
        if (num >= 0) {
            std::cout << num << " events  occur" << std::endl;
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
                    // processRecv();
                    std::cout<<"fd :"<<connections[i]<<"has sth occurs"<<std::endl;
                    if(read(connections[i], buffer, MAX_BUFFER) == 0){
                        close(connections[i]);
                        std::cout<<"delete fd:"<<connections[i]<<std::endl;
                        connections[i] = -1;
                    }
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

void Server::processRecv() {
}
