#include "filebot.h"
#include "log.h"

filebot::filebot(const string &fileTypeMapPath)
{
    fileTypeMap = loadFileTypeMap(fileTypeMapPath);
    homeDirectory = getHomeDirectory();
}

string filebot::getHomeDirectory()
{
#ifdef _WIN32
    const char *homeDir = getenv("USERPROFILE"); // Windows
#else
    const char *homeDir = getenv("HOME"); // Linux/macOS
#endif

    if (!homeDir)
    {
        log::print(logLevel::ERROR, "Could not determine home directory.");
        return "";
    }

    return string(homeDir);
}

unordered_map<string, string> filebot::loadFileTypeMap(const string &jsonFile)
{
    unordered_map<string, string> map;
    ifstream file(jsonFile);
    json j;
    file >> j;

    for (auto &[category, extensions] : j.items())
    {
        for (auto &ext : extensions)
        {
            map[ext] = category;
        }
    }
    return map;
}

void filebot::listUserDirectories(vector<fs::path> &directories)
{
    if (!homeDirectory.empty())
    {
        cout << "User Home Directory: " << homeDirectory << endl;

        int index = 1;
        for (const auto &entry : fs::directory_iterator(homeDirectory))
        {
            if (entry.is_directory())
            {
                directories.push_back(entry.path());
                cout << index << ". " << entry.path().string() << endl;
                index++;
            }
        }
        cout << index << ". Other Directory (Enter full path): ";
    }
    else
    {
        log::print(logLevel::ERROR, "HOMEDIRECTORY not set.");
        cin.get();
    }
}

void filebot::moveFiles(const string &directory)
{
    log::print(logLevel::INFO, "filetypemap loaded successfully.");

    vector<thread> threads;
    for (const auto &file : fs::directory_iterator(directory))
    {
        if (file.is_regular_file())
        {
            threads.push_back(thread(&filebot::processFile, this, file.path(), fs::path(directory)));
        }
    }

    for (auto &th : threads)
    {
        th.join();
    }
}

void filebot::processFile(const fs::path &file, const fs::path &targetDirectory)
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

void filebot::selectDirectoryToSort()
{
    vector<fs::path> directories;
    listUserDirectories(directories);

    int choice;
    cin >> choice;

    string directory;

    if (choice > 0 && choice <= (int)directories.size())
    {
        directory = directories[choice - 1].string();
    }
    else
    {
        cout << "Please enter a full path: ";
        cin >> directory;
    }

    cout << "Sorting files in: " << directory << endl;
    moveFiles(directory);
}

void filebot::checkEnvironmentVariables() // Use this if getHomeDirectory() fails
{
    for (char **env = environ; *env != 0; env++)
    {
        char *thisEnv = *env;
        cout << thisEnv << endl;
    }
}