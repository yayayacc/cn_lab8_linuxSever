#include "server.h"
#include "test/test_ds.h"

int main() {
    Server host;
    host.init(); // 载入用户和群组信息
    // 创建监听套接字，绑定，监听
    int serverFd;
    serverFd = host.creatSocket();
    if (serverFd == -1) {
        std::cout << "server creation fail" << std::endl;
        return -1;
    }

    // 开始进入工作
    host.run(serverFd);

    test_ringQueue();
    test_bitArray();
}
