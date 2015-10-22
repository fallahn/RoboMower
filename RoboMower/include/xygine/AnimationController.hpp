//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//controls animations of entities composed of animated drawables

#ifndef ANIMATION_CONTROLLER_HP__
#define ANIMATION_CONTROLLER_HPP_

#include <xygine/Component.hpp>

class AnimatedDrawable;
class AnimationController final : public Component
{
public:
    explicit AnimationController(MessageBus&);
    ~AnimationController() = default;

    Component::Type type() const override;
    Component::UniqueType uniqueType() const override;
    void entityUpdate(Entity&, float) override;
    void handleMessage(const Message&) override;

    void onStart(Entity&) override;

private:

    AnimatedDrawable* m_drawable;
    enum AnimationId //TODO we need to ensure these are properly mapped to loaded animations
    {
        MoveRight,
        MoveLeft,
        MoveUp,
        MoveDown
    };
};


#endif //AIMATION_CONTROLLER_HPP_