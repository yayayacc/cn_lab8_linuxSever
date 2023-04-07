#include "package_factory.h"

void PackageFactory::releasePackage(Package pkg) {
}

Package PackageFactory::createLoginPackage(const char* account /* 10 byte */, const std::string& password) {
    Package pkg;
    pkg.size  = 40 + password.size(); // 40 Byte is the size of PackageHead
    pkg.start = new Byte[pkg.size];

    // clear
    memset(pkg.start, 0, pkg.size);

    // copying
    {
        Byte* cur = pkg.start;

        // 1.type
        uint8_t type = 10;
        memcpy(pkg.start, &type, 1);

        // 2.account
        memcpy(pkg.start + 1, account, 10);

        // 3.pwd_len
        size_t pwd_len = password.size();
        memcpy(pkg.start + 25, &pwd_len, 2);

        // 4.pwd_content
        memcpy(pkg.start + 40, password.c_str(), password.size());
    }

    return pkg;
}

Package PackageFactory::createPackage1(const char* account, char flag) {
    Package pkg;
    pkg.size        = 40 + 1; // 40 Byte is the size of PackageHead
    pkg.start       = new Byte[pkg.size];
    uint16_t length = 1;
    // clear
    memset(pkg.start, 0, pkg.size);

    // copying
    {
        Byte* cur = pkg.start;

        //
        uint8_t type = 1;
        memcpy(pkg.start, &type, 1);

        //
        memcpy(pkg.start + 1, account, 10);

        memcpy(pkg.start + 25, &length, 2);

        memcpy(pkg.start + 40, &flag, 1);
    }

    return pkg;
}

Package PackageFactory::createPackage2(std::string account, std::string target, std::string msg) {
    Package pkg;
    pkg.size  = 40 + msg.size(); // 40 Byte is the size of PackageHead
    pkg.start = new Byte[pkg.size];

    memset(pkg.start, 0, pkg.size);

    {
        // 1.type
        uint8_t type = 2;
        memcpy(pkg.start, &type, 1);

        // 2.account
        memcpy(pkg.start + 1, account.c_str(), 10);

        // 3.target
        memcpy(pkg.start + 11, target.c_str(), 10);

        // 4.msg_len
        size_t msglen = msg.size();
        memcpy(pkg.start + 25, &msglen, 2);

        // 5.msg
        memcpy(pkg.start + 40, msg.c_str(), msg.size());
    }
    // memset(pkg.start+pkg.size, 0, 1);
    return pkg;
}

Package PackageFactory::createPackage3(std::string account, std::string target, std::string msg) {
    Package pkg;
    pkg.size  = 40 + msg.size(); // 40 Byte is the size of PackageHead
    pkg.start = new Byte[pkg.size];

    memset(pkg.start, 0, pkg.size);

    {
        // 1.type
        uint8_t type = 2;
        memcpy(pkg.start, &type, 1);

        // 2.account
        memcpy(pkg.start + 1, account.c_str(), 10);

        // 3.target
        memcpy(pkg.start + 11, target.c_str(), 10);

        // 4.msg_len
        size_t msglen = msg.size();
        memcpy(pkg.start + 25, &msglen, 2);

        // 5.msg
        memcpy(pkg.start + 40, msg.c_str(), msg.size());
    }
    // memset(pkg.start+pkg.size, 0, 1);
    return pkg;
}

Package PackageFactory::createPackage4(std::string account, std::string target, int msgIndex, std::string filename, std::string msg) {
    Package pkg;
    pkg.size  = 40 + msg.size(); // 40 Byte is the size of PackageHead
    pkg.start = new Byte[pkg.size];

    memset(pkg.start, 0, pkg.size);

    {
        // 1.type
        uint8_t type = 4;
        memcpy(pkg.start, &type, 1);

        // 2.account
        memcpy(pkg.start + 1, account.c_str(), 10);

        // 3.groupTarget
        memcpy(pkg.start + 11, target.c_str(), 10);

        // 4.msg_index
        size_t msg_idx = msgIndex;
        memcpy(pkg.start + 21, &msgIndex, 4);

        // 5.msg_len
        size_t msglen = msg.size();
        memcpy(pkg.start + 25, &msglen, 2);

        // 6.filename
        memcpy(pkg.start + 27, filename.c_str(), 13);

        // 7.msg
        memcpy(pkg.start + 40, msg.c_str(), msg.size());
    }
    return pkg;
}

Package PackageFactory::createPackage6(std::string account, std::string target, std::string filename, int fileIndex) {
    Package pkg;
    pkg.size  = 40; // 40 Byte is the size of PackageHead
    pkg.start = new Byte[pkg.size];

    memset(pkg.start, 0, pkg.size);

    {
        // 1.type
        uint8_t type = 6;
        memcpy(pkg.start, &type, 1);

        // 2.account
        memcpy(pkg.start + 1, account.c_str(), 10);

        // 3.groupTarget
        memcpy(pkg.start + 11, target.c_str(), 10);

        // 4.msg_index
        size_t msg_idx = fileIndex;
        memcpy(pkg.start + 21, &msg_idx, 4);

        // // 5.msg_len
        // size_t msglen = msg.size();
        // memcpy(pkg.start + 25, &msglen, 2);

        // 6.filename
        memcpy(pkg.start + 27, filename.c_str(), 13);

        // // 7.msg
        // memcpy(pkg.start + 40, msg.c_str(), msg.size());
    }
    return pkg;
}