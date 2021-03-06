//
// Created by jul3x on 08.05.20.
//


#ifndef RAG3_COMMON_INCLUDE_COMMON_PLACEDWEAPON_H
#define RAG3_COMMON_INCLUDE_COMMON_PLACEDWEAPON_H

#include <cmath>
#include <string>

#include <R3E/objects/Unique.h>

#include <common/weapons/ShootingWeapon.h>


class PlacedWeapon : public ShootingWeapon, public Unique {
public:
    PlacedWeapon(const sf::Vector2f& position, float direction, const std::string& id, int u_id = -1);

    PlacedWeapon(const sf::Vector2f& position, float direction, const std::string& id, const std::string& usage,
                 float data, int u_id = -1);

    [[nodiscard]] float getData() const;
    [[nodiscard]] const std::string& getUsageStr() const;
    [[nodiscard]] bool getActive() const;

    void setUsageStr(const std::string& str);
    void setData(float data);
    void setActive(bool active);

    void update(float time_elapsed) override;

private:
    void constUse(float time_elapsed);
    void singleUse(float time_elapsed);
    void interruptedUse(float time_elapsed);

    std::function<void(float)> usage_func_;
    float time_elapsed_;
    float data_;
    bool active_;
    std::string usage_str_;

};


#endif //RAG3_COMMON_INCLUDE_COMMON_PLACEDWEAPON_H