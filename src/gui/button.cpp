#include "gui/button.hpp"

#include "input/input.hpp"

#include "time/time.hpp"

// std
#include <utility>
#include <algorithm>
#include <cmath>

static const std::array<SpriteInfo, std::to_underlying(ButtonId::MAX)> s_spriteInfos =
{
    SpriteInfo{"NONE", {0, 0, 0, 0}},
    SpriteInfo{"assets/btn_play.png", {255, 255, 255, 255}}
};

Button::Button(Renderer& renderer,
               TextureManager& textureMng,
               ButtonId id,
               SDL_FPoint pos,
               float width,
               float height)
    : m_sprite(renderer, textureMng, getSpriteInfo(id), pos),
      m_id(id),
      m_pos(pos),
      m_width(width),
      m_height(height),
      m_originalWidth(width),
      m_originalHeight(height)
{
    m_sprite.setPos(m_pos);
}

void Button::update(const Input& input)
{
    m_hovered = checkHover(input.getMousePos());
    m_held = m_hovered && input.isMouseButtonDown(SDL_BUTTON_LEFT);
    m_pressed = m_hovered && input.isMouseButtonReleased(SDL_BUTTON_LEFT);

    handleHoverAnimation();
    handleHeldAnimation();

    if (m_pressed &&
        m_pressedCallback != nullptr)
    {
        m_pressedCallback(*this);
        m_pressed = false;
    }
}

void Button::draw()
{
    m_sprite.draw(m_width, m_height);
}

bool Button::checkHover(SDL_FPoint mousePos) const
{
    return mousePos.x >= m_pos.x &&
           mousePos.x <= m_pos.x + m_width &&
           mousePos.y >= m_pos.y &&
           mousePos.y <= m_pos.y + m_height;
}

void Button::handleHoverAnimation()
{
    SDL_Color col = m_sprite.getColor();
    uint8_t brightness = 0;

    if (m_hovered)
    {
        brightness = (uint8_t)std::max(col.r - s_brightnessChangeRate * Time::deltaTime(),
                                       (float)s_hoverBrightness);
    }
    else
    {
        brightness = (uint8_t)std::min(col.r + s_brightnessChangeRate * Time::deltaTime(),
                                       (float)s_baseBrightness);
    }

    col.r = col.g = col.b = brightness;

    m_sprite.setColor(col);
}

void Button::handleHeldAnimation()
{
    if (m_held)
    {
        const float minWidth = m_originalWidth / s_animPressRatio;
        const float minHeight = m_originalHeight / s_animPressRatio;

        if (m_width > minWidth ||
            m_height > minHeight)
        {
            m_width = std::max(m_width * std::pow(s_animShrinkRate, Time::deltaTime()),
                               minWidth);
            m_height = std::max(m_height * std::pow(s_animShrinkRate, Time::deltaTime()),
                                minHeight);
        }
    }
    else
    {
        if (m_width < m_originalWidth ||
            m_height < m_originalHeight)
        {
            m_width = std::min(m_width * std::pow(s_animGrowthRate, Time::deltaTime()),
                               m_originalWidth);
            m_height = std::min(m_height * std::pow(s_animGrowthRate, Time::deltaTime()),
                                m_originalHeight);
        }
    }
}

const SpriteInfo& Button::getSpriteInfo(ButtonId id) const
{
    return s_spriteInfos.at(std::to_underlying(id));
}