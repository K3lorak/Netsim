#ifndef PACKAGE_HPP
#define PACKAGE_HPP
#include "types.hxx"
#include <cstdlib>
#include <list>
#include <set>
class Package {
public:
    //konstruktory
    Package();
    Package(ElementID id) : id_(id) {assigned_IDs.insert(id_);};
    Package(Package&& other) noexcept;

    //metody
    Package& operator=(Package&& other) noexcept;
    ElementID get_id() const {return id_;};

    //destruktor
    ~Package();

private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};
#endif //PACKAGE_HPP
