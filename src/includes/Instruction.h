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
        ADDRESS,
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
public:
    Instruction(arg_t arg1, arg_t arg2);
    virtual ~Instruction() {};

    std::string str() const;

    const char* cstr;
    int args;
};

namespace instruction
{

class Mov : public Instruction
{
    Mov(arg_t arg1, arg_t arg2);
    const char* cstr;
    int args;
};

class Push : public Instruction
{
    Push(arg_t arg);
    const char* cstr;
    int args;
};

class Pop : public Instruction
{
    Pop(arg_t arg);
    const char* cstr;
    int args;
};

} /* namesppace instruction */
} /* namespace backend */
#endif /* INSTRUCTION_H_ */
