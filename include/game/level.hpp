#pragma once

#include "game/grid.hpp"
#include "game/object.hpp"
#include "game/objectManager.hpp"
#include "game/levelTransition.hpp"
#include "game/levelLoader.hpp"
#include "game/levelBorder.hpp"
#include "game/ruleSystem.hpp"
#include "game/movementSystem.hpp"
#include "game/undoSystem.hpp"

// std
#include <vector>

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

enum class LevelSituation
{
    NONE,

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

    void resize(uint32_t windowWidth, uint32_t windowHeight);

    bool reloadRequested() const { return m_reloadRequested; }
    void allowReload() { m_canReload = true; }

private:
    void initFromDef(const LevelDefinition& def);

    void buildYouObjects();

    void checkReload();
    void reload();
    void clearReloadState();

    void updateStateIdle();
    void updateStatePlaying();
    void updateStateWin();
    void updateStateDefeat();

    LevelSituation findSituation();
    void checkSituations();

    void checkUndo();

    Renderer& m_renderer;
    Grid m_grid;
    const Input& m_input;
    ObjectManager m_objectMng;
    LevelTransition m_transition;
    LevelBorder m_border;
    MovementSystem m_movementSystem;
    RuleSystem m_ruleSystem;
    UndoSystem m_undoSystem;

    LevelState m_state = LevelState::IDLE;
    uint32_t m_id = 0;

    std::vector<std::size_t> m_youObjectsUID;

    float m_reloadTimer = 0.0f;
    bool m_reloadRequested = false;
    bool m_canReload = false;

    static constexpr float s_reloadDelay = 1.0f;
};