#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP
#include "package.hxx"
#include <cstdlib>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:
        virtual void push(Package&&);
        virtual bool empty() const {}
        virtual std::size_t size() const {}
        virtual ~IPackageStockpile() {}
};

class IPackageQueue: public IPackageStockpile{
    public:
        virtual Package pop();
        virtual PackageQueueType get_queue_type() const {}
};

class PackageQueue: public IPackageQueue{
public:
    PackageQueue(PackageQueueType type) : type_ (type){}
    PackageQueueType get_queue_type() const override(){return type_;}
private:
    std::list<Package> queue_;
    PackageQueueType type_;
};

#endif //STORAGE_TYPES_HPP