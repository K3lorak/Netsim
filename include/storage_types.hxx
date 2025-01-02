#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP
#include "package.hxx"
#include <cstdlib>
#include <list>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    void push(Package&&);
    bool empty() const {}
    std::size_t size() const {}
    ~IPackageStockpile() {}
};

class IPackageQueue: public IPackageStockpile {
public:
    virtual Package pop();

    virtual PackageQueueType get_queue_type() const;
};


#endif //STORAGE_TYPES_HPP