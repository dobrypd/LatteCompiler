/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <map>
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include "Environment.h"
#include "global.h"

namespace frontend
{

Environment::Environment()
{
    // Create empty environment.
    Environment::MapPtr empty(new Environment::MapType());
    this->env_v.push_back(empty);
    if (debug)
        std::cout << "Initializing new environment at address: " << this << std::endl;
}

void Environment::submerge()
{
    Environment::MapPtr empty(new Environment::MapType());
    this->env_v.push_back(empty);
}


void Environment::emerge()
{
    this->env_v.pop_back();
}


void Environment::prepare()
{
    if (debug)
        std::cout << "Preparing environment to new " << this->env_v.size() << " block." << std::endl;
    this->submerge();
}

void Environment::back()
{
    if (debug)
            std::cout << "Back from " << this->env_v.size() << " block." << std::endl;
    if (this->env_v.size() > 0)
    {
        this->emerge();
    }
    else
    {
        // error
        throw 0;
    }
}

void Environment::add_variable()
{
}

void Environment::add_function()
{
}

void Environment::set_variable()
{
}

void Environment::set_function()
{
}

bool Environment::lookup_variable()
{
}

bool Environment::lookup_function()
{
}

} /* namespace frontend */
