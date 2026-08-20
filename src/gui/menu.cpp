#include "gui/menu.hpp"

#include "renderer/renderer.hpp"

Menu::Menu(Renderer& renderer, TextureManager& textureMng)
    : m_renderer(renderer),
      m_textureMng(textureMng),
	  m_originalWindowWidth(m_renderer.getWidth()),
	  m_originalWindowHeight(m_renderer.getHeight())
{
}

void Menu::update(const Input& input)
{
	if (m_renderer.wasResized())
	{
		resize(m_renderer.getWidth(), m_renderer.getHeight());
	}

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

Button* Menu::findButton(const std::string& name)
{
	auto it = m_buttons.find(name);
	if (it == m_buttons.end())
	{
		return nullptr;
	}

	return it->second.get();
}

void Menu::resize(uint32_t windowWidth, uint32_t windowHeight)
{
	// 2.0f for enlargement, -2.0f for shrinking 
	float val1 = m_originalWindowWidth < windowWidth ? 2.0f : -2.0f;
	float val2 = m_originalWindowHeight < windowHeight ? 2.0f : -2.0f;
	SDL_FPoint delta = SDL_FPoint
	{
		std::abs((int32_t)(windowWidth - m_originalWindowWidth)) / val1,
		std::abs((int32_t)(windowHeight - m_originalWindowHeight)) / val2
	};

	for (auto& [_, btn] : m_buttons)
    {
        btn->applyOffset(delta);
    }

    for (auto& [_, checkBox] : m_checkBoxes)
    {
        checkBox->applyOffset(delta);
    }
}