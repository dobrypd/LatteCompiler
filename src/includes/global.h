/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <string>
#include "Absyn.H"

#ifndef NDEBUG
    const bool debug=true;
#else
    const bool debug=false;
#endif


namespace frontend
{

enum etypes {INT, BOOL, STRING, VOID, FUN, UNDEFINED};

etypes type_to_enum(Type* t);
std::string type_pretty_print(Type* t);
bool operator==(Type & t1, Type & t2);

template <class T, class U>
bool check_is(U a)
{
    return !((dynamic_cast<T>(a)) == 0);
}

}

#endif /* GLOBAL_H_ */
