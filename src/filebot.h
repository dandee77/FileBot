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
    string homeDirectory;

public:
    filebot(const string &fileTypeMapPath);
    void selectDirectoryToSort();
    void checkEnvironmentVariables();

private:
    string getHomeDirectory();
    unordered_map<string, string> loadFileTypeMap(const string &jsonFile);
    void listUserDirectories(vector<fs::path> &directories);
    void moveFiles(const string &directory);
    void processFile(const fs::path &file, const fs::path &targetDirectory);
};