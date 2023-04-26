#pragma once

#include <cstdio>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>

#ifdef __unix
#define fopen_s(pFile, filename, mode) ((*(pFile)) = fopen((filename), (mode))) == NULL
#endif

class JsonSerialzer {
public:
    static void serialze(const rapidjson::Document& doc, const std::string& path) {
        FILE* fp = fopen(path.c_str(), "w");

        char                                          writeBuffer[65536];
        rapidjson::FileWriteStream                    os(fp, writeBuffer, sizeof(writeBuffer));
        rapidjson::Writer<rapidjson::FileWriteStream> write(os);
        doc.Accept(write);

        fclose(fp);
    }

    static void deserialze(rapidjson::Document& doc, const std::string& path) {
        FILE* fp = fopen(path.c_str(), "r");

        char                      readBuffer[65536];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

        doc.ParseStream(is);

        fclose(fp);
    }
};