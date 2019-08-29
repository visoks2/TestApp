#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <google/protobuf/util/delimited_message_util.h>

#include <data.pb.h>

class dataStorageHandler
{
public:
    dataStorageHandler(/* args */);
    ~dataStorageHandler();
    void Store(gpb::Message_Record aRecord);
    std::string Read(gpb::Message_Record aRecord);
    void Update(gpb::Message_Record aRecord);
    void Remove(gpb::Message_Record aRecord);
private:
    const char * FILE = "db.txt";
    std::vector<gpb::Message_Record> records;
};
