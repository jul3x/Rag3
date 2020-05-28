//
// Created by jul3x on 28.05.2020.
//

#include <cmath>

#include <R3E/graphics/StaticShadow.h>
#include <R3E/utils/Utils.h>


namespace r3e::graphics {
    StaticShadow::StaticShadow(const sf::Vector2f& position, const sf::Vector2f& size, float shadow_direction,
                               sf::Texture* texture_name, const sf::Color& shadow_color, short frames_number, float frame_duration) :
            frames_number_(frames_number),
            frame_duration_(frame_duration),
            frame_size_(sf::Vector2i(size.x, size.y)),
            current_frame_(0),
            time_elapsed_(0.0f),
            animation_source_({0, 0}, frame_size_),
            is_visible_(true),
            shape_(sf::Quads, 4)
    {
        position_ = position;
        shadow_direction_ = shadow_direction;

        shape_[0].position = position + sf::Vector2f{-size.x / 2.0f, size.y / 2.0f};
        shape_[1].position = position + sf::Vector2f{size.x / 2.0f, size.y / 2.0f};
        shape_[2].position = position + sf::Vector2f{size.x / 2.0f + std::sin(shadow_direction) * size.y,
                                                     size.y / 2.0f - std::cos(shadow_direction) * size.y};
        shape_[3].position = position + sf::Vector2f{-size.x / 2.0f + std::sin(shadow_direction) * size.y,
                                                     size.y / 2.0f - std::cos(shadow_direction) * size.y};

        shape_[0].texCoords = {0.0f, size.y};
        shape_[1].texCoords = {size.x, size.y};
        shape_[2].texCoords = {size.x, 0.0f};
        shape_[3].texCoords = {0.0f, 0.0f};
        shape_[0].color = shadow_color;
        shape_[1].color = shadow_color;
        shape_[2].color = shadow_color;
        shape_[3].color = shadow_color;

        if (texture_name != nullptr)
        {
            states_.texture = texture_name;
//            if (frames_number > 1)
//                shape_.setTextureRect(animation_source_);

//            shape_.setTexture(texture_name);
        }
    }

    const sf::Vector2f& StaticShadow::getPosition() const
    {
        return position_;
    }

    const sf::Vector2i& StaticShadow::getSize() const
    {
        return frame_size_;
    }

    short int StaticShadow::getFramesNumber() const
    {
        return frames_number_;
    }

    short int StaticShadow::getCurrentFrame() const
    {
        return current_frame_;
    }

    void StaticShadow::setPosition(const sf::Vector2f& position)
    {
        position_ = position;
        shape_[0].position = position + scale_ * sf::Vector2f{-frame_size_.x / 2.0f, frame_size_.y / 2.0f};
        shape_[1].position = position + scale_ * sf::Vector2f{frame_size_.x / 2.0f, frame_size_.y / 2.0f};
        shape_[2].position = position + scale_ * sf::Vector2f{frame_size_.x / 2.0f + std::sin(shadow_direction_) * frame_size_.y,
                                                     frame_size_.y / 2.0f - std::cos(shadow_direction_) * frame_size_.y};
        shape_[3].position = position + scale_ * sf::Vector2f{-frame_size_.x / 2.0f + std::sin(shadow_direction_) * frame_size_.y,
                                                     frame_size_.y / 2.0f - std::cos(shadow_direction_) * frame_size_.y};

    }

    void StaticShadow::setPosition(float x, float y)
    {
        this->setPosition({x, y});
    }

    void StaticShadow::setPositionX(float x)
    {
        this->setPosition(x, this->getPosition().y);
    }

    void StaticShadow::setPositionY(float y)
    {
        this->setPosition(this->getPosition().x, y);
    }

    void StaticShadow::setScale(float scale)
    {
        scale_ = scale;
    }

    void StaticShadow::setVisibility(const sf::View& view)
    {
        is_visible_ = utils::geo::AABB(view.getCenter(), view.getSize() + sf::Vector2f{300.0f, 300.0f},
                                       this->getPosition(), static_cast<sf::Vector2f>(frame_size_)) > 0;
    }

    void StaticShadow::setCurrentFrame(short int frame)
    {
        if (frame >= frames_number_)
        {
            throw std::invalid_argument("[AbstractDrawableObject] Frame to set should be less then frames_number_");
        }

        current_frame_ = frame;
    }

    void StaticShadow::changeTexture(sf::Texture* texture, bool reset)
    {
  //      shape_.setTexture(texture, reset);
    }

    void StaticShadow::changeTextureRect(const sf::IntRect& rect)
    {
  //      shape_.setTextureRect(rect);
    }

    void StaticShadow::changeOrigin(const sf::Vector2f& origin)
    {
   //     shape_.setOrigin(origin);
    }

    bool StaticShadow::updateAnimation(float time_elapsed, float animation_speed_factor)
    {
        if (frames_number_ <= 1) return true;

        time_elapsed_ += time_elapsed * animation_speed_factor;

        auto frames_to_add = static_cast<short int>(time_elapsed_ / frame_duration_);
        time_elapsed_ -= frames_to_add * frame_duration_;

        current_frame_ = current_frame_ + frames_to_add;

        if (current_frame_ >= frames_number_)
        {
            current_frame_ -= frames_number_;
        }

        animation_source_.left = frame_size_.x * current_frame_;
        animation_source_.top = 0;

     //   shape_.setTextureRect(animation_source_);

        return true;
    }

    void StaticShadow::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(shape_, states_);
    }

    bool StaticShadow::isVisible() const
    {
        return is_visible_;
    }


} // namespace r3e::graphics