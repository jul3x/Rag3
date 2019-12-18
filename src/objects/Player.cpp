//
// Created by jprolejko on 03.11.19.
//

#include <system/ResourceManager.h>

#include <objects/Player.h>
#include <objects/ShootingWeapon.h>
#include <objects/NoWeapon.h>


Player::Player(const sf::Vector2f &position,
               const sf::Vector2f &velocity) :
        Character(position, velocity, 1000),
        is_alive_(true) {
    weapons_in_backpack_.push_back(
        std::make_unique<ShootingWeapon>(ResourceManager::getInstance().getWeapon("m4")));
    weapons_in_backpack_.push_back(
        std::make_unique<ShootingWeapon>(ResourceManager::getInstance().getWeapon("desert_eagle")));
    weapons_in_backpack_.push_back(
        std::make_unique<ShootingWeapon>(ResourceManager::getInstance().getWeapon("shotgun")));
    weapons_in_backpack_.push_back(
        std::make_unique<ShootingWeapon>(ResourceManager::getInstance().getWeapon("rocket_launcher")));
    weapons_in_backpack_.push_back(
        std::make_unique<NoWeapon>());
    weapons_in_backpack_.push_back(
        std::make_unique<NoWeapon>());

    current_weapon_ = weapons_in_backpack_.begin();
}

void Player::setDead() {
    is_alive_ = false;
}

bool Player::isAlive() const {
    return is_alive_;
}
