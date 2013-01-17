/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "CompilerEnvironment.h"

namespace backend
{


CompilerEnvironment::MapPtr CompilerEnvironment::var_tip()
{
    return this->variables.back();
}

int CompilerEnvironment::stack_size_tip()
{
    return this->stack_size.back();
}


CompilerEnvironment::CompilerEnvironment() : full_stack_size(0)
{
}

void CompilerEnvironment::prepare()
{
    CompilerEnvironment::MapPtr empty(new CompilerEnvironment::MapType());
    this->variables.push_back(empty);
    this->stack_size.push_back(0);
}

void CompilerEnvironment::back()
{
    this->variables.pop_back();
    this->full_stack_size -= this->stack_current_offset();
    this->stack_size.pop_back();
}

int CompilerEnvironment::stack_current_offset()
{
    return this->stack_size_tip();
}

int CompilerEnvironment::stack_all_offset()
{
    return this->full_stack_size;
}

void CompilerEnvironment::add_variable(Type* type, std::string name)
{
    MapPtr tip = this->var_tip();
    VarInfoPtr new_variable(new CompilerEnvironment::var_info);
    new_variable->type = type;
        (*tip)[name] = new_variable;
}

CompilerEnvironment::VarInfoPtr CompilerEnvironment::get_variable(std::string & name)
{
    for(std::vector<CompilerEnvironment::MapPtr>::const_reverse_iterator it =
            this->variables.rbegin(); it != this->variables.rend();it++){
        CompilerEnvironment::MapType::iterator fit = (*it)->find(name);
        if(fit != ((*it)->end())) {
            return fit->second;
        }
    }
    return VarInfoPtr();
}

size_t CompilerEnvironment::type_sizeof(Type* type)
{
    return 4; // TODO:
}

} /* namespace backend */
