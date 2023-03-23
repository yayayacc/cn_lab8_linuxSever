#pragma once

#include <stdint.h>
#include <string>
#include <string.h>

using Byte = char;

struct LoginPackage {
    /*
        head(40 byte):
            type    : 1
            account : 10
            pad0    : 10 + 5
            pwd_len : 2
            pad1    : 12
        content:
            pwd
    */
    Byte*    start{nullptr};
    uint32_t size;
};

// Singleton & Factory
class PackageFactory {
public:
    PackageFactory()  = default;
    ~PackageFactory() = default;

    PackageFactory(const PackageFactory&)            = delete;
    PackageFactory& operator=(const PackageFactory&) = delete;

    static PackageFactory& getInstance() {
        static PackageFactory instance;
        return instance;
    }

    LoginPackage createLoginPackage(const char* account /* 10 byte */, const std::string& password) {
        LoginPackage pkg;
        pkg.size  = 40 + password.size(); // 40 Byte is the size of PackageHead
        pkg.start = new Byte[pkg.size];

        // clear
        memset(pkg.start, 0, pkg.size);

        // copying
        {
            Byte* cur = pkg.start;

            // 1.type
            Byte type = 10;
            memcpy(pkg.start, &type, 1);

            // 2.account
            memcpy(pkg.start + 1, account, 10);

            // 3.pwd_len
            size_t pwd_len = password.size();
            memcpy(pkg.start + 1 + 10 + 15, &pwd_len, 2);

            // 4.pwd_content
            memcpy(pkg.start + 40, password.c_str(), password.size());
        }

        return pkg;
    }

    void releaseLoginPackage(LoginPackage pkg) {
        if (pkg.start) delete[] pkg.start;
    }

private:
};
