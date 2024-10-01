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
    // load the file type map
    fileTypeMap = loadFileTypeMap(fileTypeMapPath);

// get the home directory of the user
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
    navigateAndSort(homeDirectory);
    log::print(logLevel::INFO, "Exiting program.");
}

void filebot::navigateAndSort(const fs::path &currentDirectory)
{
#ifdef _WIN32
    system("cls"); // might remove later
#else
    system("clear");
#endif

    static const fs::path startingDirectory = currentDirectory;
    vector<fs::path> directories;
    string action;

    // Display current directory contents
    cout << "\nCurrent Directory: " << currentDirectory.string() << endl;

    int index = 1;
    for (const auto &entry : fs::directory_iterator(currentDirectory))
    {
        if (entry.is_directory())
        {
            directories.push_back(entry.path());
            cout << index << ". " << entry.path().string() << endl;
            index++;
        }
    }

    cout << "\nOptions:\n";
    cout << "Enter directory number to navigate, 'sort' to sort the current directory, or 'back' to move up a directory: ";

    cin >> action;

    if (action == "sort")
    {
        // Sort files in the current directory
        cout << "\nSorting files in: " << currentDirectory.string() << endl;
        moveFiles(currentDirectory.string());
    }
    else if (action == "back")
    {
        // Check if were not at the starting directory (home directory)
        if (currentDirectory != startingDirectory)
        {
            // Move one level up to the parent directory
            fs::path parentDir = currentDirectory.parent_path();
            navigateAndSort(parentDir);
        }
        else
        {
            cout << "\nYou are already at the starting directory. Cannot move back further.\n";
            navigateAndSort(currentDirectory);
        }
    }
    else if (action == "exit")
    {
        return;
    }
    else
    {
        // Check if user input is a valid directory index
        int choice = stoi(action);

        if (choice > 0 && choice <= (int)directories.size())
        {
            // Recursively navigate into the selected subdirectory
            navigateAndSort(directories[choice - 1]);
        }
        else
        {
            cout << "\nInvalid choice. Please try again.\n";
            navigateAndSort(currentDirectory);
        }
    }
}

void filebot::checkEnvironmentVariables() // use this if getHomeDirectory() fails
{
    for (char **env = environ; *env != 0; env++)
    {
        char *thisEnv = *env;
        cout << thisEnv << endl;
    }
}