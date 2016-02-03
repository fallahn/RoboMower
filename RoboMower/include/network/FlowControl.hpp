//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_FLOW_CONTROL_HPP_
#define RM_FLOW_CONTROL_HPP_

namespace Network
{
    class FlowControl final
    {
    public:
        FlowControl();
        ~FlowControl() = default;
        FlowControl(const FlowControl&) = delete;
        FlowControl& operator = (const FlowControl&) = delete;

        void reset();
        void update(float dt, float rtt);
        float getSendRate() const;

    private:
        float m_sendRate;
        float m_penaltyTime;
        float m_goodConditionTime;
        float m_penaltyReductionAccumulator;
    };
}

#endif //RM_FLOW_CONTROL_HPP_