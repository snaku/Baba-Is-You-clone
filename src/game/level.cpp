#include "game/level.hpp"
#include "game/levelLoader.hpp"
#include "game/config.hpp"
#include "game/utils.hpp"

#include "input/input.hpp"

#include "renderer/renderer.hpp"
#include "renderer/textureManager.hpp"

#include "time/time.hpp"

// std
#include <iostream>
#include <algorithm>
#include <fstream>

Level::Level(Renderer& renderer,
             TextureManager& textureMng,
             const Input& input,
             Fade& fade) 
    : m_renderer(renderer),
      m_textureMng(textureMng),
      m_input(input),
      m_objectMng(renderer, textureMng),
      m_transition(*this, fade),
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
            m_youObjectsUID.erase(std::remove(m_youObjectsUID.begin(), m_youObjectsUID.end(), object.getUID()),
                                  m_youObjectsUID.end());
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

    std::ostringstream ostream;
    ostream << "level_" << m_id << ".txt";

    LevelDefinition def = LevelLoader::read(ostream.str());
    if (!def.isValid)
    {
        m_state = LevelState::IDLE;
        return;
    }

    for (const auto& data : def.objects)
    {
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
    std::cout << "Reloading level" << std::endl;

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

        auto it = std::find_if(others.begin(), others.end(), 
        [this](const Object* other)
        {
            return other != nullptr &&
                   m_ruleSystem.hasBehavior(other->getId(), BehaviorType::WIN);
        });
        
        if (it != others.end())
        {
            std::cout << "WIN !" << std::endl;
            m_state = LevelState::WIN;
            m_reloadRequested = true;

            return;
        }
    }
}