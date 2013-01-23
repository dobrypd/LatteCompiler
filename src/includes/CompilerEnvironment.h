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
 * This environment is dynamically cahnging while passing code.
 */
class CompilerEnvironment
{
public:
    typedef struct {
        Type* type;
        int position; // Difference between current function and EBP.
        bool on_stack; // Is simple type.
    } var_info;
    typedef boost::shared_ptr<var_info> VarInfoPtr;
    typedef std::map<std::string,VarInfoPtr> MapType;
    typedef boost::shared_ptr<MapType> MapPtr;
private:
    std::vector<MapPtr> variables;
    int varaibles_on_stack; // offset
    std::vector<int> current_stack_size; // for every block
    std::vector<int> variables_on_current_block;

    MapPtr var_tip();

public:
    CompilerEnvironment();

    void prepare();
    int back(); // returns no of variables to remove from stack

    int stack_size();

    void add_variable(Type* type, std::string name);
    void add_obj(std::string obj_name, frontend::Environment::ClsInfoPtr cls);
    void add_array(Type* type, std::string name);
    void add_vtable();

    VarInfoPtr get_variable(std::string& name);

    void new_fun();
    void prev_fun();

    static size_t type_sizeof(Type* type);
};

} /* namespace backend */
#endif /* COMPILERENVIRONMENT_H_ */
