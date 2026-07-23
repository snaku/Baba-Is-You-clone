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
      m_transition(*this, fade),
      m_grid(*this, GameConfig::gridWidth, GameConfig::gridHeight),
      m_ruleSystem(*this),
      m_movementSystem(*this, m_grid, m_ruleSystem, m_input)
{
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
        addObject(data.id, data.cell);
    }

    m_ruleSystem.requestDirty();
    m_state = LevelState::PLAYING;
}

void Level::buildYouObjects()
{
    m_youObjectsUID.clear();

    for (const auto& object : m_objects)
    {
        if (!m_ruleSystem.hasBehavior(object->getId(), BehaviorType::YOU))
        {
            continue;
        }

        m_youObjectsUID.push_back(object->getUID());
    }
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
    m_destroyQueue.clear();
    m_grid.clearObjects();
    m_objectsByUID.clear();
    m_objects.clear();

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
    for (auto& object : m_objects)
    {
        if (object->shouldGetKilled())
        {
            addToDestroyQueue(*object);
            continue;
        }

        if (canMove)
        {
            moved = m_movementSystem.tryMoveYou(*object) || moved;
        }

        object->update();
    }

    updateDestroyQueue();

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
    for (auto& object : m_objects)
    {
        object->draw();
    }
}

void Level::updateDestroyQueue()
{
    if (m_destroyQueue.empty())
    {
        return;
    }

    for (auto uid : m_destroyQueue)
    {
        removeObject(uid);
    }

    m_destroyQueue.clear();
}

void Level::addToDestroyQueue(Object& object)
{
    m_destroyQueue.push_back(object.getUID());
}

void Level::checkWin()
{
    for (auto uid : m_youObjectsUID)
    {
        Object* object = findObjectFromUID(uid);

        if (object == nullptr)
        {
            continue;
        }

        std::vector<Object*> others;
        getObjectsAt(object->getCell(), others);

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

Object& Level::addObject(ObjectId id, Cell cell)
{
    auto object = std::make_unique<Object>(m_renderer, m_textureMng, id, cell);

    m_grid.addObjectAt(object->getUID(), object->getCell());

    m_objectsByUID.insert({object->getUID(), object.get()});
    m_objects.push_back(std::move(object));

    return *m_objects.back();
}

void Level::removeObject(std::size_t uid)
{
    Object* object = findObjectFromUID(uid);

    if (object == nullptr)
    {
        return;
    }

    m_grid.removeObjectAt(uid, object->getCell());
    m_ruleSystem.eraseObjectWithTransformation(uid);
    m_objectsByUID.erase(uid);
    m_youObjectsUID.erase(std::remove(m_youObjectsUID.begin(), m_youObjectsUID.end(), uid),
                          m_youObjectsUID.end());

    auto it = std::find_if(m_objects.begin(), m_objects.end(), 
    [object](const std::unique_ptr<Object>& ptr)
    {
        return ptr.get() == object;
    });

    if (it == m_objects.end())
    {
        return;
    }

    m_objects.erase(it);
}

void Level::getObjectsAt(Cell cell, std::vector<Object*>& out)
{
    const std::vector<std::size_t>& objectsUID = m_grid.getObjectsAt(cell);

    out.clear();
    out.reserve(objectsUID.size());

    for (auto uid : objectsUID)
    {
        Object* object = findObjectFromUID(uid);
        if (object == nullptr)
        {
            continue;
        }

        out.push_back(object);
    }
}

Object* Level::findObjectFromUID(std::size_t objectUID)
{
    auto it = m_objectsByUID.find(objectUID);

    if (it == m_objectsByUID.end() ||
        it->second == nullptr)
    {
        return nullptr;
    }

    return it->second;
}