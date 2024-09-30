#include "filebot.h"
#include "log.h"

static unordered_map<string, string> loadFileTypeMap(const string &jsonFile)
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

    if (map.empty())
    {
        log::print(logLevel::ERROR, "Could not load file type map.");
        cin.get();
    }

    return map;
}

filebot::filebot(const string &fileTypeMapPath)
{
    // Load the file type map
    fileTypeMap = loadFileTypeMap(fileTypeMapPath);

// Get the home directory of the user
#ifdef _WIN32
    homeDirectory = getenv("USERPROFILE"); // Windows
#else
    homeDirectory = getenv("HOME"); // Linux/macOS
#endif

    if (!homeDirectory)
    {
        log::print(logLevel::ERROR, "Could not determine home directory.");
        cin.get();
    }
}

void filebot::moveFiles(const string &directory)
{
    unordered_map<string, string> fileTypeMap = loadFileTypeMap("src/filetypes.json");

    vector<thread> threads;

    auto processFiles = [&fileTypeMap](const fs::path &file, const fs::path &targetDirectory)
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
    };

    for (const auto &file : fs::directory_iterator(directory))
    {
        if (file.is_regular_file())
        {
            threads.push_back(thread(processFiles, file.path(), fs::path(directory)));
        }
    }

    for (auto &th : threads)
    {
        th.join();
    }
}

void filebot::run()
{
    vector<fs::path> directories;

    // Display user directories
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

    // Select directory
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