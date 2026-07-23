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

void ObjectManager::removeObject(Object& object)
{
    if (m_removeCallback != nullptr)
    {
        m_removeCallback(object);
    }

    m_objectsByUID.erase(object.getUID());

    auto it = std::find_if(m_objects.begin(), m_objects.end(), 
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

Object* ObjectManager::findObjectFromUID(std::size_t objectUID)
{
    auto it = m_objectsByUID.find(objectUID);

    if (it == m_objectsByUID.end() ||
        it->second == nullptr)
    {
        return nullptr;
    }

    return it->second;
}

void ObjectManager::updateDestroyQueue()
{
    if (m_destroyQueue.empty())
    {
        return;
    }

    for (auto uid : m_destroyQueue)
    {
        Object* object = findObjectFromUID(uid);
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
    m_destroyQueue.clear();
    m_objectsByUID.clear();
    m_objects.clear();
}