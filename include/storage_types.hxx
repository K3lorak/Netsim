#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP
#include "package.hxx"
#include <cstdlib>
#include <vector>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using Iterator = std::list<Package>::iterator;
    using ConstIterator = std::list<Package>::const_iterator;

    virtual void push(Package&&) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;

    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;

    virtual ConstIterator cbegin() const = 0;
    virtual ConstIterator cend() const = 0;

    virtual ~IPackageStockpile() = default;

};

class IPackageQueue: public IPackageStockpile{
    public:
        virtual Package pop();
        virtual PackageQueueType get_queue_type() const {}
};

class PackageQueue: public IPackageQueue{
public:
    PackageQueue(PackageQueueType type) : type_ (type){}
    PackageQueueType get_queue_type() const override {return type_;}
    Package pop() override ;

    Iterator begin() override {return queue_.begin();}
    Iterator end() override {return queue_.end();}

    ConstIterator cbegin() const override {return queue_.cbegin();}
    ConstIterator cend() const override {return queue_.cend();}
private:
    std::list<Package> queue_;
    PackageQueueType type_;
};

#endif //STORAGE_TYPES_HPP