//
// Created by User on 03.01.2025.
//
#include "nodes.hxx"

void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    double new_prop = 1/preferences_.size() + 1;
    for (auto i = preferences_.begin(); i != preferences_.end(); i++)
    {
        i->second = new_prop;
    }
    preferences_.insert(preferences_pair(r, new_prop));

}

IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    double rn = pg_();
    if (rn == 0 )return preferences_.begin()->first;
    for (auto i = begin(); i != end(); i++ )
    {
        double prob_sum = i->second;

        if (rn > 0 );
    }

}

