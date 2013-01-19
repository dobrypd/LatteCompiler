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
        int position;
        bool on_stack;
    } var_info;
    typedef boost::shared_ptr<var_info> VarInfoPtr;
    typedef std::map<std::string,VarInfoPtr> MapType;
    typedef boost::shared_ptr<MapType> MapPtr;
private:
    std::vector<MapPtr> variables;
    int varaibles_on_stack; // offset
    std::vector<int> variables_sizes;
    std::vector<int> variables_on_current_block;
    int return_addr;

public:
    CompilerEnvironment();

    void prepare();
    void back();

    int stack_size();

    void add_variable(Type* type, std::string name);
    void add_obj(std::string obj_name, frontend::Environment::ClsInfoPtr cls);
    void add_array(Type* type, std::string name);

    VarInfoPtr get_variable(std::string& name);

    int get_return_addr() const;
    void set_return_addr();

    static size_t type_sizeof(Type* type);
};

} /* namespace backend */
#endif /* COMPILERENVIRONMENT_H_ */
