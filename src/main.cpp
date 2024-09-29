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

const std::string embedded_json = R"(
{
    "archives": [".zip", ".rar", ".7z", ".tar", ".gz", ".bz2", ".xz", ".iso", ".dmg", ".pkg"],
    "executables": [".exe", ".msi", ".apk", ".app", ".bat", ".sh", ".deb", ".rpm", ".jar"],
    "scripts": [".py", ".js", ".php", ".rb", ".java", ".pl", ".sh", ".swift", ".html", ".css", ".cpp", ".h", ".cs", ".go", ".ts", ".lua", ".r", ".json", ".xml", ".yml", ".yaml"],
    "images": [".jpg", ".png", ".bmp", ".gif", ".webp", ".hdr", ".raw", ".ico", ".svg", ".psd"],
    "audio": [".mp3", ".wav", ".flac", ".ogg", ".aac", ".m4a", ".ac3", ".wma", ".aiff", ".alac", ".dsd", ".ape", ".opus", ".mka", ".mpc"],
    "videos": [".mp4", ".avi", ".mkv", ".mov", ".wmv", ".webm", ".mpeg", ".flv", ".vob", ".3gp", ".m4v", ".m2ts", ".ts", ".f4v", ".f4p", ".f4a", ".f4b"],
    "documents": [".pdf", ".docx", ".ppt", ".pptx", ".xls", ".xlsx", ".txt", ".md", ".html", ".xps", ".epub", ".mobi", ".azw", ".azw3", ".djvu", ".fb2", ".pdb", ".lit", ".prc", ".ibooks", ".cbz", ".cbr", ".cb7", ".cbt", ".cba", ".chm", ".doc"]
}
)";

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

void listUserDirectories()
{
    const char *homeDir = getHomeDirectory().c_str();

    if (homeDir)
    {
        std::cout << "User Home Directory: " << homeDir << std::endl;

        fs::path desktop = (fs::path(homeDir) / "Desktop").string();
        fs::path documents = (fs::path(homeDir) / "Documents").string();
        fs::path downloads = (fs::path(homeDir) / "Downloads").string();

        std::cout << "1. Desktop: " << desktop << std::endl;
        std::cout << "2. Documents: " << documents << std::endl;
        std::cout << "3. Downloads: " << downloads << std::endl;
        std::cout << "4. Other Directory (Please Enter Path): ";
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
    listUserDirectories();

    int choice;
    std::cin >> choice;

    std::string directory;

    if (choice == 1)
    {
        directory = (fs::path(getenv("USERPROFILE")) / "Desktop").string();
    }
    else if (choice == 2)
    {
        directory = (fs::path(getenv("USERPROFILE")) / "Documents").string();
    }
    else if (choice == 3)
    {
        directory = (fs::path(getenv("USERPROFILE")) / "Downloads").string();
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
    cout << "All files have been moved." << endl;
    std::cin.get();
    return 0;
}
