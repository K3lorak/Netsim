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
     void push(Package&&);
     bool empty() const {query}
     std::size_t size() const {query}
     ~IPackageStockpile() {}
};

class IPackageQueue{
  public:
    Package pop();
    PackageQueueType get_queue_type() const {query};
#endif //STORAGE_TYPES_HPP
