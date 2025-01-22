#include "factory.hxx"
#include "nodes.hxx"

bool Factory::is_consistent() const{
  return 0
}

void Factory::do_deliveries(Time t){
  for (auto &ramp : cont_r)
        ramp.deliver_goods(t);
}

void Factory::do_package_passing(void){
  for (auto &ramp : cont_r)
        ramp.send_package();

    for (auto &worker : cont_w)
        worker.send_package();
}

void Factory::do_work(Time t){
  for(auto &ramp : const_w)
    worker.do_work(t)
}

void Factory::remove_worker(ElementID id){
  Worker* node = &(*cont_w.find_by_id(id));

  std::for_each(cont_w.begin(), cont_w.end(), [node](Worker& worker) {
        worker.receiver_preferences_.remove_receiver(node);
    });

  std::for_each(cont_r.begin(), cont_r.end(), [node](Ramp& ramp) {
        ramp.receiver_preferences_.remove_receiver(node);
    });

  cont_w.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id)
{
    Storehouse* node = &(*cont_s.find_by_id(id));
    std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& ramp) {
        ramp.receiver_preferences_.remove_receiver(node);
    });

    std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& worker) {
        worker.receiver_preferences_.remove_receiver(node);
    });
    cont_s.remove_by_id(id);
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
};

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
