/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef PARSERMANAGER_H_
#define PARSERMANAGER_H_

#include <stdio.h>
#include "Absyn.H"

namespace frontend
{

class ParserManager
{
public:
    ParserManager(FILE* input);
    Visitable* get_ast();
};

} /* namespace frontend */
#endif /* PARSERMANAGER_H_ */
