#include "gui/menuManager.hpp"
#include "gui/menu.hpp"

#include "renderer/renderer.hpp"

#include "time/time.hpp"

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

    if (m_renderer.wasResized())
	{
		m_activeMenu->resize(m_renderer.getWidth(), m_renderer.getHeight());
	}

    m_activeMenu->update(input);

    checkInactiveMenusRemoval();
}

void MenuManager::draw()
{
    if (m_activeMenu == nullptr)
    {
        return;
    }

    m_activeMenu->draw();
}

void MenuManager::checkInactiveMenusRemoval()
{
    if (m_menus.empty())
    {
        m_inactiveMenuRemovalTimer = 0.0f;
        return;
    }

    m_inactiveMenuRemovalTimer += Time::deltaTime();
    if (m_inactiveMenuRemovalTimer < s_inactiveMenuRemovalInterval)
    {
        return;
    }

    for (auto it = m_menus.begin(); it != m_menus.end();)
    {
        if (it->first == m_activeMenuName)
        {
            it++;
            continue;
        }

        it = m_menus.erase(it);
    }

    m_inactiveMenuRemovalTimer = 0.0f;
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

void MenuManager::removeMenu(const std::string& name)
{
    setInactive(name); // if it's the active menu
    m_menus.erase(name);
}

void MenuManager::setActive(const std::string& name)
{
    auto it = m_menus.find(name);
    if (it == m_menus.end())
    {
        return;
    }

    m_activeMenu = it->second.get();
    m_activeMenuName = it->first;

    m_activeMenu->resize(m_renderer.getWidth(), m_renderer.getHeight());
}

void MenuManager::setInactive(const std::string& name)
{
    if (name != m_activeMenuName)
    {
        return;
    }

    m_activeMenu = nullptr;
    m_activeMenuName = std::nullopt;
}