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
    virtual ~Instruction() {};

    virtual const char* c_str() const = 0;
};

class x86_Mov : public Instruction
{
private:
    static const char* cstr;
public:
    x86_Mov();

    virtual const char* c_str() const;
};

} /* namespace backend */
#endif /* INSTRUCTION_H_ */
