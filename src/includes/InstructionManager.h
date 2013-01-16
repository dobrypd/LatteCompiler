/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef INSTRUCTIONMANAGER_H_
#define INSTRUCTIONMANAGER_H_


#include <ostream>
#include <list>
#include <boost/shared_ptr.hpp>
#include "Instruction.h"

namespace backend
{

class Block
{
public:
    typedef boost::shared_ptr<Instruction> instr_ptr_t;
    typedef std::list<instr_ptr_t> instructions_list_t;
    typedef std::list<instr_ptr_t>::iterator list_it_t;

private:
    instructions_list_t i_list;

    std::string block_name;

    Block();
public:

    Block(std::string name);

    list_it_t begin();
    list_it_t end();

    std::string& get_name();

    void add(instr_ptr_t instruction);
};

class InstructionManager
{
public:
    typedef boost::shared_ptr<Block> block_ptr_t;
    typedef std::list<block_ptr_t> blocks_list_t;
    typedef std::list<block_ptr_t>::iterator list_it_t;

private:
    blocks_list_t blocks;

public:
    InstructionManager();

    list_it_t begin();
    list_it_t end();

    void write_to_stream(std::ostream& stream);

    void new_block(std::string name);
    void add(Block::instr_ptr_t instruction);
};


} /* namespace backend */
#endif /* INSTRUCTIONMANAGER_H_ */
