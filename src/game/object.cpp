#include "game/object.hpp"
#include "game/objectUtils.hpp"
#include "game/gridConfig.hpp"
#include "game/utils.hpp"

#include "time/time.hpp"

#include "renderer/sprite.hpp"

// std
#include <array>
#include <algorithm>
#include <utility>

Object::Object(Renderer& renderer,
               TextureManager& textureMng,
               std::size_t uid,
               ObjectId id,
               Cell baseCell) 
    : m_textureMng(textureMng),
      m_sprite(renderer, m_textureMng, ObjectUtils::getSpriteInfo(id), baseCell.toFPoint()),
      m_uid(uid),
      m_id(id),
      m_oldId(id),
      m_type(ObjectUtils::idToType(id)),
      m_oldType(m_type),
      m_textType(ObjectUtils::textIdToTextType(id)),
      m_oldTextType(m_textType),
      m_cell(baseCell)
{
    if (!m_cell.isValidPos())
    {
        m_cell = {0, 0};
    }
}
Object::~Object() noexcept = default;

void Object::update()
{
    m_sprite.updateAnim();
    syncPos();
}

void Object::draw()
{
    m_sprite.draw(GridConfig::cellSize, GridConfig::cellSize);
}

void Object::syncPos()
{
    m_sprite.setPos(m_cell.toFPoint());
}

void Object::move(Direction dir)
{
    m_dir = dir;

    Cell next = GameUtils::getNextCellFromDir(m_cell, dir);

    setCell(next);
}

void Object::faceDir(Direction dir)
{
    float angle = 0.0f;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    switch (dir)
    {
        case Direction::UP:    angle = -90.0f;             break;
        case Direction::DOWN:  angle = 90.0f;              break;
        case Direction::LEFT:  flip = SDL_FLIP_HORIZONTAL; break;
        case Direction::RIGHT: break;

        default: return;
    }

    m_sprite.setAngle(angle);
    m_sprite.setFlip(flip);
}

void Object::changeTo(ObjectId id)
{
    m_id = id;
    m_type = ObjectUtils::idToType(id);
    m_textType = ObjectUtils::textIdToTextType(id);

    m_sprite.reload(ObjectUtils::getSpriteInfo(id));
}

void Object::changeBack()
{
    m_id = m_oldId;
    m_type = ObjectUtils::idToType(m_id);
    m_textType = ObjectUtils::textIdToTextType(m_id);

    m_sprite.reload(ObjectUtils::getSpriteInfo(m_id));
}

void Object::setCell(Cell cell)
{
    if (m_cell == cell || 
        !cell.isValidPos())
    {
        return;
    }

    m_cell = cell;
}