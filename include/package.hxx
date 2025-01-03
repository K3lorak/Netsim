#ifndef PACKAGE_HPP
#define PACKAGE_HPP
#include "types.hxx"
#include "storage_types.hxx"
#include <cstdlib>
#include <list>
class Package {
  public:
    //konstruktory
    Package() : id_(0) {}
    Package(ElementID id) : id_(id) {};
    Package(Package&& other) noexcept;

    //metody
    Package& operator=(Package&& other) noexcept;
    ElementID get_id() const {return id_;};

    //destruktor
    ~Package() {};

  private:
      ElementID id_;
    
};
#endif //PACKAGE_HPP
