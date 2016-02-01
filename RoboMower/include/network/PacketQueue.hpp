//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_PACKET_Q_HPP_
#define RM_PACKET_Q_HPP_

//TODO bear in mind that if this ever becomes a bottleneck
//it should be properly implemented as a circular buffer

#include <SFML/Config.hpp>

#include <list>

namespace Network
{
    using SeqID = sf::Uint32;
    struct PacketData final
    {
        SeqID sequence; //TODO as long as wrap around works we can save some bw making this a short
        sf::Int32 timeOffset; //offset in milliseconds
        sf::Int32 size;
    };

    class PacketQueue final : std::list<PacketData>
    {
    public:
        PacketQueue() = default;
        ~PacketQueue() = default;
        PacketQueue(const PacketQueue&) = delete;
        PacketQueue& operator = (const PacketQueue&) = delete;

        bool exists(SeqID);
        void insertSorted(const PacketData&, SeqID);
        void verifySorted(SeqID);

    private:

    };
}

#endif //RM_PACKET_Q_HPP_