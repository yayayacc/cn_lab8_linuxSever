#pragma once

/*
    Only For Test
*/

#include <stdint.h>
#include <string.h>
#include <string>
struct Info {
    // std::string account;
    // std::string password;


    char opcode; // 1
    std::string account; // 10
    std::string target; // 10
    uint32_t msgindex; //4
    uint16_t msglen; // 2
    std::string filename; // 13
};

struct Parser {
    // parsePkgHead first, then parseContent
    
public:
    uint16_t parsePkgHead(char* start) {
        info.account.resize(10);
        info.target.resize(10);
        info.filename.resize(13);


        // parse
        {
            char* cur = start;

            memcpy(&info.opcode, cur, 1);

            memcpy(const_cast<char*>(info.account.c_str()), cur + 1, 10); //account

            memcpy(const_cast<char*>(info.target.c_str()), cur + 11, 10); // target

            memcpy(&info.msgindex, cur + 21, 4);

            memcpy(&info.msglen, cur + 25, 2);

            memcpy(const_cast<char*>(info.filename.c_str()), cur + 27, 13);
        }

        return info.msglen;
    }

    // void parseContent(char* start) {
        
    //     info.password.resize(msglen);
        
    //     memcpy(const_cast<char*>(info.password.c_str()), start, msglen);
    // }

    





    Info info;
    // uint16_t  msglen; //必须为uint16_t
};