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

#ifndef RM_NET_PROTOCOL_HPP_
#define RM_NET_PROTOCOL_HPP_

#include <xygine/network/Config.hpp>

enum PacketIdent
{
    //client id, name
    PlayerDetails = xy::PacketID(xy::Network::PacketType::Count),
    //count, client id, position
    PositionUpdate,
    //clientId, direction
    DirectionUpdate,
    //clientID, size, bytestream
    TransmitProgram,
    //transport state
    TransportStateChanged,
    //clientID, transport state
    TransportRequestChange,
    //action
    ProgramStatus
};

sf::Packet& operator << (sf::Packet&, PacketIdent);
sf::Packet& operator >> (sf::Packet&, PacketIdent&);

#endif //RM_NET_PROTOCOL_HPP_
