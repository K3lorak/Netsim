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
