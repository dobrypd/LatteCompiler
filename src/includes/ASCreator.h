/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef ASCREATOR_H_
#define ASCREATOR_H_

#include <string>
#include <ostream>
#include "Absyn.H"
#include "Environment.h"
#include "Creator_x86.h"
#include "InstructionManager.h"


namespace backend
{

class ASCreator
{
private:
    InstructionManager instruction_manager;
    Creator_x86 creator;
    std::string assembly_file_name;
    std::string original_file_name;

    frontend::Environment& env;

    void peep_hole_optimization();
    void save_in_file();
    void write_file_prologue(std::ostream& stream);
    void write_file_epilogue(std::ostream& stream);
public:
    ASCreator(std::string original_file_name, std::string output_file_name,
            frontend::Environment& env);

    void generate(Visitable *ast_root);
};

} /* namespace backend */
#endif /* ASCREATOR_H_ */
