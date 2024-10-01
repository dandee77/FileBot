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
    // start in the users home directory
    navigateAndSort(static_cast<fs::path>(homeDirectory));
}

void filebot::navigateAndSort(const fs::path &currentDirectory) // shii recursive
{
    system("cls");
    vector<fs::path> directories;
    string action;

    // display current directory contents
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
    cout << "Enter directory number to navigate or 'sort' to sort the current directory files: ";

    cin >> action;

    if (action == "sort")
    {
        // sort files in the current directory
        cout << "\nSorting files in: " << currentDirectory.string() << endl;
        moveFiles(currentDirectory.string());
    }
    else
    {
        int choice = stoi(action);

        // check if user input is a valid directory index
        if (choice > 0 && choice <= (int)directories.size())
        {
            // recursively navigate into the selected subdir
            navigateAndSort(directories[choice - 1]);
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
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