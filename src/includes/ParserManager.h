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
private:
    Visitable* main_visitable;
    FILE* prog_file;
public:
    ParserManager(FILE* input);
    bool try_to_parse();
    Visitable* get();
};

} /* namespace frontend */
#endif /* PARSERMANAGER_H_ */
