//
// Created by jul3x on 31.10.19.
//


#ifndef RAG3_GAME_CHARACTERS_ENEMY_H
#define RAG3_GAME_CHARACTERS_ENEMY_H

#include <engine/ai/AbstractAgent.h>

#include <game/characters/Character.h>


class Enemy : public Character, public ai::AbstractAgent {
public:
    enum class LifeState {
        High,
        Low,
        Critical
    };

    enum class AmmoState {
        High,
        Low,
        Zero
    };

    enum class VisibilityState {
        Close,
        Far,
        TooFar,
        OutOfRange
    };

    enum class ActionState {
        StandBy,
        Follow,
        DestroyWall,
        Shot,
        ShotAndRun,
        Run
    };

    Enemy(const sf::Vector2f& position,
          const sf::Vector2f& velocity);

    bool update(float time_elapsed) override;

    const sf::Vector2f& getStartPosition() const override;

private:
    inline void handleLifeState();
    inline void handleAmmoState();
    inline void handleVisibilityState();
    inline void handleActionState();

    static constexpr float MAX_DISTANCE_ = 1500.0f;

    LifeState life_state_;
    AmmoState ammo_state_;
    VisibilityState visibility_state_;
    ActionState action_state_;

};

#endif // RAG3_GAME_CHARACTERS_ENEMY_H
