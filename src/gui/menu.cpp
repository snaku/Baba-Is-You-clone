#include "gui/menu.hpp"

#include "renderer/renderer.hpp"
#include "renderer/sprite.hpp"

Menu::Menu(Renderer& renderer, TextureManager& textureMng)
    : m_renderer(renderer),
      m_textureMng(textureMng),
	  m_originalWindowWidth(m_renderer.getWidth()),
	  m_originalWindowHeight(m_renderer.getHeight())
{
}

void Menu::update(const Input& input)
{
	for (const auto& [_, btn] : m_buttons)
	{
		btn->update(input);
	}
}
    
void Menu::draw()
{
	if (m_backgroundSpr != nullptr)
	{
		m_backgroundSpr->draw(m_renderer.getWidth(), m_renderer.getHeight());
	}
	else if (m_backgroundCol.has_value())
	{
		SDL_Rect rect = SDL_Rect
		{
			0,
			0,
			(int)m_renderer.getWidth(),
			(int)m_renderer.getHeight()
		};

		m_renderer.drawRect(rect, m_backgroundCol.value());
	}

	for (const auto& [_, btn] : m_buttons)
	{
		btn->draw();
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
}

void Menu::setBackground(const std::filesystem::path& path, SDL_Color col)
{
	if (m_backgroundSpr != nullptr)
	{
		m_backgroundSpr->reload(SpriteInfo{{path}, col});
		return;
	}

	m_backgroundCol.reset();

	m_backgroundSpr = std::make_unique<Sprite>(m_renderer,
											   m_textureMng,
											   SpriteInfo{{path}, col},
											   SDL_FPoint{0.0f, 0.0f});
}

void Menu::setBackground(SDL_Color col)
{
	m_backgroundSpr.reset();
	m_backgroundCol = col;
}

void Menu::resetButtons()
{
	for (auto& [_, button] : m_buttons)
	{
		button->reset();
	}
}