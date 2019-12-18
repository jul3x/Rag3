//
// Created by jul3x on 01.10.19.
//


#ifndef RAG3_GAME_MISC_CAMERA_H
#define RAG3_GAME_MISC_CAMERA_H

#include <engine/graphics/AbstractCamera.h>


class Camera : public AbstractCamera {
public:
    Camera();

    void setShaking();

    void update(const sf::Vector2f& position, float time_elapsed) override;

private:
    enum class State {
        NORMAL,
        SHOOTING
    };

    static constexpr float SHAKING_TIME_ = 0.3f;

    Camera::State state_;
};

#endif // RAG3_GAME_MISC_CAMERA_H