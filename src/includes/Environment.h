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

namespace frontend
{

class Environment
{
public:
    enum etypes {INT, BOOL, STRING, VOID};
    typedef struct {
        etypes type;
    } var_info;
    typedef struct {
        etypes ret_type;
    } fun_info;

private:
    // Environment for variables, vector of map -> ident, ident_struct
    std::vector<std::map<std::string, var_info> > env_v;
    // Environment for functions.
    std::map<std::string, fun_info> env_f;
    // Stack.
    std::deque<std::map<std::string, var_info> > env_stack;

    void submerge();  // When going into neasted block.
    void save_tip();  // Save tip of environment to stack.
    void load_tip(); // Load tip of environment from stack.
    void emerge();  // When returning from block.

public:
    Environment();

    // Common case - save_tip -> emerge -> submerge -> emerge -> load_tip - loading function
    void prepare_to_load_function();
    void back_from_function();

    void prepare_to_load_block();
    void back_from_block();

    void add_variable();
    void add_function();

    void set_variable(); // TODO: really I need this?
    void set_function();

    bool lookup_variable();
    bool lookup_function();
};

} /* namespace frontend */
#endif /* ENVIRONMENT_H_ */
