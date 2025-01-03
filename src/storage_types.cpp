#include "storage_types.hxx"

Package PackageQueue::pop() {
    Package p;
    if(type_ == PackageQueueType::FIFO){
        p = std::move(queue_.front());
        queue_.pop_front();
        return p;
    }
    else{
        p = std::move(queue_.back());
        queue_.pop_back();
        return p;
    }
}

void PackageQueue::push(Package&&){
 };

bool PackageQueue::empty() const{
    if(PackageQueue::queue_.empty())
        return True;
    else 
        return False;
};

std::size_t PackageQueue::size() const{
    return PackageQueue::queue_.length();
};
