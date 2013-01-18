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
#include "Absyn.H"
#include "Instruction.h"

namespace backend
{

class Block
{
public:
    typedef boost::shared_ptr<Instruction> instr_ptr_t;
    typedef std::list<instr_ptr_t> instructions_list_t;
    typedef std::list<instr_ptr_t>::iterator list_it_t;

    static const char* ident_prefix;
private:
    instructions_list_t i_list;
    std::string block_name;

    Block();

    static long current_ident;
public:

    Block(std::string name);

    list_it_t begin();
    list_it_t end();

    std::string& get_name();

    void add(instr_ptr_t instruction);

    static std::string new_ident();
};

class InstructionManager
{
public:
    typedef boost::shared_ptr<Block> block_ptr_t;
    typedef std::list<block_ptr_t> blocks_list_t;
    typedef std::list<block_ptr_t>::iterator list_it_t;

private:
    blocks_list_t blocks;

    void alloc_scalar_var();
    void alloc_object_var();
    void alloc_array_var();

public:
    InstructionManager();

    list_it_t begin();
    list_it_t end();

    void write_to_stream(std::ostream& stream);

    void new_block(std::string name);
    void add(Block::instr_ptr_t instruction);

    // Allocating:
    void alloc_var(Type* type);
    void alloc_default(Type* type);
    void alloc_array(size_t size);

    // Stack operations;
    void pop_from_the_stack();
    void pop_deeper_on_stack(int offset);

    // Getting adresses:
    void get_addr_to_EDI(ListStructuredIdent* ident);

    // Assign
    void assign_scalar_from_top(int var_pos);

    // Moving:
    void pop_to_addr_from_EDI();


};


} /* namespace backend */
#endif /* INSTRUCTIONMANAGER_H_ */
