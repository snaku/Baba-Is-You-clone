#include "game/object.hpp"
#include "game/config.hpp"
#include "game/utils.hpp"

#include "time/time.hpp"

#include "renderer/sprite.hpp"

// std
#include <array>
#include <algorithm>

static constexpr std::array<std::string_view, (std::size_t)ObjectId::MAX> s_paths =
{
    "NONE",                      // ObjectId::NONE         
    "assets/baba_0_1.png",       // ObjectId::BABA
    "assets/wall_0_1.png",       // ObjectId::WALL
    "assets/water_0_1.png",      // ObjectId::WATER
    "assets/flag_0_1.png",       // ObjectId::FLAG
    "assets/rock_0_1.png",       // ObjectId::ROCK
    "assets/text_baba_0_1.png",  // ObjectId::TEXT_BABA
    "assets/text_water_0_1.png", // ObjectId::TEXT_WATER
    "assets/text_is_0_1.png",    // ObjectId::TEXT_IS
    "assets/text_and_0_1.png",   // ObjectId::TEXT_AND
    "assets/text_not_0_1.png",   // ObjectId::TEXT_NOT
    "assets/text_you_0_1.png",   // ObjectId::TEXT_YOU
    "assets/text_win_0_1.png",   // ObjectId::TEXT_WIN
    "assets/text_sink_0_1.png",  // ObjectId::TEXT_SINK
    "assets/text_stop_0_1.png",  // ObjectId::TEXT_STOP
    "assets/text_push_0_1.png",  // ObjectId::TEXT_PUSH
    "assets/text_flag_0_1.png",  // ObjectId::TEXT_FLAG
    "assets/text_rock_0_1.png",  // ObjectId::TEXT_ROCK
    "assets/text_wall_0_1.png"   // ObjectId::TEXT_WALL
};

Object::Object(Renderer& renderer,
               TextureManager& textureMng,
               std::size_t uid,
               ObjectId id,
               Cell baseCell) 
    : m_textureMng(textureMng),
      m_sprite(renderer, m_textureMng, getSpritePath(id), baseCell.toFPoint()),
      m_uid(uid),
      m_id(id),
      m_oldId(id),
      m_type(getTypeFromId(id)),
      m_oldType(m_type),
      m_textType(getTextTypeFromId(id)),
      m_oldTextType(m_textType),
      m_cell(baseCell)
{
    if (!m_cell.isValidPos())
    {
        m_cell = Cell{0, 0};
    }

    m_col = {255, 255, 255, 255};
}
Object::~Object() noexcept = default;

void Object::update()
{
    if (m_dirty)
    {
        m_sprite.setPos(m_cell.toFPoint());

        m_dirty = false;
    }

    m_sprite.setColor(m_col);
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
    m_type = getTypeFromId(id);
    m_textType = getTextTypeFromId(id);

    m_sprite.reload(getSpritePath(id));
}

void Object::changeBack()
{
    m_id = m_oldId;
    m_type = getTypeFromId(m_id);
    m_textType = getTextTypeFromId(m_id);

    m_sprite.reload(getSpritePath(m_id));
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

ObjectId Object::textIdToNounId() const
{
    if (!isText() ||
        m_textType != TextType::NOUN)
    {
        return ObjectId::NONE;
    }

    switch (m_id)
    {
        case ObjectId::TEXT_BABA:  return ObjectId::BABA;
        case ObjectId::TEXT_FLAG:  return ObjectId::FLAG;
        case ObjectId::TEXT_ROCK:  return ObjectId::ROCK;
        case ObjectId::TEXT_WATER: return ObjectId::WATER;
        case ObjectId::TEXT_WALL:  return ObjectId::WALL;
    }

    return ObjectId::NONE;
}

BehaviorType Object::textIdToBehavior() const
{
    if (!isText() ||
        m_textType != TextType::BEHAVIOR)
    {
        return BehaviorType::NONE;
    }

    switch (m_id)
    {
        case ObjectId::TEXT_YOU:  return BehaviorType::YOU;
        case ObjectId::TEXT_WIN:  return BehaviorType::WIN;
        case ObjectId::TEXT_SINK: return BehaviorType::SINK;
        case ObjectId::TEXT_STOP: return BehaviorType::STOP;
        case ObjectId::TEXT_PUSH: return BehaviorType::PUSH;
    }

    return BehaviorType::NONE;
}

ObjectType Object::getTypeFromId(ObjectId id)
{
    switch (id)
    {
        case ObjectId::BABA:  [[fallthrough]];
        case ObjectId::WALL:  [[fallthrough]];
        case ObjectId::WATER: [[fallthrough]];
        case ObjectId::FLAG:  [[fallthrough]];
        case ObjectId::ROCK:  return ObjectType::ENTITY;

        case ObjectId::TEXT_BABA:  [[fallthrough]];
        case ObjectId::TEXT_WATER: [[fallthrough]];
        case ObjectId::TEXT_IS:    [[fallthrough]];
        case ObjectId::TEXT_NOT:   [[fallthrough]];
        case ObjectId::TEXT_AND:   [[fallthrough]];
        case ObjectId::TEXT_YOU:   [[fallthrough]];
        case ObjectId::TEXT_WIN:   [[fallthrough]];
        case ObjectId::TEXT_SINK:  [[fallthrough]];
        case ObjectId::TEXT_STOP:  [[fallthrough]];
        case ObjectId::TEXT_PUSH:  [[fallthrough]];
        case ObjectId::TEXT_FLAG:  [[fallthrough]];
        case ObjectId::TEXT_ROCK:  [[fallthrough]];
        case ObjectId::TEXT_WALL:  return ObjectType::TEXT;
    }

    return ObjectType::NONE;
}

TextType Object::getTextTypeFromId(ObjectId id)
{
    switch (id)
    {
        case ObjectId::TEXT_BABA:  [[fallthrough]];
        case ObjectId::TEXT_FLAG:  [[fallthrough]];
        case ObjectId::TEXT_WATER: [[fallthrough]];
        case ObjectId::TEXT_ROCK:  [[fallthrough]];
        case ObjectId::TEXT_WALL:  return TextType::NOUN;

        case ObjectId::TEXT_IS:  [[fallthrough]];
        case ObjectId::TEXT_AND: [[fallthrough]];
        case ObjectId::TEXT_NOT: return TextType::OPERATOR;

        case ObjectId::TEXT_YOU:  [[fallthrough]];
        case ObjectId::TEXT_WIN:  [[fallthrough]];
        case ObjectId::TEXT_SINK: [[fallthrough]];
        case ObjectId::TEXT_STOP: [[fallthrough]];
        case ObjectId::TEXT_PUSH: return TextType::BEHAVIOR;
    }

    return TextType::NONE;
}

std::string_view Object::getSpritePath(ObjectId id)
{
    return s_paths.at((std::size_t)id);
}

void Object::setColor(Uint8 r, Uint8 g, Uint8 b)
{
    m_col.r = r;
    m_col.g = g;
    m_col.b = b;
}