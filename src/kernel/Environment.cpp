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

    this->global_int_type = new Int;
    this->global_str_type = new Str;
    this->global_void_type = new Void;
    this->arg_int.reset(new Environment::var_info);
    this->arg_int->type = this->global_int_type;
    this->arg_str.reset(new Environment::var_info);
    this->arg_str->type = this->global_str_type;

    this->print_int_f.reset(new Environment::fun_info);
    print_int_f->ret_type = this->global_void_type;
    print_int_f->is_extern = true;
    print_int_f->arguments.push_back(this->arg_int);
    this->env_f["printInt"] = this->print_int_f;

    this->print_string_f.reset(new Environment::fun_info);
    print_string_f->ret_type = this->global_void_type;
    print_string_f->is_extern = true;
    print_string_f->arguments.push_back(this->arg_str);
    this->env_f["printString"] = this->print_string_f;

    this->error_f.reset(new Environment::fun_info);
    error_f->ret_type = this->global_void_type;
    error_f->is_extern = true;
    this->env_f["error"] = this->error_f;

    this->read_int_f.reset(new Environment::fun_info);
    read_int_f->ret_type = this->global_int_type;
    read_int_f->is_extern = true;
    this->env_f["readInt"] = this->read_int_f;

    this->read_string_f.reset(new Environment::fun_info);
    read_string_f->ret_type = this->global_str_type;
    read_string_f->is_extern = true;
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
    this->submerge();
}
void Environment::back()
{
    if(this->env_v.size() > 0){
        this->emerge();
    }else{
        if (debug) std::cerr << "Emerging below bottom is not possible.";
        throw 0;
    }
}

void Environment::add_variable(Type *t, std::string & ident)
{
    MapPtr tip = this->env_v_tip();
    VarInfoPtr new_variable(new Environment::var_info);
    new_variable->type = t;
    (*tip)[ident] = new_variable;
}

void Environment::add_function(FnDef *function_definition,
        bool is_extern = false)
{
    Environment::FunInfoPtr new_function(new Environment::fun_info);
    new_function->ret_type = function_definition->type_;
    new_function->is_extern = is_extern;
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

bool Environment::can_add_variable(std::string& ident) const
{
    Environment::MapPtr env_tip = this->env_v.back();
    return (env_tip->find(ident)) == env_tip->end();
}

void Environment::add_class(std::string ident)
{
}

void Environment::add_class(std::string ident, std::string extends_ident)
{
}

void Environment::add_method_to_cls(std::string & class_name, FnDef *funciton_definition)
{
}

void Environment::add_field_to_cls(std::string & class_name, Type *type, std::string & ident)
{
}

bool Environment::can_add_function(std::string & ident) const
{
    return (this->env_f.find(ident)) == this->env_f.end();
}

Environment::VarInfoPtr Environment::get_variable(ListStructuredIdent *ident) const
{
    return VarInfoPtr();
}

Environment::FunInfoPtr Environment::get_function(ListStructuredIdent *ident) const
{
    return FunInfoPtr();
}

bool Environment::can_add_class(std::string & ident) const
{
}

VarInfoPtr Environment::get_variable(std::string & ident) const
{
}

FunInfoPtr Environment::get_function(std::string & ident) const
{
}

ClsInfoPtr Environment::get_class(std::string & ident) const
{
}

} /* namespace frontend */
