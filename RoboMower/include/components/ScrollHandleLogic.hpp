//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//logic for scroll bar handle

#ifndef SCROLL_HANDLE_LOGIC_HPP_
#define SCROLL_HANDLE_LOGIC_HPP_

#include <xygine/Component.hpp>

class ScrollHandleLogic final : public xy::Component
{
public:
    explicit ScrollHandleLogic(xy::MessageBus&);
    ~ScrollHandleLogic() = default;
    ScrollHandleLogic(const ScrollHandleLogic&) = delete;
    ScrollHandleLogic& operator = (const ScrollHandleLogic&) = delete;

    xy::Component::Type type() const override { return xy::Component::Type::Script; }
    void entityUpdate(xy::Entity&, float) override;
    void handleMessage(const  xy::Message&) override;

    void setCarried(bool);
    bool carried() const;

    void setLength(float);
    //returns position as normalised value of length
    float getPosition() const;

private:

    bool m_carried;
    float m_length;
    float m_position;
};

#endif //SCROLL_HANDLE_LOGIC_HPP_