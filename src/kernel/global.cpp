/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <sstream>
#include "global.h"
#include "Absyn.H"
#include "Environment.h"

std::string type_pretty_print(Type *t)
{
    if (check_is<Int*>(t)) {
        return "int";
    } else if (check_is<Str*>(t)) {
        return "string";
    } else if (check_is<Bool*>(t)) {
        return "boolean";
    } else if (check_is<Void*>(t)) {
        return "void";
    } else if (check_is<TType*>(t)) {
        return "array of "
                + type_pretty_print(dynamic_cast<TType*>(t)->type_);
    } else if (check_is<Class*>(t)) {
        return "class " + (dynamic_cast<Class*>(t))->ident_;
    } else {
        if (debug) std::cerr << "Did you changed the grammar? "
                "Could not find type";
        return "undefined";
    }
}

/*
 * Very easy and slow but need it in this way.
 */
std::string int2str(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

bool operator==(Type & t1, Type & t2)
{
    Type *p1 = &t1;
    Type *p2 = &t2;

    if (check_is<Int*>(p1))
        return check_is<Int*>(p2);
    if (check_is<Str*>(p1))
        return check_is<Str*>(p2);
    if (check_is<Bool*>(p1))
        return check_is<Bool*>(p2);
    if (check_is<Void*>(p1))
        return check_is<Void*>(p2);
    if (check_is<TType*>(p1)) {
        if (check_is<TType*>(p2)) {
            return (*(dynamic_cast<TType*>(p1)->type_)
                == *(dynamic_cast<TType*>(p2)->type_));
        }
        return false;
    }
    if (check_is<Class*>(p1)) {
        if (check_is<Class*>(p2)) {
            return ((dynamic_cast<Class*>(p1)->ident_)
                == (dynamic_cast<Class*>(p2)->ident_));
        }
        return false;
    }
    return false;
}


std::string ident_to_string(ListStructuredIdent* ident_list)
{
    std::string identifier;
    for (ListStructuredIdent::iterator it = ident_list->begin();
            it != ident_list->end(); it++) {
        if (it != ident_list->begin())
            identifier += ".";
        if (dynamic_cast<SingleIdent*>(*it) != 0) {
            SingleIdent* si = dynamic_cast<SingleIdent*>(*it);
            identifier += si->ident_;
        }
        else if (dynamic_cast<TableVal*>(*it) != 0) {
            TableVal* tv = dynamic_cast<TableVal*>(*it);
            identifier += tv->ident_;
            identifier += "[]";
        }
    }
    return identifier;
}
