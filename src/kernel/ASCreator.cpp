/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */
#include <iostream>
#include <string>
#include <fstream>
#include "Absyn.H"
#include "Environment.h"
#include "Creator_x86.h"
#include "global.h"
#include "ASCreator.h"

namespace backend
{

ASCreator::ASCreator(std::string output_file_name,
        frontend::Environment& env)
    : creator(Creator_x86(this->instruction_manager)),
      assembly_file_name(output_file_name),
      assembly(0),
      env(env)
{
}

void ASCreator::peep_hole_optimization()
{
    // TODO:
    if (debug) std::cout << "Peep hole optimization.";
}

void ASCreator::save_in_file()
{
    std::ofstream file;
    file.open(this->assembly_file_name.c_str());

    this->instruction_manager.write_to_stream(file);

    file.close();
}

void ASCreator::generate(Visitable *ast_root)
{
    ast_root->accept(&this->creator);

    this->peep_hole_optimization();
    this->save_in_file();
}

} /* namespace backend */
