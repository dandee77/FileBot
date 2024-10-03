#include "rmanager.h"
#include <iostream>

// Singleton instance getter
ResourceManager &ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

// Load and retrieve a texture
Texture2D &ResourceManager::GetTexture(const std::string &name, const std::string &path)
{
    // Check if the texture is already loaded
    if (m_Textures.find(name) == m_Textures.end())
    {
        Texture2D texture = LoadTexture(path.c_str());
        if (texture.id == 0)
        {
            std::cerr << "Failed to load texture: " << path << std::endl;
        }
        else
        {
            m_Textures[name] = std::make_shared<Texture2D>(texture);
        }
    }
    return *m_Textures[name];
}

// Load and retrieve a font
Font &ResourceManager::GetFont(const std::string &name, const std::string &path, int fontSize)
{
    // Check if the font is already loaded
    if (m_Fonts.find(name) == m_Fonts.end())
    {
        Font font = LoadFontEx(path.c_str(), fontSize, nullptr, 0);
        if (font.baseSize == 0)
        {
            std::cerr << "Failed to load font: " << path << std::endl;
        }
        else
        {
            m_Fonts[name] = std::make_shared<Font>(font);
        }
    }
    return *m_Fonts[name];
}

// Load and retrieve a sound
Sound &ResourceManager::GetSound(const std::string &name, const std::string &path)
{
    // Check if the sound is already loaded
    if (m_Sounds.find(name) == m_Sounds.end())
    {
        Sound sound = LoadSound(path.c_str());
        if (sound.stream.buffer == nullptr)
        {
            std::cerr << "Failed to load sound: " << path << std::endl;
        }
        else
        {
            m_Sounds[name] = std::make_shared<Sound>(sound);
        }
    }
    return *m_Sounds[name];
}

// Unload all loaded assets
void ResourceManager::UnloadAll()
{
    // Unload textures
    for (auto &[name, texture] : m_Textures)
    {
        UnloadTexture(*texture);
    }
    m_Textures.clear();

    // Unload fonts
    for (auto &[name, font] : m_Fonts)
    {
        UnloadFont(*font);
    }
    m_Fonts.clear();

    // Unload sounds
    for (auto &[name, sound] : m_Sounds)
    {
        UnloadSound(*sound);
    }
    m_Sounds.clear();
}
