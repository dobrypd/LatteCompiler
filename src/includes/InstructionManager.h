/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef INSTRUCTIONMANAGER_H_
#define INSTRUCTIONMANAGER_H_


#include <ostream>
#include <list>
#include "Instruction.h"

namespace backend
{

class Block
{
private:
    std::list<Instruction> i_list;
public:
    Block();
};

class InstructionManager
{
private:
    std::list<Block> blocks;
public:
    InstructionManager();

    void to_stream(std::ostream& stream);

    //void add();
};

} /* namespace backend */
#endif /* INSTRUCTIONMANAGER_H_ */
