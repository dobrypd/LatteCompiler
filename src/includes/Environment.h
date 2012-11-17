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
    typedef boost::shared_ptr<var_info> VarInfoPtr;
    typedef boost::shared_ptr<std::string> StrPtr;
    typedef std::map<std::string, VarInfoPtr> MapType;
    typedef boost::shared_ptr<MapType> MapPtr;
    typedef boost::shared_ptr<fun_info> FunInfoPtr;

    // Environment for variables, vector of map -> ident, ident_struct
    std::vector<MapPtr> env_v;
    // Environment for functions.
    std::map<std::string, FunInfoPtr> env_f;

    void submerge();  // When going into nested block.
    void emerge();  // When returning from block.

public:
    Environment();

    void prepare();
    void back();

    void add_variable();
    void add_function();

    void set_variable();  // TODO: really I need this?
    void set_function();

    bool lookup_variable();
    bool lookup_function();
};

} /* namespace frontend */
#endif /* ENVIRONMENT_H_ */
