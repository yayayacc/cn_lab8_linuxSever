#include"package_factory.h"

Package PackageFactory:: createLoginPackage(const char* account /* 10 byte */, const std::string& password) {
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

Package PackageFactory::createPackage1(const char* account, char flag){
    Package pkg;
    pkg.size  = 40 + 1; // 40 Byte is the size of PackageHead
    pkg.start = new Byte[pkg.size];
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

void PackageFactory::releasePackage(Package pkg){

} 