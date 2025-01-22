#ifndef FACTORY_HXX
#define FACTORY_HXX

#include "nodes.hxx"


class Factory{
public:
//Dla Ramp
    void add_ramp(Ramp&&){};
    void remove_ramp(ElementID id){};
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id){};
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id){};
    NodeCollection<Ramp>::const_iterator ramp_cbegin() {};
    NodeCollection<Ramp>::const_iterator ramp_cend() {};
//Dla Worker
    void add_worker(Ramp&&){};
    void remove_worker(ElementID id){};
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){};
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id){};
    NodeCollection<Worker>::const_iterator worker_cbegin() {};
    NodeCollection<Worker>::const_iterator worker_cend() {};
//Dla Storehouse
    void add_storehouse(Storehouse&&){};
    void remove_storehouse(ElementID id){};
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id){};
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id){};
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() {};
    NodeCollection<Storehouse>::const_iterator storehouse_cend() {};

    bool is_consistent(){}];
    void do_deliveries(Time t){};
    void do_package_passing(void){};
    void do_work(Time t){};
private:
    void remove_receiver(NodeCollection<Node>& collection, ElementID id){};
};

#endif //FACTORY_HXX
