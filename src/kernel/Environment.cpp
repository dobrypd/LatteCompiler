/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include "Absyn.H"
#include "Environment.h"
#include "global.h"

namespace frontend
{

Environment::Environment()
{
    Environment::MapPtr empty(new Environment::MapType());
    this->env_v.push_back(empty);
    if(debug)
        std::cout << "Initializing new environment at address: " << this << std::endl;

    this->global_int_type = new Int;
    this->global_str_type = new Str;
    this->global_void_type = new Void;
    this->arg_int.reset(new Environment::var_info);
    this->arg_int->type = this->global_int_type;
    this->arg_str.reset(new Environment::var_info);
    this->arg_str->type = this->global_str_type;

    this->print_int_f.reset(new Environment::fun_info);
    print_int_f->ret_type = this->global_void_type;
    print_int_f->arguments.push_back(this->arg_int);
    this->env_f["printInt"] = this->print_int_f;

    this->print_string_f.reset(new Environment::fun_info);
    print_string_f->ret_type = this->global_void_type;
    print_string_f->arguments.push_back(this->arg_str);
    this->env_f["printString"] = this->print_string_f;

    this->error_f.reset(new Environment::fun_info);
    error_f->ret_type = this->global_void_type;
    this->env_f["error"] = this->error_f;

    this->read_int_f.reset(new Environment::fun_info);
    read_int_f->ret_type = this->global_int_type;
    this->env_f["readInt"] = this->read_int_f;

    this->read_string_f.reset(new Environment::fun_info);
    read_string_f->ret_type = this->global_str_type;
    this->env_f["readString"] = this->read_string_f;
}

Environment::~Environment()
{
    delete this->global_int_type;
    delete this->global_str_type;
    delete this->global_void_type;
}

Environment::MapPtr Environment::env_v_tip()
{
    return this->env_v.back();
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
    if(debug)
        std::cout << "Preparing environment to new " << this->env_v.size() + 1 << "'th block." << std::endl;

    this->submerge();
}
void Environment::back()
{
    if(debug)
        std::cout << "Back from block " << this->env_v.size() << "." << std::endl;

    if(this->env_v.size() > 0){
        this->emerge();
    }else{
        throw 0;
    }
}

void Environment::add_variable(Type *t, Ident & ident)
{
    if (debug)
        std::cout << "Adding new variable: " << ident << " line: " << t->line_number << std::endl;
    MapPtr tip = this->env_v_tip();
    VarInfoPtr new_variable(new Environment::var_info);
    new_variable->type = t;
    (*tip)[ident] = new_variable;
}

void Environment::add_function(FnDef *function_definition)
{
    Environment::FunInfoPtr new_function(new Environment::fun_info);
    new_function->ret_type = function_definition->type_;
    if(debug && (function_definition->listarg_->size() > 0))
        std::cout << " found arguments: ";

    for(ListArg::iterator it = function_definition->listarg_->begin();it != function_definition->listarg_->end();it++){
        if(debug)
            std::cout << (dynamic_cast<Argument*>(*it))->ident_ << ", ";

        Environment::VarInfoPtr next_argument(new Environment::var_info);
        Argument *argument = dynamic_cast<Argument*>(*it);
        if(argument == 0)
            throw "Did you changed grammar?!";

        next_argument->type = argument->type_;
        new_function->arguments.push_back(next_argument);
    }
    if(debug)
        std::cout << std::endl;

    this->env_f[function_definition->ident_] = new_function;
}

bool Environment::can_add_variable(Ident & ident) const
{
    Environment::MapPtr env_tip = this->env_v.back();
    return (env_tip->find(ident)) == env_tip->end();
}

bool Environment::can_add_funciton(FnDef *function_definition) const
{
    return (this->env_f.find(function_definition->ident_)) == this->env_f.end();
}

Environment::VarInfoPtr Environment::get_variable(Ident& ident) const
{
    for(std::vector<MapPtr>::const_reverse_iterator it = this->env_v.rbegin();
            it != this->env_v.rend(); it++) {
        MapType::iterator fit = (*it)->find(ident);
        if (fit != ((*it)->end()))
            return fit->second;
    }
    return VarInfoPtr();
}


Environment::FunInfoPtr Environment::get_function(Ident& ident) const
{
    std::map<std::string, FunInfoPtr>::const_iterator it = this->env_f.find(ident);
    if (it == this->env_f.end())
        return FunInfoPtr();
    return it->second;
}

} /* namespace frontend */
