//
// Created by jul3x on 30.09.19.
//

#include <iostream>

#include <engine/utils/Geometry.h>
#include <engine/utils/Numeric.h>
#include <game/misc/ResourceManager.h>

#include <game/characters/Character.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>


Character::Character(const sf::Vector2f& position,
                     const sf::Vector2f& velocity,
                     int max_life) :
        DynamicObject(position,
                      velocity,
                      {SIZE_X_, SIZE_Y_},
                      Collision::Circle((SIZE_X_ - 5.0f) / 2.0f),
                      &ResourceManager::getInstance().getTexture("player"),
                      sf::Color(CFG.getInt("trail_color")),
                      CFG.getFloat("player_max_acceleration")),
        max_life_(max_life),
        Shootable(max_life) {}

bool Character::shot()
{
    auto new_velocity = (*current_weapon_)->use();

    if (!utils::num::isNearlyEqual(new_velocity, {0.0f, 0.0f}))
    {
        this->setForcedVelocity(new_velocity);
        return true;
    }

    return false;
}

void Character::getShot(const Bullet& bullet)
{
    //Engine::spawnBloodAnimation();
    this->setForcedVelocity(this->getVelocity() +
                            utils::geo::getNormalized(bullet.getVelocity()) * static_cast<float>(bullet.getDeadlyFactor()) *
                            CFG.getFloat("get_shot_factor"));

    life_ -= bullet.getDeadlyFactor();
    life_ = life_ < 0 ? 0 : life_;
}

int Character::getCurrentWeapon() const
{
    return static_cast<int>(std::distance<std::vector<std::unique_ptr<AbstractWeapon>>::const_iterator>(
            this->weapons_in_backpack_.begin(), this->current_weapon_));
}

const std::vector<std::unique_ptr<AbstractWeapon>>& Character::getWeapons() const
{
    return this->weapons_in_backpack_;
}

int Character::getHealth() const
{
    return this->life_;
}

int Character::getMaxHealth() const
{
    return this->max_life_;
}

void Character::switchWeapon(int relative_position_backpack)
{
    current_weapon_ = current_weapon_ + relative_position_backpack;

    if (current_weapon_ >= weapons_in_backpack_.end())
    {
        current_weapon_ = weapons_in_backpack_.begin();
    }

    if (current_weapon_ < weapons_in_backpack_.begin())
    {
        current_weapon_ = weapons_in_backpack_.end() - 1;
    }
}

bool Character::update(float time_elapsed)
{
    DynamicObject::update(time_elapsed);

    return life_ > 0;
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape_, states);
    target.draw(**current_weapon_, states);

    static sf::Text text("", ResourceManager::getInstance().getFont(),
                  24);

    text.setFillColor(sf::Color::Red);
    text.setPosition(this->getPosition());
    int start_x = std::round(this->getPosition().x / Obstacle::SIZE_X_);
    int start_y = std::round(this->getPosition().y / Obstacle::SIZE_Y_);

    text.setString(std::to_string(start_x) + ", " + std::to_string(start_y) + ":\n" +
                   std::to_string(this->getPosition().x) + ", " + std::to_string(this->getPosition().y));

    target.draw(text, states);

    static sf::VertexArray path(sf::LineStrip);
    path.clear();

    for (const auto &v : path_)
    {
        path.append(sf::Vertex{{v.cord.first *Obstacle::SIZE_X_, v.cord.second * Obstacle::SIZE_Y_}, sf::Color::Blue});
    }

    target.draw(path, states);
}

void Character::setPosition(const sf::Vector2f& pos)
{
    AbstractDrawableObject::setPosition(pos);
    (*current_weapon_)->setPosition(pos);
}

void Character::setRotation(float theta)
{
    AbstractDrawableObject::setRotation(theta);
    (*current_weapon_)->setRotation(theta);
}

void Character::setPosition(float x, float y)
{
    AbstractDrawableObject::setPosition(x, y);
    (*current_weapon_)->setPosition(x, y);
}


void Character::setPositionX(float x)
{
    AbstractDrawableObject::setPositionX(x);
    (*current_weapon_)->setPositionX(x);
}

void Character::setPositionY(float y)
{
    AbstractDrawableObject::setPositionY(y);
    (*current_weapon_)->setPositionY(y);
}