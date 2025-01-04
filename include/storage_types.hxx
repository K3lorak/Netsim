#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP
#include "package.hxx"
#include <cstdlib>
#include <vector>
#include <list>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using Iterator = std::list<Package>::iterator;
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& moved) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;

    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;

};

class IPackageQueue: public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;

    //destruktor
    ~IPackageQueue() override = default;
};

class PackageQueue: public IPackageQueue{
public:
    PackageQueue() = delete;

    PackageQueue(PackageQueueType type) : type_ (type), queue_(){}
    Package pop() override ;
    PackageQueueType get_queue_type() const override {return type_;}

    void push(Package&& moved) override{
        queue_.emplace_back(std::move(moved));
    }
    bool empty() const override{
        return queue_.empty();
    }
    std::size_t size() const override{
        return queue_.size();
    }

    Iterator begin() override {return queue_.begin();}
    Iterator end() override {return queue_.end();}

    const_iterator begin() const override {return queue_.begin();};
    const_iterator end() const override {return queue_.end();};
    const_iterator cbegin() const override {return queue_.cbegin();}
    const_iterator cend() const override {return queue_.cend();}

    //destruktor
    ~PackageQueue() override = default;
private:
    PackageQueueType type_;
    std::list<Package> queue_;
};




#endif //STORAGE_TYPES_HPP
