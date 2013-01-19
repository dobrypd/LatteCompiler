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
    this->current_stack_size.push_back(0);
    // SET EBP
}

int CompilerEnvironment::back()
{
    this->variables.pop_back();
    int remove_variables = this->current_stack_size.back();
    this->current_stack_size.pop_back();
    this->varaibles_on_stack -= remove_variables;
    return remove_variables; // ESP should be changed.
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
    //add +1 to `current` stack
    MapPtr tip = this->var_tip();
    VarInfoPtr new_variable(new CompilerEnvironment::var_info);
    new_variable->type = type;
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

CompilerEnvironment::VarInfoPtr CompilerEnvironment::get_variable(ListStructuredIdent *ident)
{
    return CompilerEnvironment::VarInfoPtr();
}

void CompilerEnvironment::new_fun()
{
    // EBP changed!
    this->varaibles_on_stack = 0;
}

void CompilerEnvironment::prev_fun()
{
    // TODO: check it!
    this->varaibles_on_stack = this->current_stack_size.back();
}

size_t CompilerEnvironment::type_sizeof(Type *type)
{
    return 4;
}

} /* namespace backend */
