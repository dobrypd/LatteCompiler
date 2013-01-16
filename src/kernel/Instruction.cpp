/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "Instruction.h"

namespace backend
{

Instruction::Instruction()
{
}

x86_Mov::x86_Mov()
{
}

const char* x86_Mov::cstr = "mov";

const char* x86_Mov::c_str() const
{
    return this->cstr;
}

} /* namespace backend */
