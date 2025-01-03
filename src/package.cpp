#include "package.hxx"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <set>

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package::Package(){
    // jak freed jest puste
    // to przydziela id o 1 większe niż ostatnie w assigned
    // else przydziela najmniejsze z freed
    if(freed_IDs.empty()){
        if(assigned_IDs.empty()){
            id_ = 1;
        }
        else{
            id_ = *assigned_IDs.end() + 1;
        }
    }
    else{
        id_ = *freed_IDs.begin();
        freed_IDs.erase(*freed_IDs.begin());
    }
    assigned_IDs.insert(id_);
};

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

Package::~Package() {
    freed_IDs.insert(id_);
    assigned_IDs.erase(id_);
}



