/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <list>
#include <set>
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

// make space for pointer to vtable
const int Environment::object_fields_offset  = 1;
// make space for predefined methods
const int Environment::object_methods_offset = 0;


Environment::FunInfoPtr Environment::create_fun(Type* ret_type, ListArg* args)
{
    Environment::FunInfoPtr new_function(new Environment::fun_info);
    new_function->ret_type = ret_type;
    new_function->is_extern = false;
    new_function->position = -1;

    for(ListArg::iterator it = args->begin(); it != args->end();it++){
        Environment::VarInfoPtr next_argument(new Environment::var_info);
        Argument *argument = dynamic_cast<Argument*>(*it);
        if(argument == 0) {
            if (debug)
                std::cerr << "Cannot cast Arg to Argument. Did you changed grammar?";
            throw "Did you changed grammar?!";
        }

        next_argument->type = argument->type_;
        new_function->arguments.push_back(next_argument);
    }

    return new_function;
}

Environment::Environment()
{
    Environment::MapPtr empty(new Environment::MapType());
    this->env_v.push_back(empty);

    this->global_int_type = new Int;
    this->global_str_type = new Str;
    this->global_void_type = new Void;
    this->global_cls_type = new Class("");
    this->arg_int.reset(new Environment::var_info);
    this->arg_int->type = this->global_int_type;
    this->arg_str.reset(new Environment::var_info);
    this->arg_str->type = this->global_str_type;

    this->print_int_f.reset(new Environment::fun_info);
    print_int_f->ret_type = this->global_void_type;
    print_int_f->is_extern = true;
    print_int_f->position = -1;
    print_int_f->arguments.push_back(this->arg_int);
    this->env_f["printInt"] = this->print_int_f;

    this->print_string_f.reset(new Environment::fun_info);
    print_string_f->ret_type = this->global_void_type;
    print_string_f->is_extern = true;
    print_string_f->position = -1;
    print_string_f->arguments.push_back(this->arg_str);
    this->env_f["printString"] = this->print_string_f;

    this->error_f.reset(new Environment::fun_info);
    error_f->ret_type = this->global_void_type;
    error_f->is_extern = true;
    error_f->position = -1;
    this->env_f["error"] = this->error_f;

    this->read_int_f.reset(new Environment::fun_info);
    read_int_f->ret_type = this->global_int_type;
    read_int_f->is_extern = true;
    read_int_f->position = -1;
    this->env_f["readInt"] = this->read_int_f;

    this->read_string_f.reset(new Environment::fun_info);
    read_string_f->ret_type = this->global_str_type;
    read_string_f->is_extern = true;
    read_string_f->position = -1;
    this->env_f["readString"] = this->read_string_f;
}

Environment::~Environment()
{
    delete this->global_int_type;
    delete this->global_str_type;
    delete this->global_void_type;
    delete this->global_cls_type;
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

void Environment::add_variable(Type *t, std::string ident)
{
    MapPtr tip = this->env_v_tip();
    VarInfoPtr new_variable(new Environment::var_info);
    new_variable->type = t;
    (*tip)[ident] = new_variable;
}

void Environment::add_function(FnDef *function_definition,
        bool is_extern = false)
{
    Environment::FunInfoPtr new_function =
            this->create_fun(function_definition->type_,
                    function_definition->listarg_);
    new_function->is_extern = is_extern;

    this->env_f[function_definition->ident_] = new_function;
}

bool Environment::can_add_variable(std::string& ident) const
{
    Environment::MapPtr env_tip = this->env_v.back();
    return (env_tip->find(ident)) == env_tip->end();
}

void Environment::add_class(std::string ident)
{
    Environment::ClsInfoPtr new_class(new Environment::lat_class);
    new_class->ident = ident;
    this->env_cls[ident] = new_class;
}

void Environment::add_class(std::string ident, std::string extends_ident)
{
    Environment::ClsInfoPtr new_class(new Environment::lat_class);
    new_class->ident = ident;
    Environment::ClsInfoPtr parent = this->get_class(extends_ident);
    new_class->lat_cls_parent = parent;
    this->env_cls[ident] = new_class;
}

void Environment::add_method_to_cls(std::string & class_name, MethodDef *method_definition)
{
    Environment::ClsInfoPtr this_class = this->get_class(class_name);
    this_class->methods[method_definition->ident_] =
            this->create_fun(method_definition->type_,
                    method_definition->listarg_);
}

void Environment::add_field_to_cls(std::string & class_name, Type *type, std::string & ident)
{
    Environment::ClsInfoPtr this_class = this->get_class(class_name);
    Environment::VarInfoPtr new_variable(new Environment::var_info);
    new_variable->type = type;
    this_class->fields[ident] = new_variable;
}

bool Environment::can_add_function(std::string & ident) const
{
    return (this->env_f.find(ident)) == this->env_f.end();
}

bool Environment::can_add_class(std::string & ident) const
{
    return (this->env_cls.find(ident)) == this->env_cls.end();
}

bool Environment::can_add_class(std::string & ident, std::string & parent_ident) const
{
    return ((this->env_cls.find(parent_ident)) != this->env_cls.end()) && ((this->env_cls.find(ident)) == this->env_cls.end());
}

Environment::VarInfoPtr Environment::get_variable(std::string & ident) const
{
    for(std::vector<Environment::MapPtr>::const_reverse_iterator it = this->env_v.rbegin();it != this->env_v.rend();it++){
        Environment::MapType::iterator fit = (*it)->find(ident);
        if(fit != ((*it)->end())){
            return fit->second;
        }
    }
    return Environment::VarInfoPtr();
}

Environment::FunInfoPtr Environment::get_function(std::string & ident) const
{
    std::map<std::string, FunInfoPtr>::const_iterator fit = this->env_f.find(ident);
    if(fit != (this->env_f.end())){
        return fit->second;
    }

    return Environment::FunInfoPtr();
}

Environment::ClsInfoPtr Environment::get_class(std::string & ident) const
{
    std::map<std::string, ClsInfoPtr>::const_iterator fit = this->env_cls.find(ident);
    if(fit != (this->env_cls.end())){
        return fit->second;
    }

    return Environment::ClsInfoPtr();
}

std::vector<Environment::MapPtr>::iterator Environment::get_env_v_it_begin()
{
    return this->env_v.begin();
}

std::vector<Environment::MapPtr>::iterator Environment::get_env_v_it_end()
{
    return this->env_v.end();
}

std::map<std::string, Environment::FunInfoPtr>::iterator Environment::get_env_f_begin()
{
    return this->env_f.begin();
}

std::map<std::string, Environment::FunInfoPtr>::iterator Environment::get_env_f_end()
{
    return this->env_f.end();
}

std::map<std::string, Environment::ClsInfoPtr>::iterator Environment::get_env_cls_begin()
{
    return this->env_cls.begin();
}

std::map<std::string,Environment::ClsInfoPtr>::iterator Environment::get_env_cls_end()
{
    return this->env_cls.end();
}

Environment::MethodsPtr Environment::get_class_methods_list(std::string& cls_name)
{
    Environment::MethodsPtr methods(new std::vector<PairOfStrPtr>);
    ClsInfoPtr cls = this->get_class(cls_name);
    std::list<ClsInfoPtr> inheritance_chain;
    while (cls) {
        inheritance_chain.push_front(cls);
        cls = cls->lat_cls_parent;
    }
    for (std::list<ClsInfoPtr>::iterator it = inheritance_chain.begin();
            it != inheritance_chain.end(); it++) {
        for(Environment::lat_class::methods_t::iterator
                m_it = (*it)->methods.begin(); m_it != (*it)->methods.end(); m_it++) {
            FunInfoPtr fun = m_it->second;
            int position_on_vector = fun->position - Environment::object_methods_offset;
            StringPtr cls_name_ptr(new std::string((*it)->ident));
            StringPtr function_ident_ptr(new std::string(m_it->first));

            PairOfStrPtr new_function(
                    std::make_pair( cls_name_ptr, function_ident_ptr)
            );
            if (methods->size() <= (unsigned)position_on_vector)
                methods->resize(position_on_vector + 1);
            (*methods)[position_on_vector] = new_function;  // maybe function will be overriden
        }
    }

    return methods;
}

void Environment::set_class_positions() {
    typedef std::map<std::string, Environment::ClsInfoPtr>::iterator it_t;
    for (it_t it = this->get_env_cls_begin(); it != this->get_env_cls_end(); it++) {
        ClsInfoPtr cls = it->second;
        // first:MOST BASE CLASS -> ... -> THIS_CLASS:back
        std::list<ClsInfoPtr> inheritance_chain;
        while (cls) {
            inheritance_chain.push_front(cls);
            cls = cls->lat_cls_parent;
        }
        // FIELDS
        int position = Environment::object_fields_offset;
        for (std::list<ClsInfoPtr>::iterator it = inheritance_chain.begin();
                it != inheritance_chain.end(); it++) {
            for(Environment::lat_class::fields_t::iterator
                    f_it = (*it)->fields.begin(); f_it != (*it)->fields.end(); f_it++) {
                f_it->second->position = position;
                position++;
            }
        }

        // METHODS
        position = Environment::object_methods_offset;
        std::map<std::string, FunInfoPtr> already_added;
        for (std::list<ClsInfoPtr>::iterator it = inheritance_chain.begin();
                it != inheritance_chain.end(); it++) {
            for(Environment::lat_class::methods_t::iterator
                    m_it = (*it)->methods.begin(); m_it != (*it)->methods.end(); m_it++) {
                FunInfoPtr fun = m_it->second;
                std::map<std::string, FunInfoPtr>::iterator pos = already_added.find(m_it->first);
                if (pos != already_added.end()) {
                    fun->position = pos->second->position;
                } else {
                    // New function
                    fun->position = position;
                    position++;
                    already_added[m_it->first] = fun;
                }
            }
        }
    }
}

Environment::VarInfoPtr Environment::get_field(std::string & ident, std::string & cls_name)
{
    ClsInfoPtr cls = this->get_class(cls_name);
    std::list<ClsInfoPtr> inheritance_chain;
    while (cls) {
        inheritance_chain.push_front(cls);
        cls = cls->lat_cls_parent;
    }
    for (std::list<ClsInfoPtr>::iterator it = inheritance_chain.begin();
            it != inheritance_chain.end(); it++) {
        lat_class::fields_t::iterator var_it = (*it)->fields.find(ident);
        if (var_it != (*it)->fields.end()) return var_it->second;
    }
    return Environment::VarInfoPtr();
}

Environment::FunInfoPtr Environment::get_method(std::string & ident, std::string & cls_name)
{
    //if (debug) std::cout << "get method " << ident << " from class: " << cls_name << std::endl;
    ClsInfoPtr cls = this->get_class(cls_name);
    Environment::FunInfoPtr fun;
    std::list<ClsInfoPtr> inheritance_chain;
    while (cls) {
        inheritance_chain.push_back(cls);
        cls = cls->lat_cls_parent;
    }
    for (std::list<ClsInfoPtr>::iterator it = inheritance_chain.begin();
            it != inheritance_chain.end(); it++) {
        //if (debug) std::cout << "\tsearching in " << (*it)->ident << std::endl;
        lat_class::methods_t::iterator fun_it = (*it)->methods.find(ident);
        if (fun_it != (*it)->methods.end()) return fun_it->second;
    }
    return Environment::FunInfoPtr();
}

int Environment::get_class_size(std::string & cls_name)
{
    // All fields size in chain;
    ClsInfoPtr cls = this->get_class(cls_name);
    int sum = Environment::object_fields_offset;
    while (cls) {
        sum += cls->fields.size();
        cls = cls->lat_cls_parent;
    }
    return sum;
}


} /* namespace frontend */
