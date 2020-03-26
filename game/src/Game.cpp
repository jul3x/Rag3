//
// Created by jul3x on 27.02.19.
//

#include <chrono>

#include <R3E/utils/Geometry.h>

#include <common/ResourceManager.h>

#include <events/ExplosionEvent.h>
#include <events/ShotEvent.h>
#include <events/SparksEvent.h>

#include <Game.h>


Game::Game() : current_time_factor_(1.0f), state_(GameState::Normal)
{
    engine_ = std::make_unique<Engine>();
    engine_->registerGame(this);
}

void Game::initialize()
{
    player_ = std::make_unique<Player>(sf::Vector2f{0.0f, 0.0f});
    ui_ = std::make_unique<UserInterface>();
    camera_ = std::make_unique<Camera>();
    journal_ = std::make_unique<Journal>(CFG.getFloat("journal_max_time"), CFG.getFloat("journal_sampling_rate"));
    map_ = std::make_unique<Map>();
    agents_manager_ = std::make_unique<ai::AgentsManager>(map_->getMapBlockage(), ai::AStar::EightNeighbours,
                                                          1000.0f, // max time without recalculation of path in ms
                                                          20.0f, // min change of goal to trigger recalculation
                                                          1000); // max search of path

    music_manager_ = std::make_unique<audio::MusicManager>();
    music_manager_->addDirectoryToQueue(CFG.getString("paths/music_dir"));

    music_manager_->setVolume(50.0f);
    music_manager_->play();

    ui_->registerCamera(camera_.get());
    ui_->registerPlayer(player_.get());

    engine_->initializeGraphics(
            sf::Vector2i{CFG.getInt("graphics/window_width_px"), CFG.getInt("graphics/window_height_px")}, "Codename: Rag3",
            CFG.getInt("graphics/full_screen") ? sf::Style::Fullscreen : sf::Style::Default,
            sf::Color(CFG.getInt("graphics/background_color")));

    engine_->initializeSoundManager(CFG.getFloat("sound/sound_attenuation"));

    engine_->registerCamera(camera_.get());
    engine_->registerUI(ui_.get());

    map_->loadMap("map");

    for (auto& special : map_->getSpecials())
    {
        if (special->getId() == "starting_position")
            player_->setPosition(special->getPosition());
    }

    engine_->initializeCollisions(map_->getSize(), COLLISION_GRID_SIZE_);

    for (auto& obstacle : map_->getObstaclesTiles())
        engine_->registerStaticObject(obstacle.get());

    for (auto& obstacle : map_->getObstacles())
        engine_->registerStaticObject(obstacle.get());

    for (auto& character : map_->getCharacters())
    {
        engine_->registerDynamicObject(character.get());

        character->registerAgentsManager(agents_manager_.get());
        character->registerEnemy(player_.get());
        character->registerMapBlockage(&map_->getMapBlockage());

        for (auto& weapon : character->getWeapons())
        {
            weapon->registerSpawningFunction(std::bind(&Game::spawnBullet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        }
    }

    engine_->registerDynamicObject(player_.get());
    for (auto& weapon : player_->getWeapons())
    {
        weapon->registerSpawningFunction(std::bind(&Game::spawnBullet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
}

void Game::update(float time_elapsed)
{
    switch (state_)
    {
        case GameState::Paused:
        {
            break;
        }
        case GameState::Normal:
        {
            agents_manager_->update();

            updateMapObjects(time_elapsed);

            if (player_->isAlive() && !player_->update(time_elapsed))
            {
                map_->spawnDecoration(player_->getPosition(), "blood");
                spawnExplosionEvent(player_->getPosition(), 25.0f);
                player_->setDead();
                deleteDynamicObject(player_.get());
                music_manager_->stop();
            }

            for (auto it = bullets_.begin(); it != bullets_.end(); ++it)
            {
                if (!(*it)->update(time_elapsed))
                {
                    journal_->eventBulletDestroyed(it->get());
                    deleteHoveringObject(it->get());
                    auto next_it = std::next(it);
                    bullets_.erase(it);
                    it = next_it;
                }
            }

            journal_->update(time_elapsed);
            camera_->update(time_elapsed);

            if (CFG.getInt("sound/sound_on"))
            {
                Engine::changeSoundListenerPosition(player_->getPosition());
                music_manager_->update(time_elapsed);
            }
            break;
        }
        case GameState::Reverse:
        {
            if (!journal_->executeTimeReversal(CFG.getFloat("time_reversal_speed_factor") * time_elapsed))
            {
                this->setGameState(GameState::Normal);
            }

            break;
        }
    }
}

void Game::updateMapObjects(float time_elapsed)
{
    auto& obstacles_tiles = map_->getObstaclesTiles();
    auto& obstacles = map_->getObstacles();
    auto& enemies = map_->getCharacters();
    auto& blockage = map_->getMapBlockage();

    for (auto it = obstacles_tiles.begin(); it != obstacles_tiles.end();)
    {
        bool do_increment = true;
        if (!(*it)->update(time_elapsed))
        {
            journal_->eventObstacleTileDestroyed(it->get());
            // draw on this place destruction
            auto dec_ptr = map_->spawnDecoration((*it)->getPosition(), "destroyed_wall");
            journal_->eventDecorationSpawned(dec_ptr);
            dec_ptr = map_->spawnDecoration((*it)->getPosition(), "flame");
            journal_->eventDecorationSpawned(dec_ptr);

            this->spawnExplosionEvent((*it)->getPosition(), 250.0f);

            auto next_it = std::next(it);
            this->deleteStaticObject(it->get());

            auto grid_pos = std::make_pair(static_cast<size_t>((*it)->getPosition().x / DecorationTile::SIZE_X_),
                                           static_cast<size_t>((*it)->getPosition().y / DecorationTile::SIZE_Y_));
            blockage.blockage_.at(grid_pos.first).at(grid_pos.second) = false;

            obstacles_tiles.erase(it);
            it = next_it;
            do_increment = false;
        }

        if (do_increment) ++it;
    }

    for (auto it = obstacles.begin(); it != obstacles.end();)
    {
        bool do_increment = true;
        (*it)->updateAnimation(time_elapsed);
        if (!(*it)->update(time_elapsed))
        {
            journal_->eventObstacleDestroyed(it->get());
            // draw on this place destruction
            auto dec_ptr = map_->spawnDecoration((*it)->getPosition(), "flame");
            journal_->eventDecorationSpawned(dec_ptr);

            this->spawnExplosionEvent((*it)->getPosition(), 250.0f);

            auto next_it = std::next(it);
            this->deleteStaticObject(it->get());

            auto grid_pos = std::make_pair(static_cast<size_t>((*it)->getPosition().x / DecorationTile::SIZE_X_),
                                           static_cast<size_t>((*it)->getPosition().y / DecorationTile::SIZE_Y_));
            blockage.blockage_.at(grid_pos.first).at(grid_pos.second) = 0.0f;

            obstacles.erase(it);
            it = next_it;
            do_increment = false;
        }

        if (do_increment) ++it;
    }

    for (auto it = enemies.begin(); it != enemies.end();)
    {
        bool do_increment = true;
        if (!(*it)->update(time_elapsed, this->getCurrentTimeFactor()))
        {
            journal_->eventEnemyDestroyed(it->get());

            // draw on this place destruction
            auto dec_ptr = map_->spawnDecoration((*it)->getPosition(), "blood");
            journal_->eventDecorationSpawned(dec_ptr);

            this->spawnExplosionEvent((*it)->getPosition(), 250.0f);

            auto next_it = std::next(it);
            this->deleteDynamicObject(it->get());

            enemies.erase(it);
            it = next_it;
            do_increment = false;
        }

        if (do_increment) ++it;
    }

    for (auto& decoration : map_->getDecorations())
        decoration->updateAnimation(time_elapsed);

    for (auto& collectible : map_->getCollectibles())
        collectible->updateAnimation(time_elapsed);

}

void Game::draw(graphics::Graphics& graphics)
{
    for (auto& decoration : map_->getDecorationsTiles())
        graphics.draw(*decoration);

    for (auto& decoration : map_->getDecorations())
        graphics.draw(*decoration);

    for (auto& obstacle : map_->getObstaclesTiles())
        graphics.drawSorted(*obstacle);

    for (auto& obstacle : map_->getObstacles())
        graphics.drawSorted(*obstacle);

    for (auto& character : map_->getCharacters())
        graphics.drawSorted(*character);

    for (auto& bullet : bullets_)
        graphics.drawSorted(*bullet);

    if (player_->isAlive())
        graphics.drawSorted(*player_);

    engine_->drawSortedAnimationEvents();

    graphics.drawAlreadySorted();
}

void Game::start()
{
    engine_->start();
}

const sf::Vector2f& Game::getPlayerPosition() const
{
    return player_->getPosition();
}

Map& Game::getMap()
{
    return *map_;
}

const Journal& Game::getJournal() const
{
    return *journal_;
}

const std::list<std::unique_ptr<Bullet>>& Game::getBullets() const
{
    return bullets_;
}

const ai::MapBlockage& Game::getMapBlockage() const
{
    return map_->getMapBlockage();
}

void Game::spawnSparksEvent(const sf::Vector2f& pos, const float dir, const float r)
{
    engine_->spawnAnimationEvent(std::make_shared<SparksEvent>(pos, dir, r));
}

void Game::spawnExplosionEvent(const sf::Vector2f& pos, const float r)
{
    engine_->spawnAnimationEvent(std::make_shared<ExplosionEvent>(pos, r));

    if (CFG.getInt("sound/sound_on"))
        engine_->spawnSoundEvent(RM.getSound("wall_explosion"), pos);
}

void Game::spawnShotEvent(const std::string& name, const sf::Vector2f& pos, const float dir)
{
    auto shot_event = std::make_shared<ShotEvent>(pos, dir * 180.0f / M_PI,
                                                  utils::getFloat(RM.getObjectParams("bullets", name), "burst_size"));
    engine_->spawnAnimationEvent(shot_event);

    if (CFG.getInt("sound/sound_on"))
        engine_->spawnSoundEvent(RM.getSound(name + "_bullet_shot"), pos);
}

void Game::spawnBullet(const std::string& name, const sf::Vector2f& pos, const float dir)
{
    auto ptr = this->spawnNewBullet(name, pos, dir);
    journal_->eventBulletSpawned(ptr);
    this->spawnShotEvent(name, pos, dir);
}

void Game::alertCollision(HoveringObject* h_obj, StaticObject* s_obj)
{
    auto bullet = dynamic_cast<Bullet*>(h_obj);
    auto obstacle = dynamic_cast<Obstacle*>(s_obj);
    auto obstacle_tile = dynamic_cast<ObstacleTile*>(s_obj);

    if (obstacle != nullptr)
    {
        journal_->eventObstacleShot(obstacle);

        obstacle->getShot(*bullet);
    }
    else
    {
        journal_->eventObstacleTileShot(obstacle_tile);

        obstacle_tile->getShot(*bullet);
    }

    spawnSparksEvent(bullet->getPosition(), bullet->getRotation() - 90.0f,
                     static_cast<float>(std::pow(CFG.getFloat("graphics/sparks_size_factor") * bullet->getDeadlyFactor(), 0.4f)));

    bullet->setDead();
}

void Game::alertCollision(HoveringObject* h_obj, DynamicObject* d_obj)
{
    auto bullet = dynamic_cast<Bullet*>(h_obj);
    auto character = dynamic_cast<Character*>(d_obj);
    character->getShot(*bullet);
    spawnSparksEvent(bullet->getPosition(), bullet->getRotation() - 90.0f,
                     static_cast<float>(std::pow(CFG.getFloat("graphics/sparks_size_factor") * bullet->getDeadlyFactor(), 0.4f)));

    bullet->setDead();
}

void Game::alertCollision(DynamicObject* d_obj, StaticObject* s_obj)
{
    // Nothing to do for now (maybe sounds?)
}

void Game::alertCollision(DynamicObject* d_obj_1, DynamicObject* d_obj_2)
{
    // Nothing to do for now (maybe sounds?)
}

void Game::deleteStaticObject(StaticObject* s_obj)
{
    engine_->deleteStaticObject(s_obj);
}

void Game::deleteHoveringObject(HoveringObject* h_obj)
{
    engine_->deleteHoveringObject(h_obj);
}

void Game::deleteDynamicObject(DynamicObject* d_obj)
{
    engine_->deleteDynamicObject(d_obj);
}

Bullet* Game::spawnNewBullet(const std::string &id, const sf::Vector2f &pos, float dir)
{
    bullets_.emplace_back(std::make_unique<Bullet>(pos, id, dir));

    auto ptr = bullets_.back().get();

    engine_->registerHoveringObject(ptr);

    return ptr;
}

Enemy* Game::spawnNewEnemy(const std::string& id)
{
    auto ptr = map_->spawnCharacter({}, id);
    engine_->registerDynamicObject(ptr);

    ptr->registerAgentsManager(agents_manager_.get());
    ptr->registerEnemy(player_.get());
    ptr->registerMapBlockage(&map_->getMapBlockage());

    for (auto& weapon : ptr->getWeapons())
    {
        weapon->registerSpawningFunction(std::bind(&Game::spawnBullet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    return ptr;
}

ObstacleTile* Game::spawnNewObstacleTile(const std::string& id, const sf::Vector2f& pos)
{
    auto new_ptr = map_->spawnObstacleTile(pos, id);
    engine_->registerStaticObject(new_ptr);
    return new_ptr;
}


Obstacle* Game::spawnNewObstacle(const std::string& id, const sf::Vector2f& pos)
{
    auto new_ptr = map_->spawnObstacle(pos, id);
    engine_->registerStaticObject(new_ptr);
    return new_ptr;
}

void Game::findAndDeleteBullet(Bullet* ptr)
{
    for (auto it = bullets_.rbegin(); it != bullets_.rend(); ++it)
    {
        if (it->get() == ptr)
        {
            deleteHoveringObject(ptr);
            bullets_.erase((++it).base());
            return;
        }
    }

    std::cerr << "[Game] Warning - bullet to delete not found!" << std::endl;
}

void Game::findAndDeleteDecoration(Decoration* ptr)
{
    auto& decorations = map_->getDecorations();
    for (auto it = decorations.rbegin(); it != decorations.rend(); ++it)
    {
        if (it->get() == ptr)
        {
            decorations.erase((++it).base());
            return;
        }
    }

    std::cerr << "[Game] Warning - decoration to delete not found!" << std::endl;
}

ai::AgentsManager& Game::getAgentsManager() const
{
    return *agents_manager_;
}

void Game::setBulletTime()
{
    current_time_factor_ = CFG.getFloat("bullet_time_factor");
    engine_->setTimeScaleFactor(current_time_factor_);

    if (CFG.getInt("sound/sound_on"))
        music_manager_->setPlaybackPitch(CFG.getFloat("sound/bullet_time_music_factor"));
}

void Game::setNormalTime()
{
    current_time_factor_ = 1.0f;
    engine_->setTimeScaleFactor(1.0f);

    if (CFG.getInt("sound/sound_on"))
        music_manager_->setPlaybackPitch(1.0f);
}

void Game::setGameState(Game::GameState state)
{
    switch (state)
    {
        case GameState::Normal:
            if (state_ == GameState::Reverse)
                journal_->clear();

            engine_->turnOnCollisions();
            break;
        case GameState::Reverse:
            if (this->isJournalFreezed())
                return;

            engine_->turnOffCollisions();
            break;
        case GameState::Paused:
            break;
    }

    state_ = state;
}

Game::GameState Game::getGameState() const
{
    return state_;
}

bool Game::isJournalFreezed() const
{
    return journal_->getDurationSaved() < CFG.getFloat("journal_min_time");
}

float Game::getCurrentTimeFactor() const
{
    return current_time_factor_;
}

float Game::getFPS() const
{
    return engine_->getCurrentFPS();
}
