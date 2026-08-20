#include "game/baseMenu.hpp"

#include "gui/menuManager.hpp"

BaseMenu::BaseMenu(Renderer& renderer, MenuManager& menuMng, const std::string& menuName)
    : m_renderer(renderer),
      m_menu(menuMng.addMenu(menuName)),
      m_name(std::move(menuName)),
      m_menuMng(menuMng)
{
}

void BaseMenu::update(const Input& input)
{
    m_menuMng.update(input);
}

void BaseMenu::draw()
{
    m_menuMng.draw();
}

void BaseMenu::setActive()
{
    m_menuMng.setActive(m_name);
}