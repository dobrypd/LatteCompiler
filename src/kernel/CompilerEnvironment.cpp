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


CompilerEnvironment::CompilerEnvironment() : varaibles_on_stack(0)
{
}

void CompilerEnvironment::prepare()
{
    CompilerEnvironment::MapPtr empty(new CompilerEnvironment::MapType());
    this->variables.push_back(empty);
}

int CompilerEnvironment::back()
{
    int current_block_size = this->variables.back()->size();
    this->variables.pop_back();
    this->varaibles_on_stack -= current_block_size;
    return current_block_size;
}

void CompilerEnvironment::add_variable(Type *type, std::string name, bool is_argument)
{
    MapPtr tip = this->var_tip();
    VarInfoPtr new_variable(new CompilerEnvironment::var_info);
    new_variable->type = type;
    if (is_argument){
        new_variable->position = -(++this->varaibles_on_stack) - 1; // -1 because of Return address
    } else {
        new_variable->position = ++this->varaibles_on_stack;
    }
    (*tip)[name] = new_variable;
}

CompilerEnvironment::VarInfoPtr CompilerEnvironment::get_variable(std::string & name)
{
    for(std::vector<CompilerEnvironment::MapPtr>::const_reverse_iterator it = this->variables.rbegin();it != this->variables.rend();it++){
        CompilerEnvironment::MapType::iterator fit = (*it)->find(name);
        if(fit != ((*it)->end())){
            return fit->second;
        }
    }

    return CompilerEnvironment::VarInfoPtr();
}

void CompilerEnvironment::new_fun()
{
    this->varaibles_on_stack = 0;
}

} /* namespace backend */
