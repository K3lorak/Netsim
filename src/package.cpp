#include "package.hxx"
#include <cstdlib>
#include <vector>
#include <iostream>

Package::Package(){}
Package::Package(ElementID id) : id(id){}
Package::Package(Package&& other) : id(std::move(other.id)){
  other.id = 0;
}

Package& Package::operator=(Package&& other){
  if (this != &other){
    id = std::move(other.id);
    other.id = 0;
  }
  return *this;
}

ElementID Package::get_id() const{
  return id;
}
