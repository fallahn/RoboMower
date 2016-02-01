//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <network/AckSystem.hpp>

using namespace Network;

namespace
{
    sf::Int32 bitIndexInSequence(SeqID sequence, SeqID ack, SeqID maxID)
    {
        return 0;
    }

    sf::Uint32 geenrateAckBits(SeqID ack, const PacketQueue& receivedQueue, SeqID maxID)
    {
        return 0u;
    }

    void processAck(SeqID ack, sf::Uint32 ackBits, PacketQueue& pendingAcks, PacketQueue& ackedQueue,
        std::vector<SeqID>& acks, sf::Uint32& ackPackets, float& rtt, SeqID maxID)
    {

    }
}

AckSystem::AckSystem(SeqID maxID)
{

}

//public
void AckSystem::reset()
{

}

void AckSystem::packetSent(sf::Int32 size)
{

}

void AckSystem::packetReceived(SeqID seqID, sf::Int32 size)
{

}

sf::Uint32 AckSystem::generateAckBits()
{
    return 0;
}

void AckSystem::processAck(SeqID ack, sf::Uint32 ackBits)
{

}

void AckSystem::update(float dt)
{

}

SeqID AckSystem::getLocalSequence() const
{
    return m_localSequence;
}

SeqID AckSystem::getRemoteSequence() const
{
    return m_remoteSequence;
}

SeqID AckSystem::getMaxSequence() const
{
    return m_maxID;
}

const std::vector<SeqID>& AckSystem::getAcks() const
{
    return m_acks;
}

sf::Uint32 AckSystem::getSentPackets() const
{
    return m_sentPackets;
}

sf::Uint32 AckSystem::getReceivedPackets() const
{
    return m_receivedPackets;
}

sf::Uint32 AckSystem::getLostPackets() const
{
    return m_lostPackets;
}

sf::Uint32 AckSystem::getAckedPackets() const
{
    return m_ackedPackets;
}

float AckSystem::getSentBandwidth() const
{
    return m_sentBandwidth;
}

float AckSystem::getAckedBandwidth() const
{
    return m_ackedBandwidth;
}

float AckSystem::getRoundTripTime() const
{
    return m_rtt;
}

sf::Int32 AckSystem::getHeaderSize() const
{
    return 12; //TODO double check this
}
//private
void AckSystem::advanceQueueTime(float dt)
{

}

void AckSystem::updateQueues()
{

}

void AckSystem::updateStats()
{

}