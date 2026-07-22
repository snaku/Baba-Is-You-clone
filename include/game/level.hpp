#pragma once

#include "game/grid.hpp"
#include "game/object.hpp"
#include "game/rule.hpp"
#include "game/ruleParser.hpp"
#include "game/levelTransition.hpp"

// std
#include <vector>
#include <memory>
#include <optional>
#include <bitset>
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

    void addBehavior(ObjectId id, BehaviorType behavior);
    void clearBehaviors(ObjectId id);
    bool hasBehavior(ObjectId id, BehaviorType behavior);

    void addToTransformationQueue(ObjectId id, ObjectId newId);
    void applyObjectsTransformation();
    void revertObjectsTransformation();

    void buildYouObjects();

    void applyRules();
    bool updateRules();

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
    std::vector<std::unique_ptr<Object>> m_objects;
    std::unordered_map<std::size_t, Object*> m_objectsByUID;
    std::vector<std::size_t> m_youObjectsUID;
    Grid m_grid;
    RuleParser m_ruleParser;
    float m_moveTimer = 0.0f;
    std::vector<Rule> m_rules;
    bool m_rulesDirty = false;
    std::unordered_map<ObjectId, std::bitset<(std::size_t)BehaviorType::MAX>> m_behaviors;
    std::vector<std::size_t> m_destroyQueue;
    LevelState m_state = LevelState::IDLE;
    std::unordered_map<Object*, ObjectId> m_objectsWithTransformation;
    bool m_movedLastFrame = false;
    float m_reloadTimer = 0.0f;
    bool m_reloadRequested = false;
    bool m_canReload = false;
    int m_id = 0;
};