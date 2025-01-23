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


enum class ReceiverType
{
    WORKER,
    STOREHOUSE
};

enum class NodeColor
{
    UNVISITED, VISITED, VERIFIED
};

class IPackageReceiver
{
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void receive_package(Package&&) = 0;
    virtual  ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageReceiver() = default;
};

class Storehouse:public IPackageReceiver
{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)):id_(id), d_(std::move(d)) {}
    void receive_package(Package&& package) override{(*d_).push(std::move(package));}
    ElementID get_id() const override {return  id_;}
    ReceiverType get_receiver_type() const override{return ReceiverType::STOREHOUSE;};

    const_iterator begin() const override{return d_->begin();}
    const_iterator end() const override{return d_->end();}
    const_iterator cbegin() const override{return d_->cbegin();}
    const_iterator cend() const override{return d_->cend();}



private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};

class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using preferences_pair = std::pair<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator ):pg_(std::move(pg)){};
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    const preferences_t& get_preferences() const {return this->preferences_;};
    //usunąlem const z linijki wyzej bo z nim byly bledy

    const_iterator begin() const {return preferences_.begin();};
    const_iterator end() const {return preferences_.end();};
    const_iterator cbegin() const {return preferences_.cbegin();};
    const_iterator cend() const {return preferences_.cend();};
private:
    preferences_t preferences_;
    //obiekt funkcyjny zwracajacy losowa wartosc
    ProbabilityGenerator pg_;
};

class PackageSender{
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender() = default;
    PackageSender(PackageSender&& p) = default;
    void send_package();
    const std::optional<Package>& get_sending_buffer() const {return buffer_;};

protected:
    void push_package(Package&& buffered_package) {buffer_.emplace(buffered_package.get_id());};

    std::optional<Package> buffer_ = std::nullopt;
};

class Ramp: public PackageSender{
public:
    Ramp(const Ramp&){};
    Ramp(ElementID id,TimeOffset di): PackageSender(), id_(id), di_(di),buffer_(std::nullopt){};
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const{return di_;};
    ElementID get_id() const{return id_;};
private:
    ElementID id_;
    TimeOffset di_;
    Time t_;
    std::optional<Package> buffer_;
};

class Worker:public PackageSender, public IPackageReceiver
{
public:
    Worker(const Worker&){};
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : PackageSender(), id_(id), pd_(pd), q_(std::move(q)){}
    void do_work(Time t);
    TimeOffset get_processing_duration() const {return pd_;}
    Time get_package_processing_start_time() const {return t_;}
    void receive_package(Package&& package) override{(*q_).push(std::move(package));};
    ElementID get_id() const override { return id_; }
    ReceiverType get_receiver_type() const override{return ReceiverType::WORKER;};


    const_iterator begin() const override{return q_->begin();}
    const_iterator end() const override{return q_->end();}
    const_iterator cbegin() const override{return q_->cbegin();}
    const_iterator cend() const override{return q_->cend();}
    IPackageQueue* get_queue() const { return q_.get(); }

private:
    ElementID id_;
    TimeOffset pd_;
    Time t_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> buffer_ = std::nullopt;
};

#endif //NODES_HXX
