//
// Created by jul3x on 03.11.19.
//

#ifndef RAG3_GAME_INCLUDE_CHARACTERS_PLAYER_H
#define RAG3_GAME_INCLUDE_CHARACTERS_PLAYER_H

#include <common/characters/Character.h>


class Player : public Character {
public:
    enum class SideStepDir {
        Left = -1,
        Right = 1
    };

    enum class Skills {
        Intelligence = 0,
        Heart = 1,
        Strength = 2,
        Agility = 3
    };

    explicit Player(const sf::Vector2f& position);

    void setDead();
    void setHealth(float life) override;
    void setRunning(bool run);

    bool isRunning() const;
    bool isAlive() const;
    void getShot(const Bullet& bullet, float factor) override;
    void getCut(const MeleeWeapon& weapon, float factor) override;
    float getMaxHealth() const override;
    float getSpeedFactor() const override;
    float getMaxTimeManipulation() const;
    float getTimeManipulationFuelSpeed() const;
    float getMaxRunningFuel() const;
    float getRunningFuelSpeed() const;
    int getSkillPoints() const;
    int getSkill(Skills skill) const;
    std::list<std::pair<Special, int>>& getBackpack();

    void addSkillPoints(int skill_points);
    bool addSkill(Skills skill);
    void addSpecialToBackpack(Special* special, const std::function<void(Functional*)>& register_func);
    void useItem(const std::string& name);
    void upgradeWeapon(const std::string& weapon, const std::string& special);
    void changePlayerTexture(const std::string& name);
    bool sideStep(Player::SideStepDir dir);
    bool update(float time_elapsed) override;

protected:
    inline void handleGlobalState(float time_elapsed) override;

private:
    bool is_running_;
    float side_stepping_freeze_time_;
    float running_fuel_;

    bool is_alive_;

    std::list<std::pair<Special, int>> backpack_;
    int skill_points_;
    std::map<Skills, int> skills_;

};

#endif //RAG3_GAME_INCLUDE_CHARACTERS_PLAYER_H
