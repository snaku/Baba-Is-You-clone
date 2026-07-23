#pragma once

#include "game/grid.hpp"
#include "game/object.hpp"
#include "game/objectManager.hpp"
#include "game/levelTransition.hpp"
#include "game/ruleSystem.hpp"
#include "game/movementSystem.hpp"

// std
#include <vector>
#include <optional>

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

    bool reloadRequested() const { return m_reloadRequested; }
    void allowReload() { m_canReload = true; }

private:
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
    Grid m_grid;
    const Input& m_input;
    ObjectManager m_objectMng;
    LevelTransition m_transition;
    MovementSystem m_movementSystem;
    RuleSystem m_ruleSystem;
    LevelState m_state = LevelState::IDLE;
    uint32_t m_id = 0;
    std::vector<std::size_t> m_youObjectsUID;
    std::vector<std::size_t> m_destroyQueue;
    float m_reloadTimer = 0.0f;
    bool m_reloadRequested = false;
    bool m_canReload = false;
};