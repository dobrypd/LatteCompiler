/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <map>
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

}

/* Ugly but very good working function */
Environment::etypes type_to_enum(Type* t)
{
    if (dynamic_cast<Int *>(t) != 0)
        return Environment::INT;
    else if (dynamic_cast<Str *>(t) != 0)
        return Environment::STRING;
    else if (dynamic_cast<Bool *>(t) != 0)
        return Environment::BOOL;
    else if (dynamic_cast<Void *>(t) != 0)
        return Environment::VOID;
    else if (dynamic_cast<Fun *>(t) != 0)
        return Environment::FUN;
    else
        return Environment::UNDEFINED;

}

bool operator==(Type& t1, Type& t2)
{
    Type* p1 = &t1; // Do not want to catch std::bad_cast.
    Type* p2 = &t2;

    Environment::etypes et1 = type_to_enum(p1);
    Environment::etypes et2 = type_to_enum(p2);

    return et1 == et2;
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
        std::cout << "Preparing environment to new " << this->env_v.size() + 1 << " block." << std::endl;

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

void Environment::add_variable(Type* t, Ident& ident)
{
//TODO:
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
        Argument* argument = dynamic_cast<Argument* >(*it);
        if (argument == 0)
            throw "Did you changed grammar?!";
        next_argument->type = argument->type_;
        // I'm omitting identified this time.
        new_function->arguments.push_back(next_argument);
    }
    if(debug)
        std::cout << std::endl;

    this->env_f[function_definition->ident_] = new_function;

    //    if (debug)
    //    {
    //        std::string s = ((this->env_f.find(function_definition->ident_))->first);
    //        FunInfoPtr f = ((this->env_f.find(function_definition->ident_))->second);
    //
    //        if (f->arguments.size() >= 2) {
    //            std::vector<VarInfoPtr>::iterator it1 = f->arguments.begin();
    //            std::vector<VarInfoPtr>::iterator it2 = it1 + 1;
    //            std::cout << ( (*((*it1)->type)) == (*((*it2)->type))) << std::endl;
    //        }
    //    }
}
void Environment::set_variable()
{
}

void Environment::set_function()
{
}

bool Environment::lookup_variable(Ident& ident)
{
    //for()
}

bool Environment::lookup_function(FnDef *function_definition) const
{
    return (this->env_f.find(function_definition->ident_)) != this->env_f.end();
}

// Call only if you are sure that this function exists.
Environment::FunInfoPtr Environment::get_function(std::string & ident) const
{
    return (this->env_f.find(ident))->second;
}

} /* namespace frontend */
