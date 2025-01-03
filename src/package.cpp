#include "package.hxx"
#include <cstdlib>
#include <vector>
#include <iostream>

Package::Package(Package&& other) noexcept : id_(std::move(other.id_)){
  other.id_ = 0;
}

Package& Package::operator=(Package&& other) noexcept {
  if (this != &other){
    id_ = std::move(other.id_);
    other.id_ = 0;
  }
  return *this;
}



