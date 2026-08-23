#include "gui/checkBox.hpp"

CheckBox::CheckBox(Renderer& renderer,
                   TextureManager& textureMng,
                   SDL_FPoint pos,
                   float width,
                   float height)
    : Button(renderer, textureMng, ButtonId::CHECK_BOX, pos, width, height),
      m_checkMarkSprite(renderer, textureMng, SpriteInfo{"assets/check_mark.png", {255, 255, 255, 255}}, pos),
	  m_checkMarkWidth(width * 0.95f),
	  m_checkMarkHeight(height * 0.95f),
	  m_originalCheckMarkPos(pos)
{
	Button::setPressedCallback(
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

void CheckBox::draw()
{
	Button::draw();

	if (m_activated)
	{
		m_checkMarkSprite.draw(m_checkMarkWidth, m_checkMarkHeight);
	}
}

void CheckBox::applyOffset(SDL_FPoint delta)
{
	Button::applyOffset(delta);

	SDL_FPoint checkMarkPos = SDL_FPoint
	{
		m_originalCheckMarkPos.x + delta.x,
		m_originalCheckMarkPos.y + delta.y
	};

	m_checkMarkSprite.setPos(checkMarkPos);
}