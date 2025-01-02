
#define PACKAGE_HPP


#include "types.hxx"
#include "storage_types.hxx"

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

