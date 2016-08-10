/*-----------------------------------------------------------------------

Matt Marchant 2015 - 2016
http://trederia.blogspot.com

Robomower - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/

#ifndef RM_PACKET_ENUMS_HPP_
#define RM_PACKET_ENUMS_HPP_

#include <SFML/Network/Packet.hpp>

enum class TransportStatus : sf::Uint8
{
    Stopped,
    Playing,
    Paused
};

enum class TransportChange : sf::Uint8
{
    Play,
    Pause,
    Rewind
};

enum class Direction : sf::Uint8
{
    Left = 0,
    Up,
    Right,
    Down,
    Count
};

enum class ProgramState : sf::Uint8
{
    Finished = 0,
    Rewound
};

sf::Packet& operator << (sf::Packet& p, TransportStatus ts);
sf::Packet& operator >> (sf::Packet& p, TransportStatus& ts);

sf::Packet& operator << (sf::Packet& p, TransportChange tc);
sf::Packet& operator >> (sf::Packet& p, TransportChange& ts);

sf::Packet& operator << (sf::Packet&, Direction);
sf::Packet& operator >> (sf::Packet&, Direction&);

sf::Packet& operator << (sf::Packet&, ProgramState);
sf::Packet& operator >> (sf::Packet&, ProgramState&);

#endif //RM_PACKET_ENUMS_HPP_