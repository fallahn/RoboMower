//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015 - 2016
//==============================================================================

#include <network/PacketQueue.hpp>

#include <algorithm>

using namespace Network;

bool PacketQueue::exists(SeqID sequenceID)
{
    return std::find_if(begin(), end(), 
        [sequenceID](const PacketData& pd)
    {
        return pd.sequence == sequenceID;
    }) != end();
}

void PacketQueue::insertSorted(const PacketData& pd, SeqID maxID)
{
    if (empty())
    {
        push_back(pd);
    }
    else
    {
        if (!moreRecent(pd.sequence, front().sequence, maxID))
        {
            push_front(pd);
        }
        else if (moreRecent(pd.sequence, back().sequence, maxID))
        {
            push_back(pd);
        }
        else
        {
            for (auto it = begin(); it != end(); ++it)
            {
                if (moreRecent(it->sequence, pd.sequence, maxID))
                {
                    insert(it, pd);
                    break;
                }
            }
        }
    }

}

void PacketQueue::verifySorted(SeqID maxID)
{

}