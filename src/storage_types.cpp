#include "storage_types.hxx"

Package IPackageQueue::pop() {
    return Package();
}

PackageQueueType IPackageQueue::get_queue_type() const {
    return PackageQueueType::FIFO;
}
