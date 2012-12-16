/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "JVMEnvironment.h"

namespace jvm
{

JVMEnvironment::JVMEnvironment()
{
    JVMEnvironment::MapPtr empty(new JVMEnvironment::MapType());
    this->env_v.push_back(empty);

}

JVMEnvironment::MapPtr JVMEnvironment::env_v_tip()
{
    return this->env_v.back();
}

void JVMEnvironment::submerge()
{
    JVMEnvironment::MapPtr empty(new JVMEnvironment::MapType());
    this->env_v.push_back(empty);
}

void JVMEnvironment::emerge()
{
    this->env_v.pop_back();
}

void JVMEnvironment::prepare()
{
    this->submerge();
}
void JVMEnvironment::back()
{
    if(this->env_v.size() > 0){
        this->emerge();
    }else{
        throw 0;
    }
}

void JVMEnvironment::add_variable(Type *t, Ident & ident, int local_id)
{
    MapPtr tip = this->env_v_tip();
    VarInfoPtr new_variable(new JVMEnvironment::var_info);
    new_variable->type = t;
    new_variable->local_v = local_id;
    (*tip)[ident] = new_variable;
}

JVMEnvironment::VarInfoPtr JVMEnvironment::get_variable(Ident& ident) const
{
    for(std::vector<MapPtr>::const_reverse_iterator it = this->env_v.rbegin();
            it != this->env_v.rend(); it++) {
        MapType::iterator fit = (*it)->find(ident);
        if (fit != ((*it)->end()))
            return fit->second;
    }
    return VarInfoPtr();
}

} /* namespace jvm */
