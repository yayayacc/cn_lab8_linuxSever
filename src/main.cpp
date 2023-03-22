#include "server.h"
#include "test/test_ds.h"

int main() {
    Sever host;

    // 创建监听套接字，绑定，监听
    int severFd;
    severFd = host.creatSocket();
    if (severFd == -1) {
        std::cout << "sever creation fail" << std::endl;
        return -1;
    }

    // 开始进入工作
    host.run(severFd);

    test_ringQueue();
    test_bitArray();
}
