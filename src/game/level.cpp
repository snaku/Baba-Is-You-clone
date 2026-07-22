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

Level::Level(Renderer& renderer,
             TextureManager& textureMng,
             const Input& input,
             Fade& fade) 
    : m_renderer(renderer),
      m_textureMng(textureMng),
      m_input(input),
      m_transition(*this, fade),
      m_grid(*this, GameConfig::gridWidth, GameConfig::gridHeight),
      m_ruleParser(*this)
{
}
Level::~Level() noexcept = default;

void Level::load()
{
    LevelDefinition def = LevelLoader::read("level_1.txt");
    if (!def.isValid)
    {
        m_state = LevelState::IDLE;
        return;
    }

    for (const auto& data : def.objects)
    {
        addObject(data.id, data.cell);
    }

    m_rulesDirty = true;
    m_state = LevelState::PLAYING;
}

bool Level::updateMoveTimer()
{
    m_moveTimer += Time::deltaTime();

    if (!m_movedLastFrame)
    {
        m_moveTimer = 0.10f;
    }

    if (m_moveTimer >= 0.10f)
    {
        m_moveTimer -= 0.10f;
        return true;
    }

    return false;
}

bool Level::tryMoveYou(Object& object)
{
    if (!hasBehavior(object.getId(), BehaviorType::YOU))
    {
        return false;
    }

    Direction dir = Direction::NONE;

    if (m_input.isKeyDown(SDL_SCANCODE_W))
    {
        dir = Direction::UP;
    }
    else if (m_input.isKeyDown(SDL_SCANCODE_S))
    {
        dir = Direction::DOWN;
    }
    else if (m_input.isKeyDown(SDL_SCANCODE_A))
    {
        dir = Direction::LEFT;
    }
    else if (m_input.isKeyDown(SDL_SCANCODE_D))
    {
        dir = Direction::RIGHT;
    }

    if (dir == Direction::NONE)
    {
        m_movedLastFrame = false;
        return false;
    }

    m_movedLastFrame = true;

    object.faceDir(dir);

    return tryMove(object, dir);
}

bool Level::handleSinkInteraction(Object& object, Object& other)
{
    if (!hasBehavior(object.getId(), BehaviorType::SINK) &&
        !hasBehavior(other.getId(), BehaviorType::SINK))
    {
        return false;
    }

    object.requestKill();
    other.requestKill();

    return true;
}

bool Level::handlePushInteraction(Object& object, Direction dir)
{
    if (!hasBehavior(object.getId(), BehaviorType::PUSH) &&
        !object.isText())
    {
        return true; // objects without PUSH behavior can be passed through
    }

    Cell nextCell = GameUtils::getNextCellFromDir(object.getCell(), dir);
    if (!nextCell.isValidPos() ||
        nextCell == object.getCell())
    {
        return false;
    }

    if (!tryMove(object, dir))
    {
        return false;
    }

    if (object.isText())
    {
        m_rulesDirty = true;
    }

    return true;
}

bool Level::handleObjectInteractionsAt(Object& object, Cell cell, Direction dir)
{
    std::vector<Object*> others;
    getObjectsAt(cell, others);

    for (auto& other : others)
    {
        if (other == nullptr)
        {
            continue;
        }

        if (handleSinkInteraction(object, *other) ||
            hasBehavior(other->getId(), BehaviorType::STOP) ||
            !handlePushInteraction(*other, dir))
        {
            return false;
        }
    }

    return true;
}

bool Level::tryMove(Object& object, Direction dir)
{
    Cell next = GameUtils::getNextCellFromDir(object.getCell(), dir);
    if (!next.isValidPos() ||
        next == object.getCell())
    {
        return false;
    }

    if (!handleObjectInteractionsAt(object, next, dir))
    {
        return false;
    }

    if (object.shouldGetKilled())
    {
        return true;
    }

    m_grid.removeObjectAt(object.getUID(), object.getCell());
    object.move(dir);
    m_grid.addObjectAt(object.getUID(), object.getCell());

    return true;
}

void Level::addBehavior(ObjectId id, BehaviorType behavior)
{
    m_behaviors[id].set((std::size_t)behavior);
}

void Level::clearBehaviors(ObjectId id)
{
    m_behaviors[id].reset();
}

bool Level::hasBehavior(ObjectId id, BehaviorType behavior)
{
    auto it = m_behaviors.find(id);

    if (it == m_behaviors.end())
    {
        return false;
    }

    return it->second.test((std::size_t)behavior);
}

void Level::addToTransformationQueue(ObjectId id, ObjectId newId)
{
    if (newId == ObjectId::NONE)
    {
        return;
    }

    for (auto& object : m_objects)
    {
        if (object->getId() != id)
        {
            continue;
        }

        m_objectsWithTransformation.insert({object.get(), newId});
    }
}

void Level::applyObjectsTransformation()
{
    for (auto& [object, newId] : m_objectsWithTransformation)
    {
        if (object == nullptr)
        {
            continue;
        }

        object->changeTo(newId);
    }
}

void Level::revertObjectsTransformation()
{
    for (auto& [object, _] : m_objectsWithTransformation)
    {
        if (object == nullptr)
        {
            continue;
        }

        object->changeBack();
    }

    m_objectsWithTransformation.clear();
}

void Level::buildYouObjects()
{
    m_youObjectsUID.clear();

    for (const auto& object : m_objects)
    {
        if (!hasBehavior(object->getId(), BehaviorType::YOU))
        {
            continue;
        }

        m_youObjectsUID.push_back(object->getUID());
    }
}

void Level::applyRules()
{
    m_behaviors.clear();
    revertObjectsTransformation();

    for (const auto& rule : m_rules)
    {
        if (!rule.negate)
        {
            if (std::holds_alternative<BehaviorType>(rule.predicate))
            {
                addBehavior(rule.subject, std::get<BehaviorType>(rule.predicate));
            }
            else
            {
                addToTransformationQueue(rule.subject, std::get<ObjectId>(rule.predicate));
            }
        }
        else
        {
            std::cout << "Negated !" << std::endl;
        }
    }

    applyObjectsTransformation();
}

bool Level::updateRules()
{
    if (!m_rulesDirty)
    {
        return false;
    }

    std::cout << "Updating rules..." << std::endl;

    m_rules = m_ruleParser.parse();
    applyRules();

    buildYouObjects();

    m_rulesDirty = false;

    return true;
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

    m_moveTimer = 0.0f;
    m_rules.clear();
    m_behaviors.clear();
    m_destroyQueue.clear();
    m_grid.clearObjects();
    m_objectsWithTransformation.clear();
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

    bool canMove = updateMoveTimer();
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
            moved = tryMoveYou(*object) || moved;
        }

        object->update();
    }

    updateDestroyQueue();

    if (updateRules() || moved)
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
                   hasBehavior(other->getId(), BehaviorType::WIN);
        });
        
        if (it != others.end())
        {
            std::cout << "WIN !" << std::endl;
            m_state = LevelState::WIN;
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
    m_objectsWithTransformation.erase(object);
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