#pragma once

// std
#include <memory>
#include <unordered_map>
#include <string>
#include <optional>

class Renderer;
class TextureManager;
class Input;

class Menu;

class MenuManager
{
public:
    MenuManager(Renderer& renderer, TextureManager& textureMng);

    void update(const Input& input);
    void draw();

    void checkInactiveMenusRemoval();

    Menu& addMenu(const std::string& name);
    void removeMenu(const std::string& name);

    void setActive(const std::string& name);
    void setInactive(const std::string& name);

private:
    Renderer& m_renderer;
    TextureManager& m_textureMng;

    std::unordered_map<std::string, std::unique_ptr<Menu>> m_menus;
    Menu* m_activeMenu = nullptr;
    std::optional<std::string> m_activeMenuName;
    float m_inactiveMenuRemovalTimer = 0.0f;

    static constexpr float s_inactiveMenuRemovalInterval = 120.0f;
};