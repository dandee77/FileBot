#include "window.h"

Window::Window(int width, int height, std::string title)
{
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);
}

void Window::run()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        EndDrawing();
    }
}

Window::~Window()
{
    CloseWindow();
}