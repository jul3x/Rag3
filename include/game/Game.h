//
// Created by jul3x on 18.12.19.
//

#ifndef RAG3_GAME_GAME_H
#define RAG3_GAME_GAME_H

#include <memory>

#include <game/misc/Camera.h>
#include <engine/system/AbstractGame.h>
#include <game/characters/Player.h>
#include <game/environment/Map.h>
#include <game/weapons/Bullet.h>
#include <game/ui/UserInterface.h>
#include <engine/system/Engine.h>


class Game : public AbstractGame {

public:
    Game(const Game&) = delete;

    Game& operator=(const Game&) = delete;

    static Game& get()
    {
        static Game instance;
        return instance;
    }

    const sf::Vector2f& getPlayerPosition() const;

    void spawnBullet(const std::string& name, const sf::Vector2f& pos, float dir);

    void spawnSparksAnimation(const sf::Vector2f& pos, float dir, float r);

    void spawnShotAnimation(const sf::Vector2f& pos, float dir, float r);

    void spawnExplosionAnimation(const sf::Vector2f& pos, float r);

    void initialize() override;

    void update(float time_elapsed) override;

    void draw(Graphics& graphics) override;

    void alertCollision(HoveringObject* h_obj, StaticObject* s_obj) override;

    void alertCollision(HoveringObject* h_obj, DynamicObject* d_obj) override;

    void alertCollision(DynamicObject* d_obj, StaticObject* s_obj) override;

    void alertCollision(DynamicObject* d_obj_1, DynamicObject* d_obj_2) override;

    void deleteStaticObject(StaticObject* s_obj);

    void deleteHoveringObject(HoveringObject* h_obj);

    void deleteDynamicObject(DynamicObject* d_obj);

    void start(int frame_rate);

private:
    Game();

    std::unique_ptr<Engine> engine_;
    std::unique_ptr<UserInterface> ui_;
    std::unique_ptr<Camera> camera_;

    std::unique_ptr<Player> player_;
    std::unique_ptr<Map> map_;
    std::list<std::unique_ptr<Bullet>> bullets_;

};


#endif //RAG3_GAME_GAME_H