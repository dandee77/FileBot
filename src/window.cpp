#include "window.h"
#include "rmanager.h"

Window::Window(int width, int height, std::string title)
{
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);
}

void Window::run()
{
    Texture2D folderTex = ResourceManager::GetInstance().GetTexture("folder", "assets/folder.png");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawTexture(folderTex, 100, 100, WHITE);
        ClearBackground(DARKGRAY);
        EndDrawing();
    }
}

Window::~Window()
{
    ResourceManager::GetInstance().UnloadAll();
    CloseWindow();
}