//
// Created by User on 03.01.2025.
//

#ifndef NODES_HXX
#define NODES_HXX

#include "types.hxx"
#include "package.hxx"
#include "storage_types.hxx"
#include "helpers.hxx"
#include <cstdlib>
#include <optional>
#include <memory>
#include <map>


class IPackageReceiver
{
    public:
        using ConstIterator = std::list<Package>::const_iterator;

        virtual void receive_package(Package&&) = 0;
        virtual  ElementID get_id() const = 0;


        virtual ConstIterator begin() const = 0;
        virtual ConstIterator end() const = 0;
        virtual ConstIterator cbegin() const = 0;
        virtual ConstIterator cend() const = 0;
};

class Storehouse:public IPackageReceiver
{
    public:
        Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)):id_(id), d_(std::move(d)) {}
        void receive_package(Package&&) override{}
        ElementID get_id() const override {return  id_;}
    private:
        ElementID id_;
        std::unique_ptr<IPackageStockpile> d_;
};

class ReceiverPreferences{
    public:
        using preferences_t = std::map<IPackageReceiver*, double>;
        using preferences_pair = std::pair<IPackageReceiver*, double>;
        using ConstIterator = preferences_t::const_iterator;

        ReceiverPreferences(ProbabilityGenerator pg = probability_generator ):pg_(pg){};
        void add_receiver(IPackageReceiver* r);
        void remove_receiver(IPackageReceiver* r);
        IPackageReceiver* choose_receiver(void);
        preferences_t& get_preferences(void) {return preferences_;}
    //usunąlem const z linijki wyzej bo z nim byly bledy

        ConstIterator begin() {return preferences_.begin();};
        ConstIterator end() {return preferences_.end();};
        ConstIterator cbegin() {return preferences_.cbegin();};
        ConstIterator cend() {return preferences_.cend();};
    private:
        preferences_t preferences_;
    //obiekt funkcyjny zwracajacy losowa wartosc
        ProbabilityGenerator pg_;
};

class PackageSender: public IPackageReceiver{
    public:
        PackageSender() = default;
        PackageSender(PackageSender&&) = default;
        ReceiverPreferences receiver_preferences_();
        void send_package(void);
        std::optional<Package>& get_sending_buffer(void) const;
    protected:
        void push_package(Package&&);
};

class Ramp: public PackageSender{
    public:
        Ramp(ElementID id,TimeOffset di);
        void deliver_goods(Time t);
        TimeOffset get_delivery_interval() const;
        ElementID det_id(void) const;
};

class Worker:public PackageSender, public IPackageReceiver
{
    public:
        Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)){}
        void do_work(Time t){}
        TimeOffset get_processing_duration() const {}
        Time get_package_processing_start_time() const {}
    private:
        ElementID id_;
        TimeOffset pd_;
        std::unique_ptr<IPackageQueue> q_;
};

#endif //NODES_HXX
