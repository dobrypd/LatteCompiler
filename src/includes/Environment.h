/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "Absyn.H"

namespace frontend
{

class Environment
{
public:
    struct var_info
    {
        Type* type;
        int position;  // assigned in time of searching.
    };

    typedef boost::shared_ptr<var_info> VarInfoPtr;
    typedef std::map<std::string, VarInfoPtr> MapType;
    typedef boost::shared_ptr<MapType> MapPtr;

    typedef struct
    {
        bool is_extern;
        Type* ret_type;
        std::vector<VarInfoPtr> arguments;
        int position;  // assigned in time of searching.
    } fun_info;

    typedef boost::shared_ptr<fun_info> FunInfoPtr;

    // Classes
    struct lat_class
    {
        // Vector because position is important.
        typedef std::vector<std::pair<std::string, FunInfoPtr> > methods_t;
        typedef std::vector<std::pair<std::string, VarInfoPtr> > fields_t;

        methods_t methods;
        fields_t fields;

        std::string ident;

        boost::shared_ptr<Environment::lat_class> lat_cls_parent;
    };

    typedef boost::shared_ptr<Environment::lat_class> ClsInfoPtr;

private:

    // Environment for variables, vector of map -> ident, ident_struct
    std::vector<MapPtr> env_v;
    // Environment for functions.
    std::map<std::string, FunInfoPtr> env_f;
    // Environment for classes.
    std::map<std::string, ClsInfoPtr> env_cls;

    MapPtr env_v_tip();

    void submerge();  // When going into nested block.
    void emerge();  // When returning from block.

    /*
     * Globals (in context of Latte program)
     */
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

    FunInfoPtr create_fun(Type* ret_type, ListArg* args);

public:
    Void* global_void_type;
    Int* global_int_type;
    Str* global_str_type;
    Class* global_cls_type;

    Environment();
    ~Environment();

    void prepare();
    void back();

    void add_variable(Type* t, std::string& ident);
    void add_function(FnDef* function_definition, bool is_extern);
    void add_class(std::string ident);
    void add_class(std::string ident, std::string extends_ident);
    void add_method_to_cls(std::string& class_name,
            MethodDef* method_definition);
    void add_field_to_cls(std::string& class_name, Type* type,
            std::string& ident);

    bool can_add_variable(std::string& ident) const;
    bool can_add_function(std::string& ident) const;
    bool can_add_class(std::string& ident) const;
    bool can_add_class(std::string& ident, std::string& parent_ident) const;

    VarInfoPtr get_variable(std::string& ident) const;
    FunInfoPtr get_function(std::string& ident) const;
    ClsInfoPtr get_class(std::string& ident) const;

    // Assign position -> no for exacly selected type!
    VarInfoPtr get_field(std::string& ident, std::string& cls_name);
    FunInfoPtr get_method(std::string& ident, std::string& cls_name);

    // Direct access to the environment:
    std::vector<MapPtr>::iterator get_env_v_it_begin();
    std::vector<MapPtr>::iterator get_env_v_it_end();
    std::map<std::string, FunInfoPtr>::iterator get_env_f_begin();
    std::map<std::string, FunInfoPtr>::iterator get_env_f_end();
    std::map<std::string, ClsInfoPtr>::iterator get_env_cls_begin();
    std::map<std::string, ClsInfoPtr>::iterator get_env_cls_end();

    int get_class_size(std::string& cls_name);
};

} /* namespace frontend */
#endif /* ENVIRONMENT_H_ */
