#pragma once
#include <raylib.h>
#include <string>

class Window
{
private:
    Camera2D m_Camera;
    RenderTexture2D m_RenderTexture;

public:
    Window(int width, int height, std::string title);
    void run();
    ~Window();
};