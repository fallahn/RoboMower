//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <network/FlowControl.hpp>

using namespace Network;

namespace
{
    const float GOOD = 30.f;
    const float BAD = 10.f;
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

}

float FlowControl::getSendRate() const
{
    return m_sendRate;
}