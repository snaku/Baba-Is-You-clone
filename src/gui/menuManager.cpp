#include "gui/menuManager.hpp"
#include "gui/menu.hpp"

// std
#include <print>

MenuManager::MenuManager(Renderer& renderer, TextureManager& textureMng)
    : m_renderer(renderer),
      m_textureMng(textureMng)
{
}

void MenuManager::update(const Input& input)
{
    if (m_activeMenu == nullptr)
    {
        return;
    }

    m_activeMenu->update(input);
}

void MenuManager::draw()
{
    if (m_activeMenu == nullptr)
    {
        return;
    }

    m_activeMenu->draw();
}

Menu& MenuManager::addMenu(const std::string& name)
{
    auto it = m_menus.find(name);
    if (it != m_menus.end())
    {
        std::println("Menu '{}' already exists.", name);
        return *it->second;
    }

    auto menu = std::make_unique<Menu>(m_renderer, m_textureMng);

    Menu& ref = *menu;

    m_menus.emplace(name, std::move(menu));

    return ref;
}

void MenuManager::setActive(const std::string& name)
{
    auto it = m_menus.find(name);
    if (it == m_menus.end())
    {
        return;
    }

    m_activeMenu = it->second.get();
}