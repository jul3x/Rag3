//
// Created by jul3x on 16.03.2021.
//

#ifndef RAG3_MULTI_INCLUDE_MINIMALUSERINTERFACE_H
#define RAG3_MULTI_INCLUDE_MINIMALUSERINTERFACE_H

#include <common/ui/UserInterface.h>


class Server;

class MinimalUserInterface : public UserInterface {

public:
    explicit MinimalUserInterface(Server* server);

    void spawnAchievement(const std::string& title, const std::string& text, const std::string& tex) override;
    void spawnThought(Character* user, const std::string& text) override;
    void spawnBonusText(const sf::Vector2f& pos, const std::string& text) override;
    void spawnAcceptWindow(const std::string& text, const std::function<void()>& func) override;
    void spawnNoteWindow(const std::string& text) override;
    void closeWindow(Window* window) override;

    void draw(graphics::Graphics& graphics) override;

protected:
    void handleScrolling(float delta) override;
    void handleKeys() override;
    void handleMouse(sf::RenderWindow& graphics_window) override;
    void updatePlayerStates(float time_elapsed) override;


};

#endif //RAG3_MULTI_INCLUDE_MINIMALUSERINTERFACE_H
