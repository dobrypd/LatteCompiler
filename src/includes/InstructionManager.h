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
#include "Environment.h"
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

    frontend::Environment& fr_env;  // To know information about objects.

public:
    InstructionManager(frontend::Environment& fr_env);

    list_it_t begin();
    list_it_t end();

    void write_to_stream(std::ostream& stream);

    void new_block(std::string name);
    void add(Block::instr_ptr_t instruction);

    // Funciton:
    void function_prologue();
    void function_epilogue();

    // Op on reg:
    void top_to_EAX();

    // Allocating:
    void alloc_var(Type* type);
    void alloc_default(Type* type);
    void alloc_array(Type* type); // with size  of array on top of the stack, save ptr on EAX
    void alloc_object(Type* type); // calculate object size by fr_env, save ptr on EAX

    // Stack operations;
    void push_var(int var_offset);
    void add_to_ESP(int value);  // value in baits (not bits)!
    void dereference_stack_top();
    void add_to_stack_top(size_t size);
    void dereference_stack_top();

    // Assign
    void pop_top_to_var(int offset);
    void pop_sec_top_to_addr_on_top();

    // Incrementation
    void increment_var_on_stack(int offset, int inc_by);
    void increment_var_addr_on_top(int inc_by);


    // Arithmetic operations:
    void add_on_stack();
    void sub_on_stack();
    void mul_on_stack();
    void div_on_stack();
    void mod_on_stack();

    // Statics
    void add_const_string(std::string& str); // and push on stack

    // Moving:
    void pop_to_addr_from_EDI();


};


} /* namespace backend */
#endif /* INSTRUCTIONMANAGER_H_ */
