/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "global.h"
#include "Absyn.H"
#include "Environment.h"

namespace frontend
{

etypes type_to_enum(Type *t)
{
    if(dynamic_cast<Int*>(t) != 0)
        return INT;

    else
    if(dynamic_cast<Str*>(t) != 0)
        return STRING;

    else
    if(dynamic_cast<Bool*>(t) != 0)
        return BOOL;

    else
    if(dynamic_cast<Void*>(t) != 0)
        return VOID;

    else
    if(dynamic_cast<Fun*>(t) != 0)
        return FUN;

    else
        return UNDEFINED;

}
std::string type_pretty_print(Type *t)
{
    switch (type_to_enum(t)){
        case INT:
            return "int";
        case BOOL:
            return "boolean";
        case STRING:
            return "string";
        case VOID:
            return "void";
        case FUN:
            return "function type";
        case UNDEFINED:
            return "undefined";
        default:
            return "undeclared";
    }
}

bool operator==(Type & t1, Type & t2)
{
    Type *p1 = &t1;
    Type *p2 = &t2;
    etypes et1 = type_to_enum(p1);
    etypes et2 = type_to_enum(p2);
    return et1 == et2;
}


} /* frontend */
