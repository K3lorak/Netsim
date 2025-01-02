#ifndef PACKAGE_HPP
#define PACKAGE_HPP
#include "types.hxx"
#include "storage_types.hxx"
#include <cstdlib>
#include <list>
class Package {
  public:
    //konstruktory
    Package() : id(0) {}
    Package(ElementID id) : id(id) {};
    Package(Package&& other) noexcept;
    //metody
    Package& operator=(Package&& other) noexcept;
    ElementID get_id() const;
    //destruktor
    ~Package() {};

  private:
      ElementID id;
    
};

class PackageQueue: public IPackageQueue{
public:
    PackageQueue(PackageQueueType type) : type_ (type){}


private:
    std::list<Package> queue_;
    PackageQueueType type_;
};
#endif //PACKAGE_HPP
