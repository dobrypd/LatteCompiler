/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

namespace backend
{

class Instruction
{
public:
    Instruction();
};

class x86_Mov : public Instruction
{
public:
    x86_Mov();
};

} /* namespace backend */
#endif /* INSTRUCTION_H_ */
