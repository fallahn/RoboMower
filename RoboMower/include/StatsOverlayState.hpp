//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef STATS_OVERLAY_HPP_
#define STATS_OVERLAY_HPP_

#include <StateIds.hpp>

#include <xygine/State.hpp>
#include <xygine/ui/Window.hpp>
#include <xygine/ui/Label.hpp>
#include <xygine/Resource.hpp>

#include <vector>

class StatsOverlayState final : public xy::State
{
public:
    StatsOverlayState(xy::StateStack&, Context);
    ~StatsOverlayState(); //hide cursor

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const xy::Message&) override;
    xy::StateId stateID() const override
    {
        return States::ID::Stats;
    }
private:
    std::vector<xy::UI::Window::Ptr> m_windows;
    xy::UI::Label::Ptr m_statsText;
    xy::UI::Label::Ptr m_consoleText;

    xy::FontResource m_fontResource;
};

#endif //STATS_OVERLAY_HPP_