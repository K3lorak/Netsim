#include "factory.hxx"
#include <stdexcept>
#include <src/gtest-internal-inl.h>






















































































































































































































































bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors)
{
    if (node_colors[sender] == NodeColor::VERIFIED) return true;

    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty())throw std::logic_error('Sender doesnt have any receivers');

    bool sender_has_different_receiver_than_themself = false;
    for (auto& receiver : sender->receiver_preferences_.get_preferences())
    {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE)
            sender_has_different_receiver_than_themself = true;
        else if (receiver.first->get_receiver_type() == ReceiverType::WORKER)
        {
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender) continue;
            sender_has_different_receiver_than_themself = true;
            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED) has_reachable_storehouse(sendrecv_ptr, node_colors);
        }

    }
    node_colors[sender] = NodeColor::VERIFIED;

    if (sender_has_different_receiver_than_themself)return true;

    throw std::logic_error("Error");
}

bool Factory::is_consistent() const
{
    std::map<const PackageSender*, NodeColor> node_colors ;
    auto set_colors = [&node_colors](const auto container)
    {
        for (const auto& item : container)
        {
            const PackageSender* sender = dynamic_cast<const PackageSender*>(&item);
            node_colors[sender] = NodeColor::UNVISITED;
        }
    };
    set_colors(node_r);
    set_colors(node_w);
    try
    {
        for (auto& ramp : node_r)
        {
            const PackageSender* sender = dynamic_cast<const PackageSender*>(&ramp);
            has_reachable_storehouse(sender, node_colors);
        }
    }catch (std::logic_error&)
    {
        return false;
    }

    return true;
}
