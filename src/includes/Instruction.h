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
    EAX, EDX, EBX, ECX, ESI, EDI, ESP, EBP
};

enum ARG_TYPES_T {
        REGISTER,
        MEMORY,
        CONSTANT_FIELD
};

class Argument
{
public:
    int base;
    int offset;
    int index;
    int mul;

    int type;
    bool dereference;

    std::string str();
};

typedef boost::shared_ptr<Argument> arg_t;

arg_t arg(int base, int type, bool dereference, int offset, int index,
        int mul);

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
    Mov(arg_t arg1, arg_t arg2);
};

class Push : public Instruction
{
    Push(arg_t arg);
};

class Pop : public Instruction
{
    Pop(arg_t arg);
};

class Lea : public Instruction
{
    Lea(arg_t arg1, arg_t arg2);
};

class Add : public Instruction
{
    Add(arg_t arg1, arg_t arg2);
};

class Sub : public Instruction
{
    Sub(arg_t arg1, arg_t arg2);
};

class Inc : public Instruction
{
    Inc(arg_t arg);
};

class Dec : public Instruction
{
    Dec(arg_t arg);
};

class Imul : public Instruction
{
    Imul(arg_t arg1, arg_t arg2);
};

class Idiv : public Instruction
{
    Idiv(arg_t arg1, arg_t arg2);
};

class And : public Instruction
{
    And(arg_t arg1, arg_t arg2);
};

class Or : public Instruction
{
    Or(arg_t arg1, arg_t arg2);
};

class Xor : public Instruction
{
    Xor(arg_t arg1, arg_t arg2);
};

class Not : public Instruction
{
    Not(arg_t arg);
};

class Neg : public Instruction
{
    Neg(arg_t arg);
};

class Shl : public Instruction
{
    Shl(arg_t arg);
};

class Shr : public Instruction
{
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
    Ret();
};


} /* namesppace instruction */
} /* namespace backend */

#endif /* INSTRUCTION_H_ */
