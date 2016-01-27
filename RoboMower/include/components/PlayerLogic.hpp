//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#ifndef RM_PLAYER_LOGIC_HPP_
#define RM_PLAYER_LOGIC_HPP_

#include <xygine/components/Component.hpp>

class PlayerLogic final : public xy::Component
{
public:
    explicit PlayerLogic(xy::MessageBus&);
    ~PlayerLogic() = default;

    xy::Component::Type type() const override { return xy::Component::Type::Script; }
    void entityUpdate(xy::Entity&, float) override;

private:

    std::size_t m_targetIdx;
};

#endif //RM_PLAYER_LOGIC_HPP_