//
// Created by User on 03.01.2025.
//
#include "nodes.hxx"

void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    double new_prob = 1/preferences_.size() + 1;
    for (auto i = preferences_.begin(); i != preferences_.end(); i++)
    {
        i->second = new_prob;
    }
    preferences_.insert(preferences_pair(r, new_prob));

}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r)
{
    preferences_.erase(r);
    double new_prob = 1/preferences_.size();
    for (auto i = preferences_.begin(); i != preferences_.end(); i++)
    {
        i->second = new_prob;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    double rn = pg_();
    if (rn == 0 )return preferences_.begin()->first;
    double prob_sum = preferences_.begin()->second;
    for (auto i = std::next(begin()); i != end(); i++ )
    {
        if (prob_sum < rn and (prob_sum += i->second) >= rn  )return i->first;
    }
}

void PackageSender::send_package()
{
    if(buffer_.has_value()){
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        (*receiver).receive_package(std::move(*buffer_));
        buffer_.reset();
    }
    else{
        return;
    }
}



void Ramp::deliver_goods(Time t)
{
    if (t%di_ == 0)
    {
        send_package();
    }

}


