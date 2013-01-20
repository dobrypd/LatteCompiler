/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "global.h"
#include "Instruction.h"

namespace backend
{

std::string Argument::str()
{
    return int2str(this->base); // TODO:
}

arg_t arg(int base = 0, int type = 0, bool dereference = false,
        int offset = 0, int index = 0, int mul = 0) {
    boost::shared_ptr<Argument> argument(new Argument);
    argument->base = base;
    argument->offset = offset;
    argument->index = index;
    argument->mul = mul;
    argument->type = type;
    argument->dereference = dereference;
    return argument;
}

Instruction::Instruction(arg_t arg1 = arg_t(), arg_t arg2 = arg_t()) :
        arg1(arg1), arg2(arg2), cstr("bare_instruction"), args(0)
{
}

std::string Instruction::str() const
{
    switch (this->args) {
    case 0:
        return this->cstr;
    case 1:
        return std::string(this->cstr) + this->arg1->str();
    case 2:
        return std::string(this->cstr) + this->arg1->str()
                + this->arg2->str();
    }
    return std::string();
}

namespace instruction
{

Mov::Mov(arg_t arg1, arg_t arg2) : Instruction(arg1, arg2),
        cstr("movl"), args(2)
{ }

Push::Push(arg_t arg) : Instruction(arg),
        cstr("pushl"), args(2)
{ }

Pop::Pop(arg_t arg) : Instruction(arg),
        cstr("popl"), args(2)
{ }

} /* namespace instruction */

} /* namespace backend */
