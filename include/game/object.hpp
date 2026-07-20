#pragma once

#include "game/cell.hpp"
#include "renderer/sprite.hpp"

// SDL2
#include <SDL2/SDL.h>

// std
#include <memory>
#include <string_view>
#include <vector>
#include <bitset>

class Renderer;
class TextureManager;

enum class Direction
{
    NONE,

    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class ObjectId
{
    NONE,

    BABA,
    WALL,
    WATER,
    FLAG,
    ROCK,
    TEXT_BABA,
    TEXT_WATER,
    TEXT_IS,
    TEXT_AND,
    TEXT_NOT,
    TEXT_YOU,
    TEXT_WIN,
    TEXT_SINK,
    TEXT_STOP,
    TEXT_PUSH,
    TEXT_FLAG,
    TEXT_ROCK,
    TEXT_WALL,

    MAX
};

enum class ObjectType
{
    NONE,

    ENTITY,
    TEXT
};

enum class TextType
{
    NONE,

    NOUN,
    OPERATOR,
    BEHAVIOR
};

enum class OperatorType
{
    NONE,

    IS,
    AND,
    NOT
};

enum class BehaviorType
{
    NONE,

    YOU,
    WIN,
    STOP,
    SINK,
    PUSH,
    MAX
};

class Object
{
public:
    Object(Renderer& renderer, TextureManager& textureMng, ObjectId id, Cell baseCell);
    ~Object() noexcept;

    void update();
    void draw();

    bool isEntity() const { return m_type == ObjectType::ENTITY; }
    bool isText() const { return m_type == ObjectType::TEXT; }

    void move(Direction dir);
    void faceDir(Direction dir);

    void changeTo(ObjectId id);
    void changeBack();

    void setCell(Cell cell);
    bool isAt(Cell cell) const { return m_cell == cell; }

    void requestKill() { m_kill = true; };
    bool shouldGetKilled() { return m_kill; };

    ObjectId textIdToNounId() const;
    BehaviorType textIdToBehavior() const;
    
    static ObjectType getTypeFromId(ObjectId id);
    static TextType getTextTypeFromId(ObjectId id);

    ObjectId getId() const { return m_id; }
    ObjectType getType() const { return m_type; }
    TextType getTextType() const { return m_textType; }
    Cell getCell() const { return m_cell; }
    std::size_t getUID() const { return m_uid; }
    Direction getDir() const { return m_dir; }

    void setColor(Uint8 r, Uint8 g, Uint8 b);

private:
    static std::string_view getSpritePath(ObjectId id);

    TextureManager& m_textureMng;
    Sprite m_sprite;
    std::size_t m_uid;
    static std::size_t s_nextUID;
    ObjectId m_id = ObjectId::NONE;
    ObjectId m_oldId = ObjectId::NONE;
    ObjectType m_type = ObjectType::NONE;
    ObjectType m_oldType = ObjectType::NONE;
    TextType m_textType = TextType::NONE;
    TextType m_oldTextType = TextType::NONE;
    Cell m_cell;
    Cell m_oldCell;
    Direction m_dir;
    float m_angle = 0.0f;
    bool m_dirty = false;
    bool m_kill = false;
    SDL_Color m_col;
};