//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <network/FlowControl.hpp>

#include <xygine/Log.hpp>

using namespace Network;

namespace
{
    const float GOOD = 30.f;
    const float BAD = 10.f;
    const float RTT_Threshold = 250.0f;
}

FlowControl::FlowControl()
    : m_sendRate                    (BAD),
    m_penaltyTime                   (4.f),
    m_goodConditionTime             (0.f),
    m_penaltyReductionAccumulator   (0.f) {}

//public
void FlowControl::reset()
{
    m_sendRate = BAD;
    m_penaltyTime = 4.f;
    m_goodConditionTime = 0.f;
    m_penaltyReductionAccumulator = 0.f;
}

void FlowControl::update(float dt, float rtt)
{
    if (m_sendRate == GOOD)
    {
        if (rtt > RTT_Threshold)
        {
            LOG("Flow Control: decreasing send rate", xy::Logger::Type::Info);
            m_sendRate = BAD;
            if (m_goodConditionTime < 10.f && m_penaltyTime < 60.f)
            {
                m_penaltyTime *= 2.f;
                if (m_penaltyTime > 60.f) m_penaltyTime = 60.f;
            }
            m_goodConditionTime = 0.f;
            m_penaltyReductionAccumulator = 0.f;
            return;
        }

        m_goodConditionTime += dt;
        m_penaltyReductionAccumulator += dt;

        if (m_penaltyReductionAccumulator > 10.f && m_penaltyTime > 1.f)
        {
            m_penaltyTime /= 2.f;
            if (m_penaltyTime < 1.f) m_penaltyTime = 1.f;

            m_penaltyReductionAccumulator = 0.f;
        }
    }

    if (m_sendRate == BAD)
    {
        if (rtt <= RTT_Threshold)
        {
            m_goodConditionTime += dt;
        }
        else
        {
            m_goodConditionTime = 0.f;
        }

        if (m_goodConditionTime > m_penaltyTime)
        {
            LOG("Flow Control: Increasing send rate", xy::Logger::Type::Info);
            m_goodConditionTime = 0.f;
            m_penaltyReductionAccumulator = 0.f;
            m_sendRate = GOOD;
            return;
        }
    }
}

float FlowControl::getSendRate() const
{
    return m_sendRate;
}