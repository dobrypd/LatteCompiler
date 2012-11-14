/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */
#include <stdio.h>
#include <iostream>
#include "ParserManager.h"
#include "Parser.H"

namespace frontend
{

ParserManager::ParserManager(FILE* input) : prog_file(input)
{
}

bool ParserManager::try_to_parse()
{
    this->main_visitable = pProg(this->prog_file);
    if (this->main_visitable)
    {
        return true;
    }
    return false;
}

Visitable* ParserManager::get()
{
    return this->main_visitable;
}

} /* namespace frontend */
