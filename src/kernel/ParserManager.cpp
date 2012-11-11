/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "ParserManager.h"
#include <stdio.h>
#include <iostream>

namespace frontend
{

ParserManager::ParserManager(FILE* input)
{
        std::cout << "Parser manager initialization." << std::endl;
}

Visitable* ParserManager::get_ast()
{
    return 0;
}

} /* namespace frontend */
