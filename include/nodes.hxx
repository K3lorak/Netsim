//
// Created by User on 03.01.2025.
//

#ifndef NODES_HXX
#define NODES_HXX
#include "storage_types.hxx"
#include <memory>

class IPackageReceiver
{
    public:
        virtual void receive_package(Package&&) = 0;
        virtual  ElementID get_id() const = 0;
};

class Storehouse:public IPackageReceiver
{
    public:
        Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d):id_(id), d_(d) {}
        void receive_package(Package&&) override{}
        ElementID get_id() const override {return  id_;}
    private:
        ElementID id_;
        std::unique_ptr<IPackageStockpile> d_;
};

class Worker:public PackageSender, public IPackageReceiver
{
    public:
        Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q):
          id_(id), pd_(pd), q_(q){}
        void do_work(Time t){}
        TimeOffset get_processing_duration() const {}
        Time get_package_processing_start_time() const {}
    private:
        ElementID id_;
        TimeOffset pd_;
        std::unique_ptr<IPackageQueue> q_;

}



#include <cstdlib>
#include <optional>
#include "types.hxx"
#include "package.hxx"
#include "storage_types.hxx"

class Ramp{
    public:
        Ramp(ElementID id,TimeOffset di);
        void deliver_goods(Time t);
        TimeOffset get_delivery_interval() const;
        ElementID det_id(void) const;
};

class ReceiverPreferences{
    public:
        using preferences_t = std::map<IPackageReceiver*, double>;
        using const_iterator = preferences_t::const_iterator;

        ReceiverPreferences(ProbabilityGenerator pg);
        void add_receiver(IPackageReceiver* r);
        void add_receiver(IPackageReceiver* r);
        void remove_receiver(IPackageReceiver* r);
        IPackageReceiver* choose_receiver(void);
        preferences_t& get_preferences(void) const;
};

class PackageSender: public IPackageReceiver{
    public:
        PackageSender(PackageSender&&);
        ReceiverPreferences receiver_preferences_();
        void send_package(void);
        std::optional<Package>& get_sending_buffer(void) const;
    protected:
        void push_package(Package&&);
};

#endif //NODES_HXX
