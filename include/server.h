#pragma once

#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_CONNECTIONS 10
#define MAX_BUFFER 4095

class Server {
public:
    Server();
    ~Server();

    // int getDataBuffer();
    // int getMaxConnections();

    int creatSocket();
    int getPassivePort();

    void run(int serverFd);

    void processRecv();

private:
    int buffer[MAX_BUFFER];
    // int m_maxConnections = 10;
    fd_set m_readSet;

    int m_passivePort = 6666;
};