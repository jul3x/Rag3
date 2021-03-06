//
// Created by jul3x on 04.02.2020.
//

#ifndef RAG3_ENGINE_INCLUDE_R3E_AI_ABSTRACTAGENT_H
#define RAG3_ENGINE_INCLUDE_R3E_AI_ABSTRACTAGENT_H

#include <chrono>

#include <SFML/System/Vector2.hpp>

#include <R3E/ai/DataTypes.h>


namespace r3e::ai {

    class AgentsManager;

    class AbstractAgent {
    public:
        explicit AbstractAgent();

        virtual ~AbstractAgent();

        void registerAgentsManager(AgentsManager* manager);

        void setCurrentGoal(const ai::Goal& goal);
        void setNoGoal();

        [[nodiscard]] const sf::Vector2f& getWanderingDirection(float constraint, float max_time_ms, int when_opposite);
        [[nodiscard]] const ai::Goal& getCurrentGoal() const;
        [[nodiscard]] virtual const sf::Vector2f& getStartPosition() const = 0;
        [[nodiscard]] const ai::Path& getPath() const;

        [[nodiscard]] sf::Vector2f generateVelocityForPath() const;

    private:
        AgentsManager* manager_;

        ai::Timestamp latest_wander_point_time_;
        sf::Vector2f wandering_direction_;

    };

} // namespace r3e::ai

#endif //RAG3_ENGINE_INCLUDE_R3E_AI_ABSTRACTAGENT_H
