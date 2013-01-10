/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */
#include <string>
#include <fstream>
#include "Absyn.H"
#include "Environment.h"
#include "ASCreator.h"

namespace backend
{

ASCreator::ASCreator(std::string output_file_name,
        frontend::Environment& env)
    : assembly_file_name(output_file_name),
      assembly(0),
      env(env)
{
}

void ASCreator::generate(Visitable *ast_root)
{
    // Open file.
    std::ofstream file;
    file.open(this->assembly_file_name.c_str());

    std::stringstream assembly_current_block(std::stringstream::in
            | std::stringstream::out);
    this->assembly = &assembly_current_block;

    // Generate.
    //ast_root->accept(this);

    // Stream to file;
    file << (this->assembly->str());

    // Close file.
    file.close();
}

} /* namespace backend */
