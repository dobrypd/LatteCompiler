/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <map>
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include "Environment.h"
#include "global.h"

namespace frontend
{

Environment::Environment()
{
    // Create empty environment.
    Environment::MapPtr empty(new Environment::MapType());
    this->env_v.push_back(empty);
    if (debug)
        std::cout << "Initializing new environment at address: " << this << std::endl;
}

void Environment::submerge()
{
    Environment::MapPtr empty(new Environment::MapType());
    this->env_v.push_back(empty);
}


void Environment::emerge()
{
    this->env_v.pop_back();
}


void Environment::prepare()
{
    if (debug)
        std::cout << "Preparing environment to new " << this->env_v.size() << " block." << std::endl;
    this->submerge();
}

void Environment::back()
{
    if (debug)
            std::cout << "Back from " << this->env_v.size() << " block." << std::endl;
    if (this->env_v.size() > 0)
    {
        this->emerge();
    }
    else
    {
        // error
        throw 0;
    }
}

void Environment::add_variable()
{
}

void Environment::add_function(FnDef* function_definition)
{
    Environment::FunInfoPtr new_function(new Environment::fun_info);
    new_function->ret_type = function_definition->type_;

    if (debug) std::cout << "Found arguments: ";
    for (ListArg::iterator it = function_definition->listarg_->begin();
            it != function_definition->listarg_->end(); it++){

        if (debug) std::cout << (dynamic_cast<Argument*>(*it))->ident_ << ", ";
        Environment::VarInfoPtr next_argument(new Environment::var_info);
        new_function->arguments.push_back(next_argument);
    }

    if (debug) std::cout << std::endl;
    this->env_f[function_definition->ident_] = new_function;
}

void Environment::set_variable()
{
}

void Environment::set_function()
{
}

bool Environment::lookup_variable()
{
}

bool Environment::lookup_function(FnDef* function_definition)
{
    return (this->env_f.find(function_definition->ident_)) != this->env_f.end();
}

} /* namespace frontend */
