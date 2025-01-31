#ifndef FACTORY_HXX
#define FACTORY_HXX

#include "storage_types.hxx"
#include "nodes.hxx"
#include <algorithm>

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);

template<class Node>
class NodeCollection{
public:
    //aliasy na typy iteratorów - ze skryptu Factory + jako kontener dałem listę
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node) { c_.emplace_back(std::move(node)); }
    void remove_by_id(ElementID id) { c_.remove_if([id](const Node& elem) { return elem.get_id() == id; }); };
    NodeCollection<Node>::iterator find_by_id(ElementID id) {
        auto it = std::find_if(c_.begin(), c_.end(), [id](const Node& elem) {
            return elem.get_id() == id;
        });
        if(it == c_.end()){
            return std::next(it);
        }
        return it;
    }

    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {
        auto it = std::find_if(c_.begin(), c_.end(), [id](const Node& elem) {
            return elem.get_id() == id;
        });
        if(it == c_.end()){
            return std::next(it);
        }
        return it;
    }

    iterator begin() { return c_.begin(); }
    iterator end() { return c_.end(); }
    const_iterator begin() const { return c_.cbegin(); }
    const_iterator end() const { return c_.cend(); }
    const_iterator cbegin() const { return c_.cbegin(); }
    const_iterator cend() const { return c_.cend(); }
private:
    container_t c_;
};

class Factory{
public:
//Dla Ramp
    void add_ramp(Ramp&& r){node_r.add(std::move(r));}
    void remove_ramp(ElementID id){node_r.remove_by_id(id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id){return node_r.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const{return node_r.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const{return node_r.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() const{return node_r.cend();}
//Dla Worker
    void add_worker(Worker&& w){node_w.add(std::move(w));}
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){return node_w.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const{return node_w.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin() const{return node_w.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() const{return node_w.cend();}
//Dla Storehouse
    void add_storehouse(Storehouse&& s){node_s.add(std::move(s));}
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id){return node_s.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const{return node_s.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const{return node_s.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const{return node_s.cend();}

    bool is_consistent() const;
    void do_deliveries(Time);
    void do_package_passing(void);
    void do_work(Time);
private:
    template<class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);

    NodeCollection<Ramp> node_r;
    NodeCollection<Worker> node_w;
    NodeCollection<Storehouse> node_s;
};

template<class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {

    auto iter = collection.find_by_id(id);

    auto receiver_ptr = dynamic_cast<IPackageReceiver*>(iter);

    for (auto& ramp: node_r) {
        auto& _preferences = ramp.receiver_preferences_.get_preferences();
        for (auto _preference: _preferences) {
            if (_preference.first == receiver_ptr) {
                ramp.receiver_preferences_.remove_receiver(receiver_ptr);
                break;
            }
        }
    }

    for (auto& worker: node_w) {
        auto& _preferences = worker.receiver_preferences_.get_preferences();
        for (auto _preference: _preferences) {
            if (_preference.first == receiver_ptr) {
                worker.receiver_preferences_.remove_receiver(receiver_ptr);
                break;
            }
        }
    }
}


enum class ElementType {
    RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLineData {
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

ParsedLineData parse_line(std::string& line);
Factory load_factory_structure(std::istream& is);
void save_factory_structure(Factory& factory, std::ostream& os);

#endif //FACTORY_HXX