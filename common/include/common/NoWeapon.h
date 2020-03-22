//
// Created by jul3x on 21.11.19.
//


#ifndef RAG3_GAME_WEAPONS_NOWEAPON_H
#define RAG3_GAME_WEAPONS_NOWEAPON_H

#include <common/AbstractWeapon.h>


class NoWeapon : public AbstractWeapon {
public:
    NoWeapon();

    sf::Vector2f use(float time_factor) override;

    float getState() const override;
};

#endif // RAG3_GAME_WEAPONS_NOWEAPON_H