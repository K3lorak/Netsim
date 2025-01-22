#include "factory.hxx"
#include "nodes.hxx"

bool Factory::is_consistent() const;
void Factory::do_deliveries(Time t);
void Factory::do_package_passing(void);
void Factory::do_work(Time t);
