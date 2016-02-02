//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_ACK_SYSTEM_HPP_
#define RM_ACK_SYSTEM_HPP_

#include <network/PacketQueue.hpp>

#include <vector>

namespace Network
{
    class AckSystem final
    {
    public:
        explicit AckSystem(SeqID maxID = 0xFFFFFFFF);
        ~AckSystem() = default;
        AckSystem(const AckSystem&) = delete;
        AckSystem& operator = (AckSystem&) = delete;

        void reset();
        void packetSent(sf::Int32);
        void packetReceived(SeqID, sf::Int32);
        sf::Uint32 generateAckBits();
        void processAck(SeqID, sf::Uint32);
        void update(float);
        //void validate();

        SeqID getLocalSequence() const;
        SeqID getRemoteSequence() const;
        SeqID getMaxSequence() const;

        const std::vector<SeqID>& getAcks() const;
        sf::Uint32 getSentPackets() const;
        sf::Uint32 getReceivedPackets() const;
        sf::Uint32 getLostPackets() const;
        sf::Uint32 getAckedPackets() const;
        float getSentBandwidth() const;
        float getAckedBandwidth() const;
        float getRoundTripTime() const;
        sf::Int32 getHeaderSize() const;

    private:

        SeqID m_maxID;
        SeqID m_localSequence;
        SeqID m_remoteSequence;

        sf::Uint32 m_sentPackets;
        sf::Uint32 m_receivedPackets;
        sf::Uint32 m_lostPackets;
        sf::Uint32 m_ackedPackets;

        float m_sentBandwidth;
        float m_ackedBandwidth;
        float m_rtt;

        std::vector<SeqID> m_acks;
        PacketQueue m_sentQueue;
        PacketQueue m_pendingAckQueue;
        PacketQueue m_receivedQueue;
        PacketQueue m_ackedQueue;

        void advanceQueueTime(float);
        void updateQueues();
        void updateStats();
    };
}

#endif //RM_ACK_SYSTEM_HPP_