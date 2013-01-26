/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef COMPILERENVIRONMENT_H_
#define COMPILERENVIRONMENT_H_

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "Absyn.H"
#include "Environment.h"

namespace backend
{

/*
 * This environment is dynamically changing while passing code.
 */
class CompilerEnvironment
{
public:
    typedef frontend::Environment::var_info var_info;
    typedef boost::shared_ptr<var_info> VarInfoPtr;
    typedef std::map<std::string,VarInfoPtr> MapType;
    typedef boost::shared_ptr<MapType> MapPtr;
private:
    std::vector<MapPtr> variables;
    int varaibles_on_stack; // offset

    MapPtr var_tip();

public:
    CompilerEnvironment();

    void prepare();
    int back(); // returns no of variables to remove from stack

    int stack_size();

    void add_variable(Type* type, std::string name, bool is_argument=false);
    void add_vtable();

    VarInfoPtr get_variable(std::string& name);

    void new_fun();
};

} /* namespace backend */
#endif /* COMPILERENVIRONMENT_H_ */
