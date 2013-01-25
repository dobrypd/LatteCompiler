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

ASCreator::ASCreator(std::string original_file_name,
        std::string output_file_name,
        frontend::Environment& env)
    : creator(Creator_x86(this->instruction_manager, env)),
      assembly_file_name(output_file_name),
      original_file_name(original_file_name),
      env(env)
{
}

void ASCreator::peep_hole_optimization()
{
    if (debug) std::cout << "Peep hole optimization." << std::endl;
}

void ASCreator::save_in_file()
{
    std::ofstream file;
    file.open(this->assembly_file_name.c_str());

    this->write_file_prologue(file);
    this->instruction_manager.write_to_stream(file);
    this->write_file_epilogue(file);

    file.close();
}

void ASCreator::write_file_prologue(std::ostream& stream)
{
    stream << "\t.file \"" << this->original_file_name << "\"" << std::endl;
}

void ASCreator::write_file_epilogue(std::ostream& stream)
{

}

ASCreator::~ASCreator()
{
}

void ASCreator::generate(Visitable *ast_root)
{
    ast_root->accept(&this->creator);

    this->peep_hole_optimization();
    this->save_in_file();
}

} /* namespace backend */
