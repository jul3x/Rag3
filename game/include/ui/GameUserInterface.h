//
// Created by jul3x on 27.02.19.
//

#ifndef RAG3_GAME_INCLUDE_UI_GAMEUSERINTERFACE_H
#define RAG3_GAME_INCLUDE_UI_GAMEUSERINTERFACE_H

#include <TGUI/TGUI.hpp>

#include <R3E/system/Config.h>

#include <ui/menu/Menu.h>
#include <common/ui/HudBar.h>
#include <ui/BloodSplash.h>
#include <ui/hud/LeftHud.h>
#include <ui/hud/StatsHud.h>
#include <common/ui/SmallBackpackHud.h>
#include <ui/hud/FullHud.h>

#include <common/ui/UserInterface.h>


using namespace r3e;

class Game;

class GameUserInterface : public UserInterface {

public:
    explicit GameUserInterface(Game* game);

    GameUserInterface(const GameUserInterface&) = delete;

    GameUserInterface& operator=(const GameUserInterface&) = delete;

    void initialize(graphics::Graphics& graphics) override;
    void update(graphics::Graphics& graphics, float time_elapsed) override;

    void spawnNoteWindow(const std::string& text) override;
    void startGame() override;
    void openMenu() override;

    void draw(graphics::Graphics& graphics) override;

private:
    void handleAdditionalKeyPressed(sf::Keyboard::Key code) override;
    void handleKeyReleased(sf::Keyboard::Key code) override;
    void handleMouse(sf::RenderWindow& graphics_window) override;
    void updatePlayerStates(float time_elapsed) override;

    // NORMAL HUD
    BloodSplash blood_splash_;
    LeftHud left_hud_;
    HudBar time_bar_;
    HudBar speed_bar_;
    StatsHud stats_hud_;

    // MENUS
    std::unique_ptr<FullHud> full_hud_;
    std::unique_ptr<Menu> menu_;

    Game* game_;

};


#endif //RAG3_GAME_INCLUDE_UI_GAMEUSERINTERFACE_H
