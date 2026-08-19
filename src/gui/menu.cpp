#include "gui/menu.hpp"

Menu::Menu(Renderer& renderer, TextureManager& textureMng)
    : m_renderer(renderer),
      m_textureMng(textureMng)
{
}

void Menu::update(const Input& input)
{
	for (const auto& [_, btn] : m_buttons)
	{
		btn->update(input);
	}

	for (const auto& [_, checkBox] : m_checkBoxes)
	{
		checkBox->update(input);
	}
}
    
void Menu::draw()
{
	for (const auto& [_, btn] : m_buttons)
	{
		btn->draw();
	}

	for (const auto& [_, checkBox] : m_checkBoxes)
	{
		checkBox->draw();
	}
}