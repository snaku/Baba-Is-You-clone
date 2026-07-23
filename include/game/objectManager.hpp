#pragma once

#include "game/object.hpp"

// std
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

class Renderer;
class TextureManager;

class ObjectManager
{
public:
    ObjectManager(Renderer& renderer, TextureManager& textureMng);
    
    template<typename Fn>
    void forEach(Fn&& fn)
    {
        for (auto& object : m_objects)
        {
            fn(*object);
        }
    }

    Object& addObject(ObjectId id, Cell cell);
    void removeObject(std::size_t uid);
    Object* findObjectFromUID(std::size_t objectUID);

    void clear();

    using Callback = std::function<void(Object&)>;

    void setAddCallback(Callback addCallback) { m_addCallback = addCallback; }
    void setRemoveCallback(Callback removeCallback) { m_removeCallback = removeCallback; }

    const std::vector<std::unique_ptr<Object>>& getObjects() const { return m_objects; }

private:
    Renderer& m_renderer;
    TextureManager& m_textureMng;

    std::vector<std::unique_ptr<Object>> m_objects;
    std::unordered_map<std::size_t, Object*> m_objectsByUID;

    Callback m_addCallback = nullptr;
    Callback m_removeCallback = nullptr;
};