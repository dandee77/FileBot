#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <memory>

class ResourceManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_Textures;
    std::unordered_map<std::string, std::shared_ptr<Font>> m_Fonts;
    std::unordered_map<std::string, std::shared_ptr<Sound>> m_Sounds;

public:
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    static ResourceManager &GetInstance();
    Texture2D &GetTexture(const std::string &name, const std::string &path);
    Font &GetFont(const std::string &name, const std::string &path, int fontSize);
    Sound &GetSound(const std::string &name, const std::string &path);
    void UnloadAll();

private:
    ResourceManager() = default;
};