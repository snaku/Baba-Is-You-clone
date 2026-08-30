#pragma once

#include "game/grid.hpp"
#include "game/object.hpp"
#include "game/objectManager.hpp"
#include "game/levelTransition.hpp"
#include "game/levelFile.hpp"
#include "game/levelBorder.hpp"
#include "game/levelEditor.hpp"
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
    DEFEAT,
    EDITOR
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

    void load(uint32_t id);
    void update();
    void draw();

    void resize(uint32_t windowWidth, uint32_t windowHeight);

    bool reloadRequested() const { return m_reloadRequested; }
    void allowReload() { m_canReload = true; }

    using WinCallback = std::function<void(Level&)>;

    void setWinCallback(WinCallback callback) { m_winCallback = std::move(callback); }

    uint32_t getId() const { return m_id; }

private:
    void initFromDef(const LevelDefinition& def);

    void buildYouObjects();

    void clear();

    void checkReload();
    void reload();
    void clearReloadState();

    void updateStateIdle();
    void updateStatePlaying();
    void updateStateWin();
    void updateStateDefeat();
    void updateStateEditor();

    void resizeGrid();
    void reloadGrid();

    LevelSituation findSituation();
    void checkSituations();

    void checkUndo();

    Renderer& m_renderer;
    Grid m_grid;
    const Input& m_input;
    ObjectManager m_objectMng;
    LevelTransition m_transition;
    LevelBorder m_border;
    LevelEditor m_editor;
    MovementSystem m_movementSystem;
    RuleSystem m_ruleSystem;
    UndoSystem m_undoSystem;

    LevelState m_state = LevelState::IDLE;
    uint32_t m_id = 0;

    std::vector<std::size_t> m_youObjectsUID;

    float m_reloadTimer = 0.0f;
    bool m_reloadRequested = false;
    bool m_canReload = false;

    WinCallback m_winCallback;

    uint32_t m_currentGridWidth = 0;
    uint32_t m_currentGridHeight = 0;

    static constexpr float s_reloadDelay = 1.0f;
};