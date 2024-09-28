#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

bool search(string &extension, vector<string> &file_types)
{
    for (size_t i = 0; i < file_types.size(); ++i)
    {
        if (extension == file_types[i])
        {
            return true;
        }
    }

    return false;
}

void moveFiles()
{
    vector<string> image = {".jpg", ".png", ".bmp", ".gif", ".webp", ".hdr", ".raw", ".ico", ".svg", ".psd"};

    vector<string> audio = {".mp3", ".wav", ".flac", ".ogg", ".aac", ".m4a", ".ac3"};

    vector<string> video = {".mp4", ".avi", ".mkv", ".mov", ".wmv", ".webm", ".mpeg", ".flv", ".vob"};

    vector<string> documents = {".pdf", ".doc", ".docx", ".ppt", ".pptx", ".xls", ".xlsx", ".txt", ".md", ".html", ".xps", ".epub"};

    for (const auto &file : fs::directory_iterator("."))
    {
        if (file.is_regular_file())
        {
            string filename = file.path().filename().string();
            string extension = fs::path(filename).extension().string();

            if (filename == "FileBot.exe")
                continue;

            if (extension == ".exe")
            {
                fs::create_directory("applications");
                fs::rename(file.path(), fs::path("applications") / filename);
                cout << "Moved exe file: " << filename << endl;
            }
            else if (search(extension, image))
            {
                fs::create_directory("images");
                fs::rename(file.path(), fs::path("images") / filename);
                cout << "Moved image file: " << filename << endl;
            }
            else if (search(extension, audio))
            {
                fs::create_directory("audio");
                fs::rename(file.path(), fs::path("audio") / filename);
                cout << "Moved audio file: " << filename << endl;
            }
            else if (search(extension, video))
            {
                fs::create_directory("videos");
                fs::rename(file.path(), fs::path("videos") / filename);
                cout << "Moved video file: " << filename << endl;
            }
            else if (search(extension, documents))
            {
                fs::create_directory("documents");
                fs::rename(file.path(), fs::path("documents") / filename);
                cout << "Moved document file: " << filename << endl;
            }
            else
            {
                fs::create_directory("defaults");
                fs::rename(file.path(), fs::path("defaults") / filename);
                cout << "Moved unknown file: " << filename << endl;
            }
        }
    }
}

int main()
{
    moveFiles();

    cout << "File sorting completed." << endl;

    return 0;
}
