//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <xygine/Reports.hpp>

#include <cassert>
#include <algorithm>

namespace
{
    const std::string interweebl(": ");
}

StatsReporter::StatsReporter()
    : m_rebuildString(true)
{

}

//public
void StatsReporter::report(const std::string& name, const std::string& value)
{
    assert(!name.empty() && !value.empty());
    sf::Lock lock(m_mutex);
    m_data[name] = value;
    m_rebuildString = true;
}

void StatsReporter::remove(const std::string& name)
{
    sf::Lock lock(m_mutex);
    m_data.erase(name);
    m_rebuildString = true;
}

const std::string& StatsReporter::getString()
{
    sf::Lock lock(m_mutex);
    if (m_rebuildString)
    {
        m_rebuildString = false;

        m_string.clear();
        for (const auto& p : m_data)
        {
            m_string.append(p.first);
            m_string.append(interweebl);
            m_string.append(p.second);
            m_string.append("\n");
        }
    }

    return m_string;
}

StatsReporter StatsReporter::reporter;