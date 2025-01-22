#include "factory.hxx"
#include "nodes.hxx"

bool Factory::is_consistent() const{
  return 0
}

void Factory::do_deliveries(Time t){
  for (auto &ramp : cont_r)
        ramp.deliver_goods(t);
}

void Factory::do_package_passing(void){
  for (auto &ramp : cont_r)
        ramp.send_package();

    for (auto &worker : cont_w)
        worker.send_package();
}

oid Factory::do_work(Time t){
  for(auto &ramp : const_w)
    worker.do_work(t)
}
