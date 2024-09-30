#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "json.hpp"
#include <thread>
#include <cstdlib>
#include <fstream>

namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;

class filebot
{
private:
    unordered_map<string, string> fileTypeMap;
    const char *homeDirectory;

public:
    filebot(const string &fileTypeMapPath);
    void selectDirectoryToSort();
    void checkEnvironmentVariables();

private:
    void listUserDirectories(vector<fs::path> &directories);
    void moveFiles(const string &directory);
};