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

Instruction::Instruction(const char* cstr="bare_instruction", int args = 0,
        arg_t arg1 = arg_t(), arg_t arg2 = arg_t())
        : arg1(arg1), arg2(arg2), cstr(cstr), args(args)
{
    if ((debug) and (this->args == 2 )
            and (this->arg1->type == this->arg2->type)
            and (this->arg1->type == MEMORY)) {
        std::cerr << "CANNOT USE INSTRUCTION WITH BOTH ARGUMENTS IN MEMORY!!!"
                << std::endl;
    }
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

Mov::Mov(arg_t arg1, arg_t arg2) : Instruction("movl", 2, arg1, arg2) { }
Push::Push(arg_t arg) : Instruction("pushl", 1, arg) { }
Pop::Pop(arg_t arg) : Instruction("popl", 1, arg) { }
Lea::Lea(arg_t arg1, arg_t arg2) : Instruction("lea", 2, arg1, arg2) { }
Add::Add(arg_t arg1, arg_t arg2) : Instruction("addl", 2, arg1, arg2) { }
Sub::Sub(arg_t arg1, arg_t arg2) : Instruction("subl", 2, arg1, arg2) { }
Inc::Inc(arg_t arg) : Instruction("incl", 1, arg) { }
Dec::Dec(arg_t arg) : Instruction("decl", 1, arg) { }
Imul::Imul(arg_t arg1, arg_t arg2) : Instruction("imull", 2, arg1, arg2) { }
Idiv::Idiv(arg_t arg1, arg_t arg2) : Instruction("idivl", 2, arg1, arg2) { }
And::And(arg_t arg1, arg_t arg2) : Instruction("andl", 2, arg1, arg2) { }
Or::Or(arg_t arg1, arg_t arg2) : Instruction("orl", 2, arg1, arg2) { }
Xor::Xor(arg_t arg1, arg_t arg2) : Instruction("xorl", 2, arg1, arg2) { }
Not::Not(arg_t arg) : Instruction("not", 1, arg) { }
Neg::Neg(arg_t arg) : Instruction("neg", 1, arg) { }
Shl::Shl(arg_t arg) : Instruction("shl", 1, arg) { }
Shr::Shr(arg_t arg) : Instruction("shr", 1, arg) { }
Jump::Jump(std::string label) : Instruction("jmp"), label(label) { }
ConditionJump::ConditionJump(std::string label) : Instruction("j"), label(label) { }
Cmp::Cmp(arg_t arg1, arg_t arg2) : Instruction("cmp", 2, arg1, arg2) { }
Ret::Ret() : Instruction("ret") { }

std::string Jump::str() const {
    return std::string(this->cstr) + " " + this->label;
}

std::string Call::str() const {
    return std::string(this->cstr) + " " + this->label;
}

std::string ConditionJump::str() const {
    return std::string(this->cstr) + " " + this->label;
}

} /* namespace instruction */

} /* namespace backend */
