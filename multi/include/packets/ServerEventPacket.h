//
// Created by jul3x on 25.03.2021.
//

#ifndef RAG3_MULTI_INCLUDE_PACKETS_SERVEREVENTPACKET_H
#define RAG3_MULTI_INCLUDE_PACKETS_SERVEREVENTPACKET_H

#include <set>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>


class ServerEventPacket : public sf::Packet {
public:
    enum class Type {
        NameChange = 5,
        DestroyedObstacle = 4,
        CollectedObject = 3,
        EnteredObject = 2,
        UseObject = 1,
        Exit = -1,
        None = 0
    };

    static constexpr auto STRING_DATA_ABOVE = 4;

    ServerEventPacket() = default;

    ServerEventPacket(Type type, int uid, sf::Uint32 player_ip)
    {
        *this << static_cast<sf::Uint16>(type) << player_ip << uid;
    }

    ServerEventPacket(Type type, const j3x::Parameters& data, sf::Uint32 player_ip)
    {
        *this << static_cast<sf::Uint16>(type) << player_ip;

        std::string params;
        for (const auto& param : data)
            r3e::j3x::serializeAssign(param.first, param.second, params);
        *this << params;
    }

    [[nodiscard]] Type getType() const
    {
        return type_;
    }

    [[nodiscard]] int getUID() const
    {
        return uid_;
    }

    [[nodiscard]] const j3x::Parameters& getParams() const
    {
        return *data_;
    }

    [[nodiscard]] sf::Uint32 getIP() const
    {
        return player_ip_;
    }

private:
    void onReceive(const void* data, std::size_t size) override
    {
        append(data, size);
        sf::Uint16 type;
        *this >> type >> player_ip_;

        if (type > STRING_DATA_ABOVE)
        {
            std::string params;
            *this >> params;
            data_ = r3e::j3x::parseContent(params);
        }
        else
            *this >> uid_;
        type_ = static_cast<Type>(type);
    }

    Type type_{Type::None};
    int uid_{-1};
    sf::Uint32 player_ip_{0};
    std::shared_ptr<j3x::Parameters> data_;

};

#endif //RAG3_MULTI_INCLUDE_PACKETS_SERVEREVENTPACKET_H
