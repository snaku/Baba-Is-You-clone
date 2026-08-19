#include "gui/checkBox.hpp"
#include "gui/button.hpp"

CheckBox::CheckBox(Renderer& renderer,
                   TextureManager& textureMng,
                   SDL_FPoint pos,
                   float width,
                   float height)
    : m_button(std::make_unique<Button>(renderer, textureMng, ButtonId::CHECK_BOX, pos, width, height)),
      m_checkMarkSprite(renderer, textureMng, SpriteInfo{"assets/check_mark.png", {255, 255, 255, 255}}, pos),
	  m_checkMarkWidth(width * 0.95f),
	  m_checkMarkHeight(height * 0.95f)
{
	m_button->setPressedCallback(
		[this](const Button& btn)
		{
			m_activated ^= true;

			if (m_activated &&
				m_activateCallback != nullptr)
			{
				m_activateCallback(*this);
			}
			else if (m_deactivateCallback != nullptr)
			{
				m_deactivateCallback(*this);
			}
		}
	);
}

void CheckBox::update(const Input& input)
{
	m_button->update(input);
}

void CheckBox::draw()
{
	m_button->draw();

	if (m_activated)
	{
		m_checkMarkSprite.draw(m_checkMarkWidth, m_checkMarkHeight);
	}
}