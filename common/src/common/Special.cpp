//
// Created by jul3x on 19.03.20.
//

#include <common/ResourceManager.h>
#include <common/Special.h>


Special::Special(const sf::Vector2f& position, const std::string& id, int u_id) :
        Special(position, id,
                utils::j3x::get<std::string>(RM.getObjectParams("specials", id), "default_activation"),
                utils::j3x::get<std::vector<std::string>>(RM.getObjectParams("specials", id), "default_functions"),
                utils::j3x::get<std::vector<std::string>>(RM.getObjectParams("specials", id), "default_datas"),
                utils::j3x::get<int>(RM.getObjectParams("specials", id), "default_active"), u_id)
{

}


Special::Special(const sf::Vector2f& position, const std::string& id,
                 const std::string& activation, const std::vector<std::string>& functions,
                 const std::vector<std::string>& datas, bool is_active, int u_id) :
        HoveringObject(position, {},
                       {utils::j3x::get<float>(RM.getObjectParams("specials", id), "size_x"),
                        utils::j3x::get<float>(RM.getObjectParams("specials", id), "size_y")},
                       collision::Circle(utils::j3x::get<float>(RM.getObjectParams("specials", id), "collision_size_x"),
                                         {utils::j3x::get<float>(RM.getObjectParams("specials", id), "collision_offset_x"),
                                          utils::j3x::get<float>(RM.getObjectParams("specials", id), "collision_offset_y")}),
                       &RM.getTexture("specials/" + id),
                       utils::j3x::get<int>(RM.getObjectParams("specials", id), "z_index"),
                       utils::j3x::get<int>(RM.getObjectParams("specials", id), "frames_number"),
                       utils::j3x::get<float>(RM.getObjectParams("specials", id), "frame_duration"),
                       0.0f),
        Functional(activation, functions, datas, id, u_id),
        is_drawable_(utils::j3x::get<int>(RM.getObjectParams("specials", id), "is_drawable")),
        additional_boolean_data_(false)
{
    this->changeOrigin(sf::Vector2f(utils::j3x::get<float>(RM.getObjectParams("specials", id), "size_x"),
                                    utils::j3x::get<float>(RM.getObjectParams("specials", id), "size_y")) / 2.0f +
                       sf::Vector2f(utils::j3x::get<float>(RM.getObjectParams("specials", id), "map_offset_x"),
                                    utils::j3x::get<float>(RM.getObjectParams("specials", id), "map_offset_y")));

    if (is_active)
        this->activate();
    else
        this->deactivate();

    if (utils::j3x::get<int>(RM.getObjectParams("specials", id), "light_point"))
    {
        float light_size = CFG.get<float>("graphics/specials_light_point_size") * CFG.get<float>("graphics/global_zoom");
        light_ = std::make_unique<graphics::LightPoint>(this->getPosition(),
                                                        sf::Vector2f{light_size, light_size},
                                                        &RM.getTexture("lightpoint"));
    }
}

bool Special::isDrawable() const
{
    return is_drawable_;
}

bool Special::getAdditionalBooleanData() const
{
    return additional_boolean_data_;
}

void Special::setAdditionalBooleanData(bool data)
{
    additional_boolean_data_ = data;
}

graphics::LightPoint* Special::getLightPoint() const
{
    return light_.get();
}
