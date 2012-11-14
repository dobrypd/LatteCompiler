/*
 * ErrorHandler.cpp
 *
 *  Created on: 14-11-2012
 *      Author: piotrek
 */

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include "ErrorHandler.h"

namespace frontend
{

ErrorHandler::ErrorHandler(FILE* file) : file(file)
{
}

void ErrorHandler::handle(int line, error_type error)
{
    this->errors.push_back(std::make_pair(line, error));
}

char* ErrorHandler::get_line(int pos)
{

    if (pos > this->current_pos)
    {

    }
    else
    {
        this->pos = 0;
    }
    return NULL;//TODO: memory leaks!
}

void ErrorHandler::flush()
{
    std::sort(this->errors.begin(), this->errors.end());
    for(std::vector<std::pair<int, error_type> >::iterator it = this->errors.begin(); it != this->errors.end(); it++)
    {
        std::cout << (it->second.first ? "Error" : "Warning") << " at line: " << it->first << " :" << it->second.second << std::endl;
        std::cout << "\t" << this->get_line(it->first) << std::endl;
    }
}

} /* namespace frontend */
