//
// Created by jul3x on 19.12.2019.
//

#ifndef RAG3_COMMON_INCLUDE_COMMON_SHOOTABLE_H
#define RAG3_COMMON_INCLUDE_COMMON_SHOOTABLE_H

#include <common/weapons/Bullet.h>


class Shootable {
public:
    Shootable() = delete;

    explicit Shootable(float life) : life_(life)
    {
    }

    virtual void getShot(const Bullet& bullet, float factor = 1.0f)
    {
        life_ -= bullet.getDeadlyFactor() * factor;
        life_ = life_ < 0 ? 0 : life_;
    }

    [[nodiscard]] virtual float getHealth() const
    {
        return life_;
    }

    virtual void setHealth(float life)
    {
        life_ = life;
    }

protected:
    float life_;
};


#endif //RAG3_COMMON_INCLUDE_COMMON_SHOOTABLE_H
