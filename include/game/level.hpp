#pragma once

#include "game/grid.hpp"
#include "game/object.hpp"
#include "game/levelTransition.hpp"
#include "game/ruleSystem.hpp"

// std
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>

class Renderer;
class TextureManager;
class Input;
class Fade;

enum class LevelState
{
    IDLE,
    PLAYING,
    WIN,
    DEFEAT
};

class Level
{
public:
    Level(Renderer& renderer, TextureManager& textureMng, const Input& input, Fade& fade);
    ~Level() noexcept;

    void load();
    void update();
    void draw();

    Object& addObject(ObjectId id, Cell cell);
    void removeObject(std::size_t uid);
    void getObjectsAt(Cell cell, std::vector<Object*>& out);
    Object* findObjectFromUID(std::size_t objectUID);

    const std::vector<std::unique_ptr<Object>>& getObjects() const { return m_objects; }

    bool reloadRequested() const { return m_reloadRequested; }
    void allowReload() { m_canReload = true; }

private:
    bool updateMoveTimer();
    bool tryMoveYou(Object& object);

    bool handleSinkInteraction(Object& object, Object& other);
    bool handlePushInteraction(Object& object, Direction dir);
    bool handleObjectInteractionsAt(Object& object, Cell cell, Direction dir);
    bool tryMove(Object& object, Direction dir);

    void buildYouObjects();

    void checkReload();
    void reload();

    void updateStateIdle();
    void updateStatePlaying();
    void updateStateWin();
    void updateStateDefeat();

    void updateDestroyQueue();
    void addToDestroyQueue(Object& object);

    void checkWin();

    Renderer& m_renderer;
    TextureManager& m_textureMng;
    const Input& m_input;
    LevelTransition m_transition;

    RuleSystem m_ruleSystem;
    std::vector<std::unique_ptr<Object>> m_objects;
    std::unordered_map<std::size_t, Object*> m_objectsByUID;
    std::vector<std::size_t> m_youObjectsUID;
    Grid m_grid;
    float m_moveTimer = 0.0f;
    std::vector<std::size_t> m_destroyQueue;
    LevelState m_state = LevelState::IDLE;
    bool m_movedLastFrame = false;
    float m_reloadTimer = 0.0f;
    bool m_reloadRequested = false;
    bool m_canReload = false;
    uint32_t m_id = 0;
};