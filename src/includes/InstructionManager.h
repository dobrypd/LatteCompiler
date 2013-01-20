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

    void alloc_scalar_var();
    void alloc_object_var();
    void alloc_array_var();

    frontend::Environment& fr_env;  // To know information about objects.

public:
    InstructionManager(frontend::Environment& fr_env);

    list_it_t begin();
    list_it_t end();

    void write_to_stream(std::ostream& stream);

    void new_block(int id);
    void new_function_block(std::string name); // as new block but with where call can applicate
    void add(Block::instr_ptr_t instruction);

    // Bigger fragments/ syscals
    void compare_strings_on_stack();
    void concat_str_on_stack();

    // Jumps / calls
    void jump(int id);

    enum cmp_val_t {LTH, LE, GTH, GE, EQU, NE};
    void jump_if(cmp_val_t type, int label_id);
    void jump_if_0(int label_id);
    void jump_if_not0(int label_id);
    void cmp_stack();
    void loop(int label_id);

    // Funciton:
    void function_prologue();
    void function_epilogue();

    // Op on reg:
    void top_to_EAX();
    void dereference_from_ESI_to_ECX_minus_1();
    void push_ECX();
    void pop_ECX();

    // Allocating:
    void alloc_var(Type* type);
    void alloc_default(Type* type);
    void alloc_array(Type* type); // with size  of array on top of the stack, save ptr on EAX
    void alloc_object(Type* type); // calculate object size by fr_env, save ptr on EAX

    // Stack operations;
    void add_to_ESP(int value);  // value in baits (not bits)!
    void pop_add_to_ESI();
    void dereference_ESI_to_stack();
    void dereference_stack_top_to_ECX();
    void push_literal(int value);
    void pop_to_addr_from_ESI();
    void push_ESI();

    // Assign
    void pop_top_to_var(int offset);
    void pop_sec_top_to_addr_on_top();

    // Incrementation
    void increment_var_on_stack(int offset, int inc_by);
    void increment_var_addr_on_top(int inc_by);
    void increment_ESI(int inc_by);
    void increment_in_ESI(int inc_by); // inc in address in ESI
    void increment_ECX(int inc_by);


    // Arithmetic operations:
    void add_to_ESI_val_address(int value_position);
    void dereference_ESI();
    void add_to_ESI(int value);
    void neg_on_top();
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
