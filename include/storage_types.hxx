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

    virtual void push(Package&& moved) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;

    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;

    virtual ConstIterator begin() const = 0;
    virtual ConstIterator end() const = 0;
    virtual ConstIterator cbegin() const = 0;
    virtual ConstIterator cend() const = 0;

    virtual ~IPackageStockpile() = default;

};

class IPackageQueue: public IPackageStockpile{
    public:
        virtual Package pop();
        virtual PackageQueueType get_queue_type() const {}

        //destruktor
        ~IPackageQueue() override = default;
};

class PackageQueue: public IPackageQueue{
public:
    PackageQueue(PackageQueueType type) : type_ (type), queue_(){}
    PackageQueueType get_queue_type() const override {return type_;}
    Package pop() override ;

    void push(Package&& moved) override{
        queue_.emplace_back(std::move(moved));
    }
    bool empty() const override{
        return queue_.end();
    }
    std::size_t size() const override{
         return queue_.size();   
    }

    Iterator begin() override {return queue_.cbegin();}
    Iterator end() override {return queue_.cend();}

    ConstIterator cbegin() const override {return queue_.cbegin();}
    ConstIterator cend() const override {return queue_.cend();}

    //destruktor
    ~PackageQueue() override = default;
private:
    std::list<Package> queue_;
    PackageQueueType type_;
};

#endif //STORAGE_TYPES_HPP
