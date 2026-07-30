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
      m_undoSystem(m_objectMng),
      m_movementSystem(m_objectMng, m_grid, m_ruleSystem)
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

    m_undoSystem.snap();

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

    if (m_reloadTimer >= s_reloadDelay &&
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
        reload();
        clearReloadState();
    }
}

void Level::reload()
{
    std::println("Reloading level");

    m_ruleSystem.clear();
    m_grid.clearObjects();
    m_objectMng.clear();
    m_undoSystem.clear();

    load();
}

void Level::clearReloadState()
{
    m_reloadRequested = false;
    m_canReload = false;
    m_reloadTimer = 0.0f;
}

void Level::updateStateIdle()
{
}

void Level::updateStatePlaying()
{
    checkReload();
    checkUndo();

    bool canMove = m_movementSystem.updateMoveTimer();
    bool moved = false;

    m_objectMng.forEach(
        [&](Object& object)
        {
            if (!object.shouldGetKilled())
            {
                if (canMove)
                {
                    moved = m_movementSystem.tryMoveYou(object, m_input) || moved;
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

    if (moved)
    {
        m_undoSystem.snap();
    }

    bool rulesChanged = m_ruleSystem.update();
    if (rulesChanged)
    {
        buildYouObjects();
    }

    if (rulesChanged || moved)
    {
        checkSituations();
    }
}

void Level::updateStateWin()
{
    checkReload();
}

void Level::updateStateDefeat()
{
    checkReload();
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

LevelSituation Level::findSituation()
{
    static constexpr std::array<std::pair<BehaviorType, LevelSituation>, 2> s_bhvSituations =
    {
        std::pair{BehaviorType::DEFEAT, LevelSituation::DEFEAT}, // need to check defeat first
        std::pair{BehaviorType::WIN, LevelSituation::WIN}
    };

    for (auto uid : m_youObjectsUID)
    {
        Object* object = m_objectMng.findFromUID(uid);

        if (object == nullptr)
        {
            continue;
        }

        std::vector<Object*> others = m_objectMng.findFromUIDs(m_grid.getObjectsAt(object->getCell()));
        for (const auto& other : others)
        {
            if (other == nullptr)
            {
                continue;
            }

            for (const auto& [behavior, situation] : s_bhvSituations)
            {
                if (m_ruleSystem.hasBehavior(other->getId(), behavior))
                {
                    return situation;
                }
            }
        }
    }

    return LevelSituation::NONE;
}

void Level::checkSituations()
{
    switch (findSituation())
    {
        case LevelSituation::NONE: return;

        case LevelSituation::WIN:
            std::println("WIN !");
            m_state = LevelState::WIN;
            m_reloadRequested = true;
            m_transition.setColor(255, 255, 255);

            break;

        case LevelSituation::DEFEAT:
            std::println("DEFEAT...");
            m_state = LevelState::DEFEAT;
            m_reloadRequested = true;
            m_transition.setColor(0, 0, 0);
    }
}

void Level::checkUndo()
{
    if (!m_undoSystem.updateTimer() ||
        !m_input.isKeyDown(SDL_SCANCODE_Z))
    {
        return;
    }

    m_undoSystem.clearTimer();
    m_undoSystem.undo();

    m_ruleSystem.requestDirty();
}