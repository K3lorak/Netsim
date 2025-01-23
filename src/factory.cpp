#include "factory.hxx"
#include "nodes.hxx"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors)
{
    if (node_colors[sender] == NodeColor::VERIFIED) return true;

    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty())throw std::logic_error("Sender doesnt have any receivers");

    //bool sender_has_different_receiver_than_themself = false;
    for (auto& receiver : sender->receiver_preferences_.get_preferences())
    {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE)
            return true;
            //sender_has_different_receiver_than_themself = true;
        else if (receiver.first->get_receiver_type() == ReceiverType::WORKER)
        {
            auto receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<class Worker*>(receiver_ptr);
            if (worker_ptr == nullptr) continue;
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if (sendrecv_ptr == nullptr) continue;

            if (sendrecv_ptr == sender) continue;
            //sender_has_different_receiver_than_themself = true;
            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED && has_reachable_storehouse(sendrecv_ptr, node_colors))
                return true;
        }

    }
    node_colors[sender] = NodeColor::VERIFIED;

    //if (sender_has_different_receiver_than_themself)return true;

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
            if (sender == nullptr) continue;
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
            if (sender == nullptr) continue;
            has_reachable_storehouse(sender, node_colors);
        }
    }catch (const std::logic_error&)
    {
        return false;
    }

    return true;
}

void Factory::do_deliveries(Time t){
  for (auto &ramp : node_r)
        ramp.deliver_goods(t);
}

void Factory::do_package_passing(void){
  for (auto &ramp : node_r)
        ramp.send_package();

    for (auto &worker : node_w)
        worker.send_package();
}

void Factory::do_work(Time t){
  for(auto &worker : node_w)
    worker.do_work(t);
}

void Factory::remove_worker(ElementID id){
  Worker* node = &(*node_w.find_by_id(id));

  std::for_each(node_w.begin(), node_w.end(), [node](Worker& worker) {
        worker.receiver_preferences_.remove_receiver(node);
    });

  std::for_each(node_r.begin(), node_r.end(), [node](Ramp& ramp) {
        ramp.receiver_preferences_.remove_receiver(node);
    });

  node_w.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id)
{
    Storehouse* node = &(*node_s.find_by_id(id));
    std::for_each(node_w.begin(), node_w.end(), [&node](Worker& ramp) {
        ramp.receiver_preferences_.remove_receiver(node);
    });

    std::for_each(node_w.begin(), node_w.end(), [&node](Worker& worker) {
        worker.receiver_preferences_.remove_receiver(node);
    });
    node_s.remove_by_id(id);
}

std::vector<std::string> character_split(const std::string& splittable_str, char delimiter) {
    std::stringstream parameter_stream(splittable_str);
    std::string part;
    std::vector<std::string> result;

    while(std::getline(parameter_stream, part, delimiter)) {
        result.push_back(part);
    }

    return result;
}

ParsedLineData parse_line(std::string& line){

  std::vector<std::string> tokens;
  std::string token;

  std::istringstream token_stream(line);
  char delimiter = ' ';

  while (std::getline(token_stream, token, delimiter))
        tokens.push_back(token);

  ParsedLineData parsed_data;

  std::map<std::string, ElementType> element_types{
            {"RAMP", ElementType::RAMP},
            {"WORKER", ElementType::WORKER},
            {"STOREHOUSE", ElementType::STOREHOUSE},
            {"LINK", ElementType::LINK},
    };

    try {
        parsed_data.element_type = element_types.at(tokens[0]);

        std::for_each(std::next(tokens.cbegin()), tokens.cend(), [&](const std::string& parameter_str) {
            auto key_value = character_split(parameter_str, '=');
            parsed_data.parameters[key_value[0]] = key_value[1];
        });
    } catch (std::out_of_range& ex) {
        throw std::exception();
    }

    return parsed_data;
}

PackageQueueType get_package_queue_type(std::string& package_queue_type_str) {
    std::map<std::string, PackageQueueType> str_type_map{
            {"LIFO", PackageQueueType::LIFO},
            {"FIFO", PackageQueueType::FIFO}
    };

    return str_type_map.at(package_queue_type_str);
}

void link(Factory& factory, const std::map<std::string, std::string>& parameters) {
    enum class NodeType {
        RAMP, WORKER, STORE
    };

    std::map<std::string, NodeType> str_node_type{
            {"ramp", NodeType::RAMP},
            {"worker", NodeType::WORKER},
            {"store", NodeType::STORE}
    };

    std::string src_str = parameters.at("src");
    std::string dest_str = parameters.at("dest");

    auto src_param = character_split(src_str, '-');
    NodeType src_node_t = str_node_type.at(src_param[0]);
    ElementID src_node_id = std::stoi(src_param[1]);

    auto dest_param = character_split(dest_str, '-');
    NodeType dest_node_t = str_node_type.at(dest_param[0]);
    ElementID dest_node_id = std::stoi(dest_param[1]);

    IPackageReceiver* package_receiver = nullptr;

    switch(dest_node_t) {
        case NodeType::RAMP:
            break;
        case NodeType::WORKER:
            package_receiver = &*factory.find_worker_by_id(dest_node_id);
            break;
        case NodeType::STORE: {
            package_receiver = &*factory.find_storehouse_by_id(dest_node_id);
            break;
        }
    }

    switch(src_node_t) {
        case NodeType::RAMP: {
            factory.find_ramp_by_id(src_node_id)->receiver_preferences_.add_receiver(package_receiver);
            break;
        }
        case NodeType::WORKER: {
            factory.find_worker_by_id(src_node_id)->receiver_preferences_.add_receiver(package_receiver);
            break;
        }
        case NodeType::STORE:
            break;
    }
}
Factory load_factory_structure(std::istream& is){
    Factory factory;

    std::string line;

    while (std::getline(is, line)) {
        if (line.empty() || line[0] == ';')
            continue;

        ParsedLineData parsed = parse_line(line);

        switch(parsed.element_type) {
            case ElementType::RAMP: {
                ElementID element_id = std::stoi(parsed.parameters.at("id"));
                TimeOffset delivery_interval = std::stoi(parsed.parameters.at("delivery-interval"));
                Ramp ramp(element_id, delivery_interval);

                factory.add_ramp(std::move(ramp));
                break;
            }
            case ElementType::WORKER: {
                ElementID element_id = std::stoi(parsed.parameters.at("id"));
                TimeOffset processing_time = std::stoi(parsed.parameters.at("processing-time"));
                PackageQueueType package_queue_t = get_package_queue_type(parsed.parameters.at("queue-type"));
                Worker worker(element_id, processing_time, std::make_unique<PackageQueue>(package_queue_t));

                factory.add_worker(std::move(worker));
                break;
            }
            case ElementType::STOREHOUSE: {
                ElementID element_id = std::stoi(parsed.parameters.at("id"));
                Storehouse storehouse(element_id);

                factory.add_storehouse(std::move(storehouse));
                break;
            }
            case ElementType::LINK: {
                link(factory, parsed.parameters);
                break;
            }
        }
    }

    return factory;
}
std::string queue_type_str(PackageQueueType package_queue_type) {
    switch(package_queue_type) {
        case PackageQueueType::FIFO:
            return "FIFO";
        case PackageQueueType::LIFO:
            return "LIFO";
    }
    return {};
}

void link_stream_fill(std::stringstream& link_stream, const PackageSender& package_sender, ElementID package_sender_id, std::string&& package_sender_name) {
    auto prefs = package_sender.receiver_preferences_.get_preferences();

    std::for_each(prefs.cbegin(), prefs.cend(), [&](const std::pair<IPackageReceiver*, double>& key_value) {
        link_stream << "LINK src=" << package_sender_name << "-" << package_sender_id << " ";
        const IPackageReceiver* package_receiver = key_value.first;
        ReceiverType receiver_type = package_receiver->get_receiver_type();

        std::string receiver_type_str = receiver_type == ReceiverType::WORKER ? "worker" : "store";

        link_stream << "dest=" << receiver_type_str << "-" << package_receiver->get_id() << '\n';
        std::cout << link_stream.str();
    });
}

void save_factory_structure(Factory& factory, std::ostream& os) {

    std::stringstream link_stream;

    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&](const Ramp& ramp) {
        ElementID ramp_id = ramp.get_id();
        os << "RAMP id=" << ramp_id << ' '
           << "delivery-interval=" << ramp.get_delivery_interval() << '\n';

        link_stream_fill(link_stream, ramp, ramp_id, "ramp");
    });

    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&](const Worker& worker) {
        PackageQueueType queue_type = worker.get_queue()->get_queue_type();
        ElementID worker_id = worker.get_id();
        os << "WORKER id=" << worker_id << ' '
           << "processing-time=" << worker.get_processing_duration() << ' '
           << "queue-type=" << queue_type_str(queue_type) << '\n';

        link_stream_fill(link_stream, worker, worker_id, "worker");
    });

    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&](const Storehouse& storehouse) {
        os << "STOREHOUSE id=" << storehouse.get_id() << '\n';
    });

    os << link_stream.str();

    os.flush();
}
