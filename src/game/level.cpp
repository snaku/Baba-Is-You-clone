#include "game/level.hpp"
#include "game/config.hpp"
#include "game/utils.hpp"

#include "input/input.hpp"

#include "renderer/renderer.hpp"
#include "renderer/textureManager.hpp"

#include "time/time.hpp"

// std
#include <iostream>
#include <algorithm>

Level::Level(Renderer& renderer, TextureManager& textureMng, const Input& input) 
    : m_renderer(renderer),
      m_textureMng(textureMng),
      m_input(input),
      m_grid(*this, GameConfig::gridWidth, GameConfig::gridHeight),
      m_ruleParser(*this)
{
}
Level::~Level() noexcept = default;


void Level::load()
{
    // OBJECT BABA + BABA IS YOU RULE
    addObject(ObjectId::BABA, Cell{0, 0});

    Object& textBaba = addObject(ObjectId::TEXT_BABA, Cell{8, 5});
    textBaba.setColor(217, 57, 106);

    addObject(ObjectId::TEXT_IS, Cell{9, 5});

    Object& textYou = addObject(ObjectId::TEXT_YOU, Cell{10, 5});
    textYou.setColor(217, 57, 106);

    // OBJECT WATER + WATER IS SINK AND NOT YOU RULE
    addObject(ObjectId::WATER, Cell{23, 5});

    Object& textWater = addObject(ObjectId::TEXT_WATER, Cell{14, 5});
    textWater.setColor(95, 157, 209);

    addObject(ObjectId::TEXT_IS, Cell{15, 5});

    Object& textSink = addObject(ObjectId::TEXT_SINK, Cell{16, 5});
    textSink.setColor(95, 157, 209);

    addObject(ObjectId::TEXT_AND, Cell{17, 5});

    Object& textNot = addObject(ObjectId::TEXT_NOT, Cell{18, 5});
    textNot.setColor(229, 83, 59);

    Object& textNot2 = addObject(ObjectId::TEXT_NOT, Cell{19, 5});
    textNot2.setColor(229, 83, 59);

    Object& textYou2 = addObject(ObjectId::TEXT_YOU, Cell{20, 5});
    textYou2.setColor(217, 57, 106);

    // OBJECT ROCK + ROCK IS PUSH RULE
    Object& rock = addObject(ObjectId::ROCK, Cell{15, 15});
    rock.setColor(194, 158, 70);

    Object& textRock = addObject(ObjectId::TEXT_ROCK, Cell{10, 8});
    textRock.setColor(144, 103, 62);

    addObject(ObjectId::TEXT_IS, Cell{10, 9});

    Object& textFlag2 = addObject(ObjectId::TEXT_FLAG, Cell{11, 10});
    textFlag2.setColor(237, 226, 133);

    addObject(ObjectId::TEXT_AND, Cell{10, 11});

    Object& textPush = addObject(ObjectId::TEXT_PUSH, Cell{10, 12});
    textPush.setColor(144, 103, 62);

    // OBJECT WALL + WALL IS STOP RULE
    Object& wall = addObject(ObjectId::WALL, Cell{15, 16});
    wall.setColor(41, 49, 65);

    Object& wall2 = addObject(ObjectId::WALL, Cell{16, 16});
    wall2.setColor(41, 49, 65);

    Object& wall3 = addObject(ObjectId::WALL, Cell{17, 16});
    wall3.setColor(41, 49, 65);

    Object& textWall = addObject(ObjectId::TEXT_WALL, Cell{13, 8});
    textWall.setColor(115, 115, 115);

    addObject(ObjectId::TEXT_IS, Cell{13, 9});

    Object& textStop = addObject(ObjectId::TEXT_STOP, Cell{13, 10});
    textStop.setColor(27, 92, 28);

    // OBJECT FLAG + FLAG IS WIN RULE
    Object& flag = addObject(ObjectId::FLAG, Cell{1, 0});
    flag.setColor(237, 226, 133);

    Object& textFlag = addObject(ObjectId::TEXT_FLAG, Cell{13, 1});
    textFlag.setColor(237, 226, 133);

    addObject(ObjectId::TEXT_IS, Cell{13, 2});

    Object& textWin = addObject(ObjectId::TEXT_WIN, Cell{13, 3});
    textWin.setColor(237, 226, 133);

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

bool Level::tryMove(Object& object, Direction dir)
{
    Cell next = GameUtils::getNextCellFromDir(object.getCell(), dir);
    if (!next.isValidPos() ||
        next == object.getCell())
    {
        return false;
    }

    std::vector<Object*> others;
    getObjectsAt(next, others);
    for (auto& other : others)
    {
        if (other == nullptr)
        {
            continue;
        }

        if (hasBehavior(object.getId(), BehaviorType::SINK) ||
            hasBehavior(other->getId(), BehaviorType::SINK))
        {
            object.requestKill();
            other->requestKill();
            return true;
        }

        if (hasBehavior(other->getId(), BehaviorType::STOP))
        {
            return false;
        }

        if (hasBehavior(other->getId(), BehaviorType::PUSH) ||
            other->isText())
        {
            Cell otherNext = GameUtils::getNextCellFromDir(other->getCell(), dir);
            if (!otherNext.isValidPos() ||
                otherNext == other->getCell())
            {
                return false;
            }

            if (!tryMove(*other, dir))
            {
                return false;
            }

            if (other->isText())
            {
                m_rulesDirty = true;
            }
        }
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
    m_rulesDirty = false;

    return true;
}

void Level::checkReload()
{
    m_reloadTimer += Time::deltaTime();

    if (m_reloadTimer < 1.0f ||
        !m_input.isKeyDown(SDL_SCANCODE_R))
    {
        return;
    }

    m_reloadTimer = 0.0f;

    reload();
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
    for (const auto& object : m_objects)
    {
        if (!hasBehavior(object->getId(), BehaviorType::YOU))
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