//
// Created by jprolejko on 26.02.19.
//

#ifndef RAG3_ENGINE_GRAPHICS_GRAPHICS_H
#define RAG3_ENGINE_GRAPHICS_GRAPHICS_H

#include <SFML/Graphics.hpp>

#include <engine/objects/AbstractDrawableObject.h>
#include <engine/system/Config.h>


class Graphics {

public:
    Graphics(const Graphics&) = delete;

    Graphics& operator=(const Graphics&) = delete;

    static Graphics& getInstance()
    {
        static Graphics graphics_instance;
        return graphics_instance;
    }

    bool isWindowOpen() const;

    sf::RenderWindow& getWindow();

    sf::View& getCurrentView();

    sf::View& getStaticView();

    void setStaticView();

    void setCurrentView();

    void modifyCurrentView(const sf::View& view);

    void modifyStaticView(const sf::View& view);

    void setViewCenter(const sf::Vector3f& view);

    void clear();

    void draw(const AbstractDrawableObject& object);

    void display();

private:
    Graphics();

    sf::ContextSettings settings_;
    sf::RenderWindow window_;
    sf::View current_view_;
    sf::View static_view_;

};


#endif //RAG3_ENGINE_GRAPHICS_GRAPHICS_H
