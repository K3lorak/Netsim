#include "storage_types.hxx"

Package PackageQueue::pop() {
    switch(type_) {
        case PackageQueueType::FIFO: {
            Package& p = queue_.front();
            queue_.pop_front();
            return p;
        }
        case PackageQueueType::LIFO: {
            Package& p = queue_.back();
            queue_.pop_back();
            return p;
        }
        default:
            break;
    };
}

void PackageQueue::push(Package&&){
 };

bool PackageQueue::empty(){
    if(PackageQueue::queue_.empty())
        return True;
    else 
        return False;
};

std::size_t PackageQueue::size(){
};
