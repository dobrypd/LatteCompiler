/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef JVMENVIRONMENT_H_
#define JVMENVIRONMENT_H_

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "Absyn.H"

namespace jvm
{

/*
 * It's some kind of simpler version of frontend::Environment
 * used while constructing jvm code.
 */
class JVMEnvironment
{

public:
    typedef struct {
        Type* type;
        int local_v;
    } var_info;

    typedef boost::shared_ptr<var_info> VarInfoPtr;
    typedef std::map<std::string, VarInfoPtr> MapType;
    typedef boost::shared_ptr<MapType> MapPtr;

private:
    std::vector<MapPtr> env_v;

    MapPtr env_v_tip();

    void submerge();
    void emerge();

public:
    JVMEnvironment();

    void prepare();
    void back();

    void add_variable(Type* t, Ident& ident, int local_id);
    VarInfoPtr get_variable(Ident& ident) const;
};

} /* namespace jvm */
#endif /* JVMENVIRONMENT_H_ */
