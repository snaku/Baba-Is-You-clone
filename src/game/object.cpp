#include "game/object.hpp"
#include "game/objectUtils.hpp"
#include "game/config.hpp"
#include "game/utils.hpp"

#include "time/time.hpp"

#include "renderer/sprite.hpp"

// std
#include <array>
#include <algorithm>

static const std::array<SpriteInfo, (std::size_t)ObjectId::MAX> s_spriteInfos =
{
    SpriteInfo{"NONE", {0, 0, 0, 0}},                             // ObjectId::NONE

    SpriteInfo{"assets/baba_0_1.png", {255, 255, 255, 255}},      // ObjectId::BABA
    SpriteInfo{"assets/wall_0_1.png", {41, 49, 65, 255}},         // ObjectId::WALL
    SpriteInfo{"assets/water_0_1.png", {255, 255, 255, 255}},     // ObjectId::WATER
    SpriteInfo{"assets/flag_0_1.png", {237, 226, 133, 255}},      // ObjectId::FLAG
    SpriteInfo{"assets/rock_0_1.png", {194, 158, 70, 255}},       // ObjectId::ROCK

    SpriteInfo{"assets/text_baba_0_1.png", {217, 57, 106, 255}},  // ObjectId::TEXT_BABA
    SpriteInfo{"assets/text_water_0_1.png", {95, 157, 209, 255}}, // ObjectId::TEXT_WATER
    SpriteInfo{"assets/text_is_0_1.png", {255, 255, 255, 255}},   // ObjectId::TEXT_IS
    SpriteInfo{"assets/text_and_0_1.png", {255, 255, 255, 255}},  // ObjectId::TEXT_AND
    SpriteInfo{"assets/text_not_0_1.png", {229, 83, 59, 255}},    // ObjectId::TEXT_NOT
    SpriteInfo{"assets/text_you_0_1.png", {217, 57, 106, 255}},   // ObjectId::TEXT_YOU
    SpriteInfo{"assets/text_win_0_1.png", {237, 226, 133, 255}},  // ObjectId::TEXT_WIN
    SpriteInfo{"assets/text_sink_0_1.png", {95, 157, 209, 255}},  // ObjectId::TEXT_SINK
    SpriteInfo{"assets/text_stop_0_1.png", {27, 92, 28, 255}},    // ObjectId::TEXT_STOP
    SpriteInfo{"assets/text_push_0_1.png", {144, 103, 62, 255}},  // ObjectId::TEXT_PUSH
    SpriteInfo{"assets/text_flag_0_1.png", {237, 226, 133, 255}}, // ObjectId::TEXT_FLAG
    SpriteInfo{"assets/text_rock_0_1.png", {144, 103, 62, 255}},  // ObjectId::TEXT_ROCK
    SpriteInfo{"assets/text_wall_0_1.png", {115, 115, 115, 255}}, // ObjectId::TEXT_WALL
};

Object::Object(Renderer& renderer,
               TextureManager& textureMng,
               std::size_t uid,
               ObjectId id,
               Cell baseCell) 
    : m_textureMng(textureMng),
      m_sprite(renderer, m_textureMng, getSpriteInfo(id), baseCell.toFPoint()),
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
        m_cell = Cell{0, 0};
    }
}
Object::~Object() noexcept = default;

void Object::update()
{
    if (m_dirty)
    {
        m_sprite.setPos(m_cell.toFPoint());

        m_dirty = false;
    }
}

void Object::draw()
{
    m_sprite.draw();
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

    m_sprite.reload(getSpriteInfo(id));
}

void Object::changeBack()
{
    m_id = m_oldId;
    m_type = ObjectUtils::idToType(m_id);
    m_textType = ObjectUtils::textIdToTextType(m_id);

    m_sprite.reload(getSpriteInfo(m_id));
}

void Object::setCell(Cell cell)
{
    if (m_cell == cell || 
        !cell.isValidPos())
    {
        return;
    }

    m_cell = cell;
    m_dirty = true;
}

const SpriteInfo& Object::getSpriteInfo(ObjectId id) const
{
    return s_spriteInfos.at((std::size_t)id);
}