//
// Created by User on 03.01.2025.
//
#include "nodes.hxx"

using ReceiverPair = std::pair<IPackageReceiver* const, double>;

void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    if (preferences_.empty())
    {
        preferences_[r] = 1.;

    }else
    {
        auto new_prob = 1./static_cast<double>(preferences_.size() + 1);
        for (auto i = preferences_.begin(); i != preferences_.end(); ++i)
        {
            i->second = new_prob;
        }
        //preferences_.insert(preferences_pair(r, new_prob));
        preferences_[r] = new_prob;
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r)
{
    auto to_remove = preferences_.find(r);
    if (to_remove == end())return;
    if (preferences_.empty())return;


    preferences_.erase(to_remove);
    double new_prob = 1./preferences_.size();
    for (auto i = preferences_.begin(); i != preferences_.end(); ++i)
    {
        i->second = new_prob;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    double rn = pg_();
    if (rn < 0 || rn > 1)return nullptr;
    if(preferences_.empty())return nullptr;
    if (rn == 0 )
    {
        return preferences_.begin()->first;
    }
    double prob_sum = 0;
    for (auto i = begin(); i != end(); i++ )
    {
        if (prob_sum < rn )
        {
            prob_sum += i->second;

            if (prob_sum >= rn)
            {
                return i->first;
            }

        }
    }
    return nullptr;
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
void Worker::do_work(Time t){
     if (!buffer_.has_value() && !q_->empty()){
        t_ = t;
        buffer_.emplace(q_->pop());
    }

    if (t - t_ + 1 == pd_) {
        push_package(Package(buffer_.value().get_id()));
        buffer_.reset();
    }
}


void Ramp::deliver_goods(Time t)
{

    if (!buffer_)
    {
        push_package(Package());
        buffer_.emplace(id_);
        t_ = t;
    } else if ((t - t_) % di_== 0)
    {
        push_package(Package());
    }
}

