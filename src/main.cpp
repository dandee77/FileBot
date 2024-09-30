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

string getHomeDirectory()
{
#ifdef _WIN32
    const char *homeDir = getenv("USERPROFILE"); // Windows
#else
    const char *homeDir = getenv("HOME"); // Linux/macOS
#endif

    if (!homeDir)
    {
        cerr << "Could not determine home directory." << endl;
        return "";
    }

    return string(homeDir);
}

void listUserDirectories(vector<fs::path> &directories)
{
    string homeDir = getHomeDirectory();

    if (!homeDir.empty())
    {
        std::cout << "User Home Directory: " << homeDir << std::endl;

        int index = 1;
        for (const auto &entry : fs::directory_iterator(homeDir))
        {
            if (entry.is_directory())
            {
                directories.push_back(entry.path());
                std::cout << index << ". " << entry.path().string() << std::endl;
                index++;
            }
        }

        std::cout << index << ". Other Directory (Enter full path): ";
    }
    else
    {
        cout << "USERPROFILE not set." << endl;
        cin.get();
    }
}

unordered_map<string, string> loadFileTypeMap(const string &jsonFile)
{
    unordered_map<string, string> fileTypeMap;

    std::ifstream file(jsonFile);
    json j;
    file >> j;

    for (auto &[category, extensions] : j.items())
    {
        for (auto &ext : extensions)
        {
            fileTypeMap[ext] = category;
        }
    }

    return fileTypeMap;
}

void processFile(const fs::path &file, unordered_map<string, string> &fileTypeMap, const fs::path &targetDirectory)
{
    string filename = file.filename().string();
    string extension = fs::path(filename).extension().string();

    if (filename == "main.exe")
        return;

    auto it = fileTypeMap.find(extension);
    if (it != fileTypeMap.end())
    {
        fs::path targetSubDir = targetDirectory / it->second;
        fs::create_directory(targetSubDir);
        fs::rename(file, targetSubDir / filename);
        cout << "Moved " << it->second << " file: " << filename << " to " << targetSubDir << endl;
    }
    else
    {
        fs::path defaultDir = targetDirectory / "defaults";
        fs::create_directory(defaultDir);
        fs::rename(file, defaultDir / filename);
        cout << "Moved unknown file: " << filename << " to " << defaultDir << endl;
    }
}

void moveFiles(const string &directory)
{
    unordered_map<string, string> fileTypeMap = loadFileTypeMap("src/filetypes.json");

    cout << "Successfully loaded file type map." << endl;

    vector<thread> threads;
    for (const auto &file : fs::directory_iterator(directory))
    {
        if (file.is_regular_file())
        {
            threads.push_back(thread(processFile, file.path(), ref(fileTypeMap), fs::path(directory)));
        }
    }

    for (auto &th : threads)
    {
        th.join();
    }
}

void selectDirectoryToSort()
{
    vector<fs::path> directories;
    listUserDirectories(directories);

    int choice;
    std::cin >> choice;

    std::string directory;

    if (choice > 0 && choice <= (int)directories.size())
    {
        directory = directories[choice - 1].string();
    }
    else
    {
        std::cout << "Please enter a full path: ";
        std::cin >> directory;
    }

    std::cout << "Sorting files in: " << directory << std::endl;
    moveFiles(directory);
}

void checkEnvironmentVariables()
{
    for (char **env = environ; *env != 0; env++)
    {
        char *thisEnv = *env;
        std::cout << thisEnv << std::endl;
    }
}

int main()
{
    std::cout << "Application started!" << std::endl;
    // checkEnvironmentVariables();
    selectDirectoryToSort();
    cout << "Application works successfully" << endl;
    std::cin.get();
    return 0;
}
