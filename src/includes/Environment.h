/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <boost/shared_ptr.hpp>
#include "Absyn.H"

namespace frontend
{

class Environment
{
public:
    typedef struct {
        Type* type;
    } var_info;

    typedef boost::shared_ptr<var_info> VarInfoPtr;
    typedef std::map<std::string, VarInfoPtr> MapType;
    typedef boost::shared_ptr<MapType> MapPtr;

    typedef struct {
        Type* ret_type;
        std::vector<VarInfoPtr> arguments;
    } fun_info;

    typedef boost::shared_ptr<fun_info> FunInfoPtr;

private:

    // Environment for variables, vector of map -> ident, ident_struct
    std::vector<MapPtr> env_v;
    // Environment for functions.
    std::map<std::string, FunInfoPtr> env_f;

    MapPtr env_v_tip();

    void submerge();  // When going into nested block.
    void emerge();  // When returning from block.

    /*
     * Globals (in context of Latte program)
     */
    Void* global_void_type;
    Int* global_int_type;
    Str* global_str_type;
    Environment::FunInfoPtr print_int_f;
    Environment::FunInfoPtr print_string_f;
    Environment::FunInfoPtr error_f;
    Environment::FunInfoPtr read_int_f;
    Environment::FunInfoPtr read_string_f;

    Environment::VarInfoPtr arg_int;
    Environment::VarInfoPtr arg_str;
    /*
     * end globals.
     */

public:
    Environment();
    ~Environment();

    void prepare();
    void back();

    void add_variable(Type* t, Ident& ident);
    void add_function(FnDef* function_definition);

    void set_variable();  // TODO: really I need this?
    void set_function();

    bool can_add_variable(Ident& ident) const;
    bool can_add_funciton(FnDef* function_definition) const;

    VarInfoPtr get_variable(Ident& ident) const;
    FunInfoPtr get_function(Ident& ident) const;
};

} /* namespace frontend */
#endif /* ENVIRONMENT_H_ */
