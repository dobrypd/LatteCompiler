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

enum arch_t {x86, x86_64};

#ifdef _ARCH_x86_64
    const int arch = x86_64;
#else
    #ifdef _ARCH_x86
        const int arch = x86;
    #else
        #error You must define _ARCH as x86 or x86_64.
    #endif
#endif

std::string type_pretty_print(Type* t);
std::string ident_to_string(ListStructuredIdent* ident_list);
std::string int2str(int value);

bool operator==(Type & t1, Type & t2);

template <class T, class U>
bool check_is(U a)
{
    return !((dynamic_cast<T>(a)) == 0);
}

#endif /* GLOBAL_H_ */
