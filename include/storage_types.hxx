enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:
        void push(Package&&){}

        bool empty() {query}

        std::size_t size() {query}

        //iteratory - do zrobienia

        ~IPackageStockpile()
};

class IPackageQueue{
    public:
        Package pop() {}

        PackageQueueType get_queue_type() {query}
};

class PackageQueue(IPackageQueue){
    public:
        PackageQueue(PackageQueueType):{}
};
