//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

//allows classes to report data to a single point

#ifndef REPORTS_HPP_
#define REPORTS_HPP_

#ifdef _DEBUG_
#define REPORT(name, value) StatsReporter::reporter.report(name, value)
#else
#define REPORT(name, value)
#endif //_DEBUG_

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>

#include <string>
#include <unordered_map>

class StatsReporter final
{
public:
    StatsReporter();
    ~StatsReporter() = default;
    StatsReporter(const StatsReporter&) = delete;
    StatsReporter& operator = (const StatsReporter&) = delete;

    void report(const std::string& name, const std::string& value);
    void remove(const std::string& name);

    const std::string& getString();

    static StatsReporter reporter;

private:
    std::unordered_map<std::string, std::string> m_data;
    std::string m_string;
    bool m_rebuildString;

    sf::Mutex m_mutex;
};
#endif //REPORTS_HPP_
