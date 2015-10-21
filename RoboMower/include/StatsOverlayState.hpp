//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#ifndef STATS_OVERLAY_HPP_
#define STATS_OVERLAY_HPP_

#include <State.hpp>
#include <UIWindow.hpp>
#include <UILabel.hpp>

#include <vector>

class StatsOverlayState final : public State
{
public:
    StatsOverlayState(StateStack&, Context);
    ~StatsOverlayState(); //hide cursor

    bool update(float) override;
    void draw() override;
    bool handleEvent(const sf::Event&) override;
    void handleMessage(const Message&) override;
    States::ID stateID() const override
    {
        return States::ID::Stats;
    }
private:
    std::vector<ui::Window::Ptr> m_windows;
    ui::Label::Ptr m_statsText;
    ui::Label::Ptr m_consoleText;

};
#endif //STATS_OVERLAY_HPP_