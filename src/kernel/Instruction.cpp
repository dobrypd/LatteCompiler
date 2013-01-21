/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <sstream>
#include "global.h"
#include "Instruction.h"

namespace backend
{

void InstructionArgument::address_str(std::stringstream& ss, int value)
{
    switch (value) {
    case EAX: ss << "%eax"; break;
    case EDX: ss << "%edx"; break;
    case EBX: ss << "%ebx"; break;
    case ECX: ss << "%ecx"; break;
    case ESI: ss << "%esi"; break;
    case EDI: ss << "%edi"; break;
    case ESP: ss << "%esp"; break;
    case EBP: ss << "%ebp"; break;
    default:
        ss << value;
        break;
    }
}

std::string InstructionArgument::str()
{
    std::stringstream ss;
    if ((this->offset != 0) and (this->type != CONSTANT_FIELD))
        this->address_str(ss, this->offset);
    if (this->type == MEMORY) ss << "(";
    if (this->type == CONSTANT_FIELD)
        ss << ((this->offset != 0) ? "$C" : "$");

    this->address_str(ss, this->base);

    if ((this->index != 0) or (this->mul != 0)) {
        ss << ",";
        this->address_str(ss, this->index);
    }
    if (this->mul != 0) {
        ss << ",";
        this->address_str(ss, this->mul);
    }
    if (this->type == MEMORY) ss << ")";

    return ss.str();
}

arg_t arg(int type, int base, int offset, int index, int mul) {
    boost::shared_ptr<InstructionArgument> argument(new InstructionArgument);
    argument->base = base;
    argument->offset = offset;
    argument->index = index;
    argument->mul = mul;
    argument->type = type;
    return argument;
}

Instruction::Instruction(const char* cstr="bare_instruction", int args = 0,
        arg_t arg1 = arg_t(), arg_t arg2 = arg_t())
        : arg1(arg1), arg2(arg2), args(args), cstr(cstr)
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
        return std::string(this->cstr) + " " + this->arg1->str();
    case 2:
        return std::string(this->cstr) + " " + this->arg1->str() + ", "
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
Idiv::Idiv(arg_t arg) : Instruction("idivl", 1, arg) { }
And::And(arg_t arg1, arg_t arg2) : Instruction("andl", 2, arg1, arg2) { }
Or::Or(arg_t arg1, arg_t arg2) : Instruction("orl", 2, arg1, arg2) { }
Xor::Xor(arg_t arg1, arg_t arg2) : Instruction("xorl", 2, arg1, arg2) { }
Not::Not(arg_t arg) : Instruction("not", 1, arg) { }
Neg::Neg(arg_t arg) : Instruction("neg", 1, arg) { }
Sar::Sar(arg_t arg1, arg_t arg2) : Instruction("sarl", 2, arg1, arg2) { }
Shl::Shl(arg_t arg1, arg_t arg2) : Instruction("shl", 2, arg1, arg2) { }
Shr::Shr(arg_t arg1, arg_t arg2) : Instruction("shr", 2, arg1, arg2) { }
Jump::Jump(std::string label) : Instruction("jmp"), label(label) { }
Loop::Loop(std::string label) : Instruction("loop"), label(label) { }
ConditionJump::ConditionJump(std::string label, std::string condition_string)
    : Instruction("j"), label(label), condition_string(condition_string) { }
Cmp::Cmp(arg_t arg1, arg_t arg2) : Instruction("cmp", 2, arg1, arg2) { }
Ret::Ret() : Instruction("ret") { }

std::string Jump::str() const {
    return (std::string(this->cstr) + " " + this->label);
}

std::string Loop::str() const {
    return (std::string(this->cstr) + " " + this->label);
}

std::string Call::str() const {
    return (std::string(this->cstr) + " " + this->label);
}

std::string ConditionJump::str() const {
    return (std::string(this->cstr) + this->condition_string + " " + this->label);
}

} /* namespace instruction */

} /* namespace backend */
