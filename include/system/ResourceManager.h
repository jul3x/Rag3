//
// Created by jprolejko on 25.06.19.
//

#ifndef RAG3_SYSTEM_RESOURCEMANAGER_H
#define RAG3_SYSTEM_RESOURCEMANAGER_H

#include <list>
#include <string>
#include <map>

#include <SFML/Graphics/Texture.hpp>

#include <objects/Obstacle.h>
#include <objects/Decoration.h>
#include <objects/Weapon.h>


class ResourceManager {

public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    static ResourceManager& getInstance();

    Weapon& getWeapon(const std::string &key);
    sf::Texture& getTexture(const std::string &key);
    std::tuple<std::list<Obstacle>, std::list<Decoration>> getMap(const std::string &key);

    void lazyLoadTexture(const std::string &key);

private:
    ResourceManager() = default;

    void loadWeapon(const std::string &key);
    void loadTexture(const std::string &key);
    std::tuple<std::list<Obstacle>, std::list<Decoration>> loadMap(const std::string &key);

    std::map<std::string, sf::Texture> textures_;
    std::map<std::string, Weapon> weapons_;
};


#endif //RAG3_SYSTEM_RESOURCEMANAGER_H
