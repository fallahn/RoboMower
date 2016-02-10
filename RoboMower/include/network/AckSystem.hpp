//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_ACK_SYSTEM_HPP_
#define RM_ACK_SYSTEM_HPP_

#include <network/PacketQueue.hpp>

#include <SFML/Network/Packet.hpp>

#include <vector>

namespace Network
{
    class AckSystem final
    {
    public:
        struct Header final
        {
            SeqID sequence = 1;
            SeqID ack = 0;
            sf::Uint32 ackBits = 0;
        };

        explicit AckSystem(SeqID maxID = 0xFFFF);
        ~AckSystem() = default;
        AckSystem(const AckSystem&) = delete;
        AckSystem& operator = (AckSystem&) = delete;

        void reset();
        void packetSent(sf::Int32);
        void packetReceived(const Header&, sf::Int32);
       
        void update(float);

        SeqID getLocalSequence() const;
        SeqID getRemoteSequence() const;
        SeqID getMaxSequence() const;

        const std::vector<SeqID>& getAcks() const;
        sf::Uint32 getSentPacketCount() const;
        sf::Uint32 getReceivedPacketCount() const;
        sf::Uint32 getLostPacketCount() const;
        sf::Uint32 getAckedPacketCount() const;
        float getSentBandwidth() const;
        float getAckedBandwidth() const;
        float getRoundTripTime() const;

        Header createHeader();
    private:

        SeqID m_maxID;
        SeqID m_localSequence;
        SeqID m_remoteSequence;

        sf::Uint32 m_sentPacketCount;
        sf::Uint32 m_receivedPacketCount;
        sf::Uint32 m_lostPacketCount;
        sf::Uint32 m_ackedPacketCount;

        float m_sentBandwidth;
        float m_ackedBandwidth;
        float m_rtt;

        std::vector<SeqID> m_acks;
        PacketQueue m_sentQueue;
        PacketQueue m_pendingAckQueue;
        PacketQueue m_receivedQueue;
        PacketQueue m_ackedQueue;

        void processAck(SeqID, sf::Uint32);
        void advanceQueueTime(float);
        void updateQueues();
        void updateStats();
    };
}

sf::Packet& operator <<(sf::Packet& packet, const Network::AckSystem::Header& header);

sf::Packet& operator >>(sf::Packet& packet, Network::AckSystem::Header& header);

#endif //RM_ACK_SYSTEM_HPP_