#pragma once
#include "stdafx.h"

class TestDataLoader{
public:
    std::map<std::string,std::string> sections;
    TestDataLoader(const char* func);
    std::vector<uint8_t> ReadByteSection(const char* section);
    std::vector<MyString> ReadStrSection(const char* section);
};