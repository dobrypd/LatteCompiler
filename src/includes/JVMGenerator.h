/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef JVMGENERATOR_H_
#define JVMGENERATOR_H_

#include "Absyn.H"

namespace jvm
{

class JVMGenerator
{
public:
    JVMGenerator(const char* output_file_name);

    void generate(Visitable* ast_root);
};

} /* namespace jvm */
#endif /* JVMGENERATOR_H_ */
