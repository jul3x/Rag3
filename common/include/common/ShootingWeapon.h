//
// Created by jul3x on 16.10.19.
//


#ifndef RAG3_COMMON_SHOOTINGWEAPON_H
#define RAG3_COMMON_SHOOTINGWEAPON_H

#include <cmath>
#include <string>

#include <common/AbstractWeapon.h>


class ShootingWeapon : public AbstractWeapon {
public:
    ShootingWeapon(const std::string& id);

    sf::Vector2f use() override;

    int getAmmunition() const;

    float getState() const override;

    void setState(float state) override;

private:
    static constexpr float BULLET_STARTING_OFFSET_ = 20.0f;
    // weapon parameters
    std::string bullet_type_;
    int bullet_quantity_;
    float bullet_angular_diff_;
    float bullet_timeout_;
    float recoil_;
    int ammunition_, max_ammunition_;

};


#endif // RAG3_COMMON_SHOOTINGWEAPON_H