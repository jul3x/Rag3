//
// Created by jul3x on 10.03.20.
//

#ifndef RAG3_COMMON_MAP_H
#define RAG3_COMMON_MAP_H

#include <list>

#include <R3E/utils/Geometry.h>
#include <R3E/ai/DataTypes.h>

#include <common/DecorationTile.h>
#include <common/ObstacleTile.h>
#include <common/Character.h>
#include <common/Collectible.h>


using namespace r3e;

class Map {
public:
    using Data = std::tuple<std::list<ObstacleTile>, std::list<DecorationTile>,
                            std::list<Character>, std::list<Collectible>>;
    using TileMap = std::pair<sf::Vector2f, std::vector<std::vector<float>>>;

    Map() = default;

    bool clearMap();

    bool loadMap(const std::string& name);

    const sf::Vector2f& getSize() const;

    const ai::MapBlockage& getMapBlockage() const;

    std::list<DecorationTile>& getDecorationsTiles();

    std::list<ObstacleTile>& getObstaclesTiles();

    std::list<Character>& getCharacters();

    std::list<Collectible>& getCollectibles();

    void spawnDecorationTile(const sf::Vector2f& pos, const std::string& id);

    void spawnObstacleTile(const sf::Vector2f& pos, const std::string& id);

    void spawnWeapon(const sf::Vector2f& pos, const std::string& id);

    void spawnCharacter(const sf::Vector2f& pos, const std::string& id);

    void removeTile(const sf::Vector2f& pos);

    void removeObject(const sf::Vector2f& pos);

    std::pair<sf::Vector2<size_t>, sf::Vector2f> getTileConstraints() const;

    bool update(float time_elapsed);

private:
    template<class T>
    bool checkCollisions(const sf::Vector2f& pos, std::list<T>& objs, bool erase = false)
    {
        for (auto it = objs.begin(); it != objs.end(); ++it)
        {
            if (utils::geo::isPointInRectangle(
                    pos, it->getPosition() - sf::Vector2f(DecorationTile::SIZE_X_ / 2.0f, DecorationTile::SIZE_Y_ / 2.0f),
                    {DecorationTile::SIZE_X_, DecorationTile::SIZE_Y_}))
            {
                if (erase)
                    objs.erase(it);

                return true;
            }
        }
        return false;
    }

    std::list<ObstacleTile> obstacles_tiles_;
    std::list<Character> characters_;
    std::list<Collectible> collectibles_;
    std::list<DecorationTile> decorations_tiles_;

    ai::MapBlockage blocked_;

    sf::Vector2f size_;

};

#endif // RAG3_COMMON_MAP_H