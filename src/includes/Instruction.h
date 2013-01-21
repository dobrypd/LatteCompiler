/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <boost/shared_ptr.hpp>

namespace backend
{

enum REGISTERS_T {
    EAX=-1, EDX=-2, EBX=-3, ECX=-4, ESI=-5, EDI=-6, ESP=-7, EBP=-8
};

enum ARG_TYPES_T {
        REGISTER,
        MEMORY,
        CONSTANT_FIELD
};

class InstructionArgument
{
public:
    int base;
    int offset;
    int index;
    int mul;

    int type;
    // if != MEMORY, it stores value in base ie.
    // for constant `$C2` base==2 (constants has only integer identifier)
    // for register `%edx` base==EDX (base==-2)

    std::string str();
};

typedef boost::shared_ptr<InstructionArgument> arg_t;
arg_t arg(int type = 0, int base = 0, int offset = 0, int index = 0,
        int mul = 0);

class Instruction
{
private:
    arg_t arg1;
    arg_t arg2;
    int args;

public:
    const char* cstr;

    Instruction(const char* cstr, int args, arg_t arg1, arg_t arg2);
    virtual ~Instruction() {};

    std::string str() const;
};

namespace instruction
{

class Mov : public Instruction
{
public:
    Mov(arg_t arg1, arg_t arg2);
};

class Push : public Instruction
{
public:
    Push(arg_t arg);
};

class Pop : public Instruction
{
public:
    Pop(arg_t arg);
};

class Lea : public Instruction
{
public:
    Lea(arg_t arg1, arg_t arg2);
};

class Add : public Instruction
{
public:
    Add(arg_t arg1, arg_t arg2);
};

class Sub : public Instruction
{
public:
    Sub(arg_t arg1, arg_t arg2);
};

class Inc : public Instruction
{
public:
    Inc(arg_t arg);
};

class Dec : public Instruction
{
public:
    Dec(arg_t arg);
};

class Imul : public Instruction
{
public:
    Imul(arg_t arg1, arg_t arg2);
};

class Idiv : public Instruction
{
public:
    Idiv(arg_t arg1, arg_t arg2);
};

class And : public Instruction
{
public:
    And(arg_t arg1, arg_t arg2);
};

class Or : public Instruction
{
public:
    Or(arg_t arg1, arg_t arg2);
};

class Xor : public Instruction
{
public:
    Xor(arg_t arg1, arg_t arg2);
};

class Not : public Instruction
{
public:
    Not(arg_t arg);
};

class Neg : public Instruction
{
public:
    Neg(arg_t arg);
};

class Shl : public Instruction
{
public:
    Shl(arg_t arg);
};

class Shr : public Instruction
{
public:
    Shr(arg_t arg);
};

class Jump : public Instruction
{
private:
    std::string label;
public:
    Jump(std::string label);
    std::string str() const;
};

class ConditionJump : public Instruction
{
private:
    std::string label;
    std::string condition_string;
public:
    ConditionJump(std::string label);
    std::string str() const;
};

class Cmp : public Instruction
{
public:
    Cmp(arg_t arg1, arg_t arg2);
};

class Call : public Instruction
{
private:
    std::string label;
public:
    Call(std::string label);
    std::string str() const;
};

class Ret : public Instruction
{
public:
    Ret();
};


} /* namesppace instruction */
} /* namespace backend */

#endif /* INSTRUCTION_H_ */
