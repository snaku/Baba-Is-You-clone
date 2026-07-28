#include "game/objectManager.hpp"

// std
#include <algorithm>

ObjectManager::ObjectManager(Renderer& renderer, TextureManager& textureMng)
    : m_renderer(renderer),
      m_textureMng(textureMng)
{
}

Object& ObjectManager::addObject(ObjectId id, Cell cell)
{
    auto object = std::make_unique<Object>(m_renderer, m_textureMng, s_nextUID++, id, cell);

    if (m_addCallback != nullptr)
    {
        m_addCallback(*object);
    }

    m_objectsByUID.insert({object->getUID(), object.get()});
    m_objects.push_back(std::move(object));

    return *m_objects.back();
}

Object& ObjectManager::addObjectFromUID(std::size_t uid, ObjectId id, Cell cell)
{
    std::size_t savedUID = s_nextUID;
    s_nextUID = uid;

    Object& object = addObject(id, cell);

    s_nextUID = savedUID;

    return object;
}

void ObjectManager::removeObject(Object& object)
{
    if (m_removeCallback != nullptr)
    {
        m_removeCallback(object);
    }

    m_objectsByUID.erase(object.getUID());

    auto it = std::ranges::find_if(m_objects, 
        [&object](const std::unique_ptr<Object>& ptr)
        {
            return ptr.get() == &object;
        });

    if (it == m_objects.end())
    {
        return;
    }

    m_objects.erase(it);
}

Object* ObjectManager::findFromUID(std::size_t uid)
{
    auto it = m_objectsByUID.find(uid);

    if (it == m_objectsByUID.end() ||
        it->second == nullptr)
    {
        return nullptr;
    }

    return it->second;
}

std::vector<Object*> ObjectManager::findFromUIDs(const std::vector<std::size_t>& uids)
{
    std::vector<Object*> objects;
    objects.reserve(uids.size());

    for (auto uid : uids)
    {
        Object* object = findFromUID(uid);
        if (object == nullptr)
        {
            continue;
        }

        objects.push_back(object);
    }

    return objects;
}

void ObjectManager::updateDestroyQueue()
{
    if (m_destroyQueue.empty())
    {
        return;
    }

    for (auto uid : m_destroyQueue)
    {
        Object* object = findFromUID(uid);
        if (object == nullptr)
        {
            continue;
        }

        removeObject(*object);
    }

    m_destroyQueue.clear();
}

void ObjectManager::addToDestroyQueue(Object& object)
{
    m_destroyQueue.push_back(object.getUID());
}

void ObjectManager::clear()
{
    while (!m_objects.empty())
    {
        removeObject(*m_objects.back());
    }

    m_destroyQueue.clear();
}