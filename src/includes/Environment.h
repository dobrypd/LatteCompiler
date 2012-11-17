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
    enum etypes {INT, BOOL, STRING, VOID, FUN, UNDEFINED};

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

    //bool comp_t(Type* t1, Type* t2);

    void submerge();  // When going into nested block.
    void emerge();  // When returning from block.

public:
    Environment();

    void prepare();
    void back();

    void add_variable(Type* t, Ident& ident);
    void add_function(FnDef* function_definition);

    void set_variable();  // TODO: really I need this?
    void set_function();

    bool lookup_variable(Ident& ident);
    bool lookup_function(FnDef* function_definition) const;

    FunInfoPtr get_function(std::string& ident) const;
};

} /* namespace frontend */
#endif /* ENVIRONMENT_H_ */
