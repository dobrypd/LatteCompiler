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
    std::cout << "Parser manager initialization." << std::endl;
}

bool ParserManager::try_to_parse()
{
    this->prog = pProg(this->prog_file);
    if (this->prog)
    {
        return true;
    }
    return false;
}

Prog* ParserManager::get_prog()
{
    return this->prog;
}

} /* namespace frontend */
