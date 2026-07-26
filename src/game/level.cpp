#include "game/level.hpp"
#include "game/config.hpp"
#include "game/utils.hpp"

#include "input/input.hpp"

#include "renderer/renderer.hpp"

#include "time/time.hpp"

// std
#include <algorithm>
#include <fstream>
#include <format>
#include <print>

Level::Level(Renderer& renderer,
             TextureManager& textureMng,
             const Input& input,
             Fade& fade)
    : m_renderer(renderer),
      m_input(input),
      m_objectMng(renderer, textureMng),
      m_transition(*this, fade),
      m_border(renderer),
      m_grid(GameConfig::gridWidth, GameConfig::gridHeight),
      m_ruleSystem(m_objectMng, m_grid),
      m_movementSystem(m_objectMng, m_grid, m_ruleSystem, m_input)
{
    m_objectMng.setAddCallback(
        [this](const Object& object)
        {
            m_grid.addObjectAt(object.getUID(), object.getCell());
        }
    );

    m_objectMng.setRemoveCallback(
        [this](const Object& object)
        {
            m_grid.removeObjectAt(object.getUID(), object.getCell());
            m_ruleSystem.eraseObjectWithTransformation(object.getUID());
            std::erase(m_youObjectsUID, object.getUID());
        }
    );
}
Level::~Level() noexcept = default;

void Level::load()
{
    if (m_state == LevelState::WIN)
    {
        m_id++;
    }

    std::string fileName = std::format("level_{}.txt", m_id);

    LevelDefinition def = LevelLoader::read(fileName);
    if (!def.isValid)
    {
        m_state = LevelState::IDLE;
        return;
    }

    initFromDef(def);
}

void Level::initFromDef(const LevelDefinition& def)
{
    GameConfig::gridWidth = def.width;
    GameConfig::gridHeight = def.height;
    GameConfig::cellSize = std::min(GameConfig::windowWidth / def.width, GameConfig::windowHeight / def.height);

    GameConfig::gridOffset.x = (GameConfig::windowWidth - GameConfig::gridWidth * GameConfig::cellSize) / 2;
    GameConfig::gridOffset.y = (GameConfig::windowHeight - GameConfig::gridHeight * GameConfig::cellSize) / 2;

    m_grid.resize(GameConfig::gridWidth, GameConfig::gridHeight);
    m_border.resize(GameConfig::gridWidth, GameConfig::gridHeight);

    for (const auto& data : def.objects)
    {
        if (!data.cell.isValidPos())
        {
            continue;
        }

        m_objectMng.addObject(data.id, data.cell);
    }

    m_ruleSystem.requestDirty();
    m_state = LevelState::PLAYING;
}

void Level::buildYouObjects()
{
    m_youObjectsUID.clear();

    m_objectMng.forEach(
        [this](const Object& object)
        {
            if (m_ruleSystem.hasBehavior(object.getId(), BehaviorType::YOU))
            {
                m_youObjectsUID.push_back(object.getUID());
            }
        }
    );
}

void Level::checkReload()
{
    m_reloadTimer += Time::deltaTime();

    if (m_reloadTimer >= 1.0f &&
        m_input.isKeyDown(SDL_SCANCODE_R))
    {
        m_reloadRequested = true;
    }

    if (!reloadRequested())
    {
        return;
    }

    if (m_canReload)
    {
        m_reloadRequested = false;
        m_canReload = false;
        m_reloadTimer = 0.0f;

        reload();
    }
}

void Level::reload()
{
    std::println("Reloading level");

    m_ruleSystem.clear();
    m_grid.clearObjects();
    m_objectMng.clear();

    load();
}

void Level::updateStateIdle()
{
}

void Level::updateStatePlaying()
{
    checkReload();

    bool canMove = m_movementSystem.updateMoveTimer();
    bool moved = false;

    m_objectMng.forEach(
        [&](Object& object)
        {
            if (!object.shouldGetKilled())
            {
                if (canMove)
                {
                    moved = m_movementSystem.tryMoveYou(object) || moved;
                }

                object.update();
            }
            else
            {
                m_objectMng.addToDestroyQueue(object);
            }
        }
    );

    m_objectMng.updateDestroyQueue();

    bool rulesChanged = m_ruleSystem.update();
    if (rulesChanged)
    {
        buildYouObjects();
    }

    if (rulesChanged || moved)
    {
        checkWin();
    }
}

void Level::updateStateWin()
{
    checkReload();
}

void Level::updateStateDefeat()
{
}

void Level::update()
{
    switch (m_state)
    {
        case LevelState::IDLE:    updateStateIdle();    break;
        case LevelState::PLAYING: updateStatePlaying(); break;
        case LevelState::WIN:     updateStateWin();     break;
        case LevelState::DEFEAT:  updateStateDefeat();  break;
    }

    m_transition.update();
}

void Level::draw()
{
    m_objectMng.forEach(
        [](Object& object)
        {
            object.draw();
        }
    );

    m_border.draw();
}

void Level::checkWin()
{
    for (auto uid : m_youObjectsUID)
    {
        Object* object = m_objectMng.findObjectFromUID(uid);

        if (object == nullptr)
        {
            continue;
        }

        std::vector<Object*> others;
        GameUtils::getObjectsAt(m_objectMng, m_grid, object->getCell(), others);

        auto it = std::ranges::find_if(others, 
        [this](const Object* other)
        {
            return other != nullptr &&
                   m_ruleSystem.hasBehavior(other->getId(), BehaviorType::WIN);
        });
        
        if (it != others.end())
        {
            std::println("WIN !");

            m_state = LevelState::WIN;
            m_reloadRequested = true;

            return;
        }
    }
}