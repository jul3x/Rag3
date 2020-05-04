//
// Created by jul3x on 27.02.19.
//

#include <chrono>

#include <R3E/utils/Geometry.h>

#include <common/ResourceManager.h>

#include <events/Event.h>

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
    journal_ = std::make_unique<Journal>(CFG.get<float>("journal_max_time"), CFG.get<float>("journal_sampling_rate"));
    special_functions_ = std::make_unique<SpecialFunctions>();
    stats_ = std::make_unique<Stats>();
    achievements_ = std::make_unique<Achievements>();

    map_ = std::make_unique<Map>();
    agents_manager_ = std::make_unique<ai::AgentsManager>(map_->getMapBlockage(), ai::AStar::EightNeighbours,
                                                          1000.0f, // max time without recalculation of path in ms
                                                          20.0f, // min change of goal to trigger recalculation
                                                          1000); // max search of path

    music_manager_ = std::make_unique<audio::MusicManager>();
    music_manager_->addDirectoryToQueue(CFG.get<std::string>("paths/music_dir"));

    music_manager_->setVolume(50.0f);
    music_manager_->play();

    ui_->registerCamera(camera_.get());
    ui_->registerPlayer(player_.get());

    engine_->initializeGraphics(
            sf::Vector2i{CFG.get<int>("graphics/window_width_px"), CFG.get<int>("graphics/window_height_px")}, "Codename: Rag3",
            CFG.get<int>("graphics/full_screen") ? sf::Style::Fullscreen : sf::Style::Default,
            sf::Color(CFG.get<int>("graphics/background_color")));

    engine_->initializeSoundManager(CFG.get<float>("sound/sound_attenuation"));

    engine_->registerCamera(camera_.get());
    engine_->registerUI(ui_.get());

    map_->loadMap("test_map");

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

    for (auto& character : map_->getNPCs())
    {
        engine_->registerDynamicObject(character.get());

        character->registerAgentsManager(agents_manager_.get());
        character->registerEnemy(player_.get());
        character->registerMapBlockage(&map_->getMapBlockage());

        for (const auto& function : character->getFunctions())
        {
            character->bindFunction(special_functions_->bindFunction( function ),
                                    special_functions_->bindTextToUse( function ),
                                    special_functions_->isUsableByNPC( function ));
        }

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

    for (auto& special : map_->getSpecials())
    {
        engine_->registerHoveringObject(special.get());

        for (const auto& function : special->getFunctions())
        {
            special->bindFunction(special_functions_->bindFunction( function ),
                                  special_functions_->bindTextToUse( function ),
                                  special_functions_->isUsableByNPC( function ));
        }
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

            if (player_clone_ != nullptr)
            {
                player_clone_->setCurrentSpecialObject(nullptr);
                if (!player_clone_->update(time_elapsed))
                {
                    // draw on this place destruction
                    auto dec_ptr = map_->spawnDecoration(player_clone_->getPosition(), "blood");
                    journal_->eventDecorationSpawned(dec_ptr);

                    this->spawnExplosionEvent(player_clone_->getPosition(), 250.0f);
                    this->cleanPlayerClone();

                    player_->setHealth(0); // player clone is dead - so do player
                }

                if (!player_clone_->isLifeTime())
                {
                    this->spawnTeleportationEvent(player_clone_->getPosition());

                    const auto &player_clone_weapon = player_clone_->getWeapons().at(player_clone_->getCurrentWeapon());
                    auto player_weapon = std::find_if(player_->getWeapons().begin(), player_->getWeapons().end(),
                                                      [&player_clone_weapon](const auto &it) {
                                                          return player_clone_weapon->getName() == it->getName();
                                                      });

                    if (player_weapon != player_->getWeapons().end())
                        (*player_weapon)->setState(player_clone_weapon->getState());

                    player_->setHealth(player_clone_->getHealth());

                    camera_->setZoomInOut();

                    this->cleanPlayerClone();
                }
            }

            player_->setCurrentSpecialObject(nullptr);
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

            for (auto it = thoughts_.begin(); it != thoughts_.end(); ++it)
            {
                if (!(*it)->update(time_elapsed))
                {
                    auto next_it = std::next(it);
                    thoughts_.erase(it);
                    it = next_it;
                }
            }

            journal_->update(time_elapsed);
            camera_->update(time_elapsed);

            if (CFG.get<int>("sound/sound_on"))
            {
                Engine::changeSoundListenerPosition(player_->getPosition());
                music_manager_->update(time_elapsed);
            }
            break;
        }
        case GameState::Reverse:
        {
            if (!journal_->executeTimeReversal(CFG.get<float>("time_reversal_speed_factor") * time_elapsed))
            {
                this->setGameState(GameState::Normal);
            }

            camera_->update(time_elapsed);
            break;
        }
    }
}

void Game::updateMapObjects(float time_elapsed)
{
    auto& obstacles_tiles = map_->getObstaclesTiles();
    auto& obstacles = map_->getObstacles();
    auto& npcs = map_->getNPCs();
    auto& blockage = map_->getMapBlockage();
    auto& specials = map_->getSpecials();
    auto& decorations = map_->getDecorations();

    for (auto it = obstacles_tiles.begin(); it != obstacles_tiles.end();)
    {
        bool do_increment = true;
        if (!(*it)->update(time_elapsed))
        {
            journal_->eventObstacleTileDestroyed(it->get());
            // draw on this place destruction
            auto dec_ptr = map_->spawnDecoration((*it)->getPosition(), "destroyed_wall");
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

    for (auto it = npcs.begin(); it != npcs.end();)
    {
        bool do_increment = true;
        (*it)->setCurrentSpecialObject(nullptr);
        if (!(*it)->update(time_elapsed))
        {
            this->killNPC(it->get());

            auto next_it = std::next(it);
            npcs.erase(it);
            it = next_it;
            do_increment = false;
        }

        if (do_increment) ++it;
    }

    for (auto it = specials.begin(); it != specials.end();)
    {
        bool do_increment = true;
        (*it)->updateAnimation(time_elapsed);

        if (!(*it)->isActive())
        {
            auto next_it = std::next(it);
            this->deleteHoveringObject(it->get());

            specials.erase(it);
            it = next_it;
            do_increment = false;
        }

        if (do_increment) ++it;
    }

    for (auto it = decorations.begin(); it != decorations.end();)
    {
        bool do_increment = true;
        (*it)->updateAnimation(time_elapsed);

        if (!(*it)->isActive())
        {
            auto next_it = std::next(it);

            decorations.erase(it);
            it = next_it;
            do_increment = false;
        }

        if (do_increment) ++it;
    }

}

void Game::killNPC(NPC* npc)
{
    journal_->eventCharacterDestroyed(npc);
    stats_->killEnemy();

    if (player_clone_ != nullptr)
    {
        player_clone_->removeEnemy(npc);
    }

    // draw on this place destruction
    auto dec_ptr = map_->spawnDecoration((npc)->getPosition(), "blood");
    journal_->eventDecorationSpawned(dec_ptr);

    this->spawnExplosionEvent(npc->getPosition(), 250.0f);

    this->deleteDynamicObject(npc);

    if (npc->getActivation() == "OnKill")
    {
        npc->use(player_.get());
    }
}

void Game::draw(graphics::Graphics& graphics)
{
    for (auto& decoration : map_->getDecorationsTiles())
        graphics.drawSorted(*decoration);

    for (auto& decoration : map_->getDecorations())
        graphics.drawSorted(*decoration);

    for (auto& special : map_->getSpecials())
        if (special->isDrawable())
            graphics.drawSorted(*special);

    for (auto& obstacle : map_->getObstaclesTiles())
        graphics.drawSorted(*obstacle);

    for (auto& obstacle : map_->getObstacles())
        graphics.drawSorted(*obstacle);

    for (auto& character : map_->getNPCs())
        graphics.drawSorted(*character);

    for (auto& bullet : bullets_)
        graphics.drawSorted(*bullet);

    if (player_->isAlive())
        graphics.drawSorted(*player_);

    if (player_clone_ != nullptr)
        graphics.drawSorted(*player_clone_);

    engine_->drawSortedAnimationEvents();

    graphics.drawAlreadySorted();

    for (auto& thought : thoughts_)
        graphics.draw(*thought);
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

Player& Game::getPlayer()
{
    return *player_;
}

Camera& Game::getCamera()
{
    return *camera_;
}

Stats& Game::getStats()
{
    return *stats_;
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
    engine_->spawnAnimationEvent(std::make_shared<Event>(pos, "sparks", dir, r));
}

void Game::spawnExplosionEvent(const sf::Vector2f& pos, const float r)
{
    auto number = std::to_string(utils::num::getRandom(1, 3));
    engine_->spawnAnimationEvent(std::make_shared<Event>(pos, "explosion_" + number, 0.0f, r));

    if (CFG.get<int>("sound/sound_on"))
        engine_->spawnSoundEvent(RM.getSound("wall_explosion"), pos);
}

void Game::spawnTeleportationEvent(const sf::Vector2f& pos)
{
    engine_->spawnAnimationEvent(std::make_shared<Event>(pos + sf::Vector2f{0.0f, 10.0f}, "teleportation"));

    if (CFG.get<int>("sound/sound_on"))
        engine_->spawnSoundEvent(RM.getSound("teleportation"), pos);
}

void Game::spawnFadeInOut()
{
    engine_->spawnEffect(std::make_shared<graphics::FadeInOut>(
            sf::Vector2f{static_cast<float>(CFG.get<int>("graphics/window_width_px")),
                         static_cast<float>(CFG.get<int>("graphics/window_height_px"))}, sf::Color::Black,
            CFG.get<float>("fade_in_out_duration")
            ));
}

void Game::spawnThought(Character* user, const std::string& text)
{
    thoughts_.emplace_back(std::make_unique<Thought>(user, text, CFG.get<float>("thought_duration")));
}

void Game::spawnAchievement(Achievements::Type type)
{
    ui_->spawnAchievement(
            achievements_->getAchievementTitle(type),
            achievements_->getAchievementText(type),
            achievements_->getAchievementTexture(type)
            );
}

void Game::spawnSpecial(const sf::Vector2f& pos, const std::string& name)
{
    auto ptr = map_->spawnSpecial(pos, name);
    engine_->registerHoveringObject(ptr);

    ptr->bindFunction(special_functions_->bindFunction(ptr->getFunctions().at(0)),
                      special_functions_->bindTextToUse(ptr->getFunctions().at(0)),
                      special_functions_->isUsableByNPC(ptr->getFunctions().at(0)));
}

void Game::spawnShotEvent(const std::string& name, const sf::Vector2f& pos, const float dir)
{
    auto shot_event = std::make_shared<Event>(pos, "shot", dir * 180.0f / M_PI,
                                                  utils::j3x::get<float>(RM.getObjectParams("bullets", name), "burst_size"));
    engine_->spawnAnimationEvent(shot_event);

    if (CFG.get<int>("sound/sound_on"))
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

    if (obstacle != nullptr && bullet != nullptr)
    {
        journal_->eventObstacleShot(obstacle);

        obstacle->getShot(*bullet);

        spawnSparksEvent(bullet->getPosition(), bullet->getRotation() - 90.0f,
                         static_cast<float>(std::pow(CFG.get<float>("graphics/sparks_size_factor") * bullet->getDeadlyFactor(), 0.4f)));

        bullet->setDead();
    }
    else if (obstacle_tile != nullptr && bullet != nullptr)
    {
        journal_->eventObstacleTileShot(obstacle_tile);

        obstacle_tile->getShot(*bullet);

        spawnSparksEvent(bullet->getPosition(), bullet->getRotation() - 90.0f,
                         static_cast<float>(std::pow(CFG.get<float>("graphics/sparks_size_factor") * bullet->getDeadlyFactor(), 0.4f)));

        bullet->setDead();
    }

}

void Game::alertCollision(HoveringObject* h_obj, DynamicObject* d_obj)
{
    auto bullet = dynamic_cast<Bullet*>(h_obj);
    auto character = dynamic_cast<Character*>(d_obj);

    if (bullet != nullptr)
    {
        character->getShot(*bullet);
        spawnSparksEvent(bullet->getPosition(), bullet->getRotation() - 90.0f,
                         static_cast<float>(std::pow(
                                 CFG.get<float>("graphics/sparks_size_factor") * bullet->getDeadlyFactor(), 0.4f)));

        bullet->setDead();
        return;
    }

    auto special = dynamic_cast<Special*>(h_obj);

    if (special != nullptr && character != nullptr && special->isActive())
    {
        if (special->getActivation() == "OnEnter")
        {
            auto player = dynamic_cast<Player*>(d_obj);
            if (special->isUsableByNPC())
            {
                special->use(character);
            }
            else if (player != nullptr)
            {
                special->use(player);
            }
        }
        else
        {
            character->setCurrentSpecialObject(special);
        }
    }
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

NPC* Game::spawnNewNPC(const std::string &id)
{
    auto ptr = map_->spawnCharacter({}, id);
    engine_->registerDynamicObject(ptr);

    ptr->registerAgentsManager(agents_manager_.get());
    ptr->registerEnemy(player_.get());

    if (player_clone_ != nullptr)
        player_clone_->registerEnemy(ptr);

    ptr->registerMapBlockage(&map_->getMapBlockage());

    for (auto& weapon : ptr->getWeapons())
    {
        weapon->registerSpawningFunction(std::bind(&Game::spawnBullet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    for (const auto& function : ptr->getFunctions())
    {
        ptr->bindFunction(special_functions_->bindFunction( function ),
                          special_functions_->bindTextToUse( function ),
                          special_functions_->isUsableByNPC( function ));
    }

    return ptr;
}

NPC* Game::spawnNewPlayerClone()
{
    if (player_clone_ != nullptr)
    {
        this->cleanPlayerClone();
    }

    player_clone_ = std::make_unique<PlayerClone>(this->getPlayerPosition(), player_.get(),
                                                  journal_->getDurationSaved() * CFG.get<float>("player_clone_time_factor"));
    engine_->registerDynamicObject(player_clone_.get());

    player_clone_->registerAgentsManager(agents_manager_.get());
    player_clone_->registerMapBlockage(&map_->getMapBlockage());

    for (auto& enemy : map_->getNPCs())
    {
        player_clone_->registerEnemy(enemy.get());

        enemy->registerEnemy(player_clone_.get());
    }

    for (auto& weapon : player_clone_->getWeapons())
    {
        weapon->registerSpawningFunction(std::bind(&Game::spawnBullet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    return player_clone_.get();
}

void Game::cleanPlayerClone()
{
    for (auto& enemy : map_->getNPCs())
    {
        enemy->removeEnemy(player_clone_.get());
    }

    this->deleteDynamicObject(player_clone_.get());
    player_clone_.reset();
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

Special* Game::getCurrentSpecialObject() const
{
    return player_->getCurrentSpecialObject();
}

void Game::useSpecialObject()
{
    auto curr = player_->getCurrentSpecialObject();
    if (curr != nullptr)
    {
        curr->use(player_.get());
    }
}

void Game::setBulletTime()
{
    current_time_factor_ = CFG.get<float>("bullet_time_factor");
    engine_->setTimeScaleFactor(current_time_factor_);

    if (CFG.get<int>("sound/sound_on"))
        music_manager_->setPlaybackPitch(CFG.get<float>("sound/bullet_time_music_factor"));
}

void Game::setNormalTime()
{
    current_time_factor_ = 1.0f;
    engine_->setTimeScaleFactor(1.0f);

    if (CFG.get<int>("sound/sound_on"))
        music_manager_->setPlaybackPitch(1.0f);
}

void Game::setGameState(Game::GameState state)
{
    switch (state)
    {
        case GameState::Normal:
            if (state_ == GameState::Reverse)
            {
                if (player_clone_ != nullptr)
                    player_clone_->updateLifeTimeDependingOnPrevious(
                            journal_->getDurationSaved() * CFG.get<float>("player_clone_time_factor"));
                journal_->clear();
            }

            camera_->setNormal();
            engine_->turnOnCollisions();
            break;
        case GameState::Reverse:
            if (this->isJournalFreezed())
                return;

            this->spawnTeleportationEvent(player_->getPosition());

            camera_->setReverse();
            engine_->turnOffCollisions();
            journal_->eventTimeReversal();
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
    return journal_->getDurationSaved() < CFG.get<float>("journal_min_time");
}

float Game::getCurrentTimeFactor() const
{
    return current_time_factor_;
}

float Game::getFPS() const
{
    return engine_->getCurrentFPS();
}
