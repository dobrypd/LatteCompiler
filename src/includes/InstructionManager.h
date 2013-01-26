/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef INSTRUCTIONMANAGER_H_
#define INSTRUCTIONMANAGER_H_


#include <ostream>
#include <list>
#include <map>
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
    std::map<std::string, int> constant_strings;
    std::map<std::string, frontend::Environment::MethodsPtr> virtual_tables;
    int constant_strings_no;

    int cstr_add(std::string& string);
    void write_constant_strings(std::ostream& stream);
    void write_virtual_tables(std::ostream& stream);

    static const char* malloc_name;
public:
    InstructionManager();

    list_it_t begin();
    list_it_t end();

    void write_to_stream(std::ostream& stream);

    void new_block(int id);
    void new_function_block(std::string name); // as new block but with where call can applicate
    void add(Block::instr_ptr_t instruction);
    void add(Block::instr_ptr_t i1, Block::instr_ptr_t i2);
    void add(Block::instr_ptr_t i1, Block::instr_ptr_t i2,
            Block::instr_ptr_t i3);
    void add(Block::instr_ptr_t i1, Block::instr_ptr_t i2,
            Block::instr_ptr_t i3, Block::instr_ptr_t i4);

    // Statics
    void new_vtable(std::string class_name,
            frontend::Environment::MethodsPtr list_of_methods);

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
    void function_call(std::string ident);
    void method_call(int position);

    // Op on reg:
    void pop_to_EAX();
    void dereference_from_ESI_to_ECX_minus_1();
    void push_ECX();
    void pop_ECX();

    // Allocating:
    void alloc_default(Type* type);
    void alloc_array(); // with size  of array on top of the stack, save ptr on EAX
    void alloc_object(std::string class_name, int all_fields);

    // Stack operations;
    void add_to_ESP(int value);  // value in baits (not bits)!
    void pop_add_to_ESI();
    void dereference_ESI_to_stack();
    void push_literal(int value);
    void pop_to_addr_from_ESI();
    void push_ESI();
    void pop_ESI();
    void push_EAX();
    void pop_stack_snd_to_ESI();

    // Incrementation
    void increment_in_ESI(int inc_by); // inc in address in ESI


    // Operations on local variables:
    void dereference_var_to_var(int var_1_pos, int var_2_pos);
    void var_to_ECX(int var_pos);
    void add_to_var(int var_pos, int literal);
    void add_to_ESI_val_address(int var_pos);
    void dereference_ESI();
    void add_to_ESI(int value);

    // Arithmetic operations:
    void neg_on_top();
    void add_on_stack();
    void sub_on_stack();
    void mul_on_stack();
    void div_on_stack();
    void mod_on_stack();

    // Statics
    void add_const_string(std::string& str); // and push on stack
};


} /* namespace backend */
#endif /* INSTRUCTIONMANAGER_H_ */
