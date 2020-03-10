//
// Created by jul3x on 07.02.2020.
//

#ifndef RAG3_ENGINE_AI_DATATYPES_H
#define RAG3_ENGINE_AI_DATATYPES_H


#include <functional>
#include <vector>
#include <list>
#include <chrono>

#include <SFML/System/Vector2.hpp>


namespace r3e {
    namespace ai {
        using NeighboursVec = std::vector<sf::Vector2<size_t>>;
        using Grid = std::vector<std::vector<float>>;
        using NeighbourFunction = std::function<NeighboursVec(const Grid& grid, const sf::Vector2<size_t>&)>;

        using Path = std::list<std::pair<sf::Vector2f, float>>;
        using Goal = sf::Vector2f;
        using Timestamp = std::chrono::system_clock::time_point;

        struct MapBlockage {
            Grid blockage_;
            float scale_x_, scale_y_;
        };
    } // namespace ai
} // namespace r3e

#endif //RAG3_ENGINE_AI_DATATYPES_H
