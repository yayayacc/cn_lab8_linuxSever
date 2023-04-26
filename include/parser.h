#pragma once

/*
    Only For Test
*/
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <string>
struct Info {
    // std::string account;
    // std::string password;

    char        opcode;   // 1
    std::string account;  // 10
    std::string target;   // 10
    uint32_t    msgindex; // 4
    uint16_t    msglen;   // 2
    std::string filename; // 13
};

struct Parser {
    // parsePkgHead first, then parseContent

public:
    void parsePkgHead(char* start) {
        info.account.resize(10);
        info.target.resize(10);
        info.filename.resize(13);

        // parse
        {
            char* cur = start;

            memcpy(&info.opcode, cur, 1);

            memcpy(const_cast<char*>(info.account.c_str()), cur + 1, 10); // account

            memcpy(const_cast<char*>(info.target.c_str()), cur + 11, 10); // target

            memcpy(&info.msgindex, cur + 21, 4);

            memcpy(&info.msglen, cur + 25, 2);

            memcpy(const_cast<char*>(info.filename.c_str()), cur + 27, 13);
        }
    }

    void parseMsg(char* start) {
        // std::cout << "aaa" << std::endl;

        msg.resize(info.msglen);
        // std::cout << "bbb" << std::endl;

        std::cout << "msglen : " << msg.length() << std::endl
                  << info.msglen << std::endl;

        memcpy(const_cast<char*>(msg.c_str()), start + 40, info.msglen);
    }

    Info        info;
    std::string msg; // 报尾携带的信息
    // uint16_t  msglen; //必须为uint16_t
};