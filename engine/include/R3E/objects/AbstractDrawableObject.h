//
// Created by jul3x on 16.06.19.
//


#ifndef RAG3_ENGINE_INCLUDE_R3E_OBJECTS_ABSTRACTDRAWABLEOBJECT_H
#define RAG3_ENGINE_INCLUDE_R3E_OBJECTS_ABSTRACTDRAWABLEOBJECT_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


namespace r3e {
    class AbstractDrawableObject : public sf::Drawable {
    public:
        AbstractDrawableObject();

        AbstractDrawableObject(const sf::Vector2f& position,
                               const sf::Vector2f& size,
                               sf::Texture* texture_name,
                               int z_index = 0,
                               short int frames_number = 1,
                               float frame_duration = 0.0f);

        virtual const sf::Vector2f& getPosition() const;
        virtual const sf::Vector2f& getSize() const;
        virtual short int getFramesNumber() const;
        virtual short int getCurrentFrame() const;
        virtual float getRotation() const;
        virtual int getZIndex() const;
        virtual bool isVisible() const;

        virtual void setPosition(const sf::Vector2f& position);
        virtual void setPosition(float x, float y);
        virtual void setPositionX(float x);
        virtual void setPositionY(float y);
        virtual void setRotation(float angle_deg);
        virtual void setSize(const sf::Vector2f& size);
        virtual void setVisibility(const sf::View& view);
        virtual void setColor(int r, int g, int b, int a);
        virtual void setCurrentFrame(short int frame);
        virtual void setFlipX(bool flip);
        virtual void setFlipY(bool flip);
        virtual void setZIndex(int z_index);

        virtual void changeTexture(sf::Texture* texture, bool reset = false);
        virtual void changeTextureRect(const sf::IntRect& rect);
        virtual void changeOrigin(const sf::Vector2f& origin);

        virtual bool updateAnimation(float time_elapsed, float animation_speed_factor = 1.0f);

    protected:
        sf::RectangleShape shape_;
        bool is_visible_;
        bool is_flipped_x_;
        bool is_flipped_y_;
        int z_index_;

        float time_elapsed_;
        short int current_frame_;
        sf::Vector2i frame_size_;
        sf::IntRect animation_source_;

        short int frames_number_;
        float frame_duration_;

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    };

    struct AbstractDrawableObjectsCmp {
        bool operator()(const AbstractDrawableObject* lhs, const AbstractDrawableObject* rhs) const
        {
            return (lhs->getZIndex() < rhs->getZIndex()) ||
                   (lhs->getZIndex() == rhs->getZIndex() && lhs->getPosition().y < rhs->getPosition().y) ||
                   (lhs->getZIndex() == rhs->getZIndex() && lhs->getPosition().y == rhs->getPosition().y &&
                    lhs->getPosition().x <= rhs->getPosition().x);
        }
    };

} // namespace r3e

#endif //RAG3_ENGINE_INCLUDE_R3E_OBJECTS_ABSTRACTDRAWABLEOBJECT_H
