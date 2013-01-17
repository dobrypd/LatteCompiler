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

class CompilerEnvironment
{
public:
    typedef struct {
        Type* type;
        int position;
        bool on_stack;
        bool temp;
    } var_info;

    typedef boost::shared_ptr<var_info> VarInfoPtr;
    typedef std::map<std::string, VarInfoPtr> MapType;
    typedef boost::shared_ptr<MapType> MapPtr;

private:
    std::vector<MapPtr> variables;
    std::vector<int> stack_size;
    int full_stack_size;

    MapPtr var_tip();
    int stack_size_tip();


public:
    CompilerEnvironment();

    void prepare();
    void back();

    int stack_current_offset();
    int stack_all_offset();

    void add_variable(Type* type, std::string name);
    VarInfoPtr get_variable(std::string& name);

    static size_t type_sizeof(Type* type);
};

} /* namespace backend */
#endif /* COMPILERENVIRONMENT_H_ */
