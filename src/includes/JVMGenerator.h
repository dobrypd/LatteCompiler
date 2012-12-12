/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef JVMGENERATOR_H_
#define JVMGENERATOR_H_

#include <string>
#include <fstream>
#include "Absyn.H"

namespace jvm
{

class JVMGenerator
{
private:
    std::string class_file_name;
    std::string jasmin_file_name;
    std::ofstream jvm_output_file;

public:
    JVMGenerator(const char* output_file_name);

    void generate(Visitable* ast_root);
};

} /* namespace jvm */
#endif /* JVMGENERATOR_H_ */
