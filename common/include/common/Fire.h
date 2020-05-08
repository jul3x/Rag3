//
// Created by jul3x on 01.10.19.
//


#ifndef RAG3_COMMON_INCLUDE_COMMON_FIRE_H
#define RAG3_COMMON_INCLUDE_COMMON_FIRE_H

#include <deque>

#include <R3E/objects/AbstractPhysicalObject.h>


using namespace r3e;

class Fire : public HoveringObject {
public:
    Fire(const sf::Vector2f& position,
         float direction);

    bool update(float time_elapsed) override;

    void setDead();

private:
    sf::Color color_;
    float alpha_;

    float offset_;
    float direction_;
    float difference_;
    float r_;
    float life_;

};

#endif //RAG3_COMMON_INCLUDE_COMMON_FIRE_H