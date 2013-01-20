/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <ostream>
#include <sstream>
#include "Absyn.H"
#include "InstructionManager.h"

namespace backend
{

InstructionManager::InstructionManager(frontend::Environment& fr_env) : fr_env(fr_env)
{
    block_ptr_t block_0(new Block(0));
    this->blocks.push_back(block_0);
}

void InstructionManager::write_to_stream(std::ostream& stream)
{
    for (list_it_t it = this->blocks.begin(); it != this->blocks.end(); it++)
    {
        stream << (*it)->get_name() << ":" << std::endl;
        for (Block::list_it_t i_it = (*it)->begin(); i_it != (*it)->end();
                i_it++)
        {
            stream << "\t" << (*i_it)->c_str() << std::endl;
        }
    }
}

void InstructionManager::new_block(int id)
{
    block_ptr_t new_blc(new Block(Block::ident_prefix + int2str(id)));
    this->blocks.push_back(new_blc);
}

void InstructionManager::alloc_scalar_var()
{
}

void InstructionManager::alloc_object_var()
{
}

void InstructionManager::alloc_array_var()
{
}

void InstructionManager::add(Block::instr_ptr_t instruction)
{
    this->blocks.back()->add(instruction);
}

InstructionManager::list_it_t InstructionManager::begin()
{
    return this->blocks.begin();
}

InstructionManager::list_it_t InstructionManager::end()
{
    return this->blocks.end();
}

const char *Block::ident_prefix = "_L";
Block::Block()
{
}

Block::Block(std::string name)
:block_name(name)
{
}

Block::list_it_t Block::begin()
{
    return this->i_list.begin();
}

Block::list_it_t Block::end()
{
    return this->i_list.end();
}

std::string & Block::get_name()
{
    return this->block_name;
}

void Block::add(instr_ptr_t instruction)
{
    this->i_list.push_back(instruction);
}

void InstructionManager::alloc_var(Type *type)
{
}

void InstructionManager::alloc_default(Type *type)
{
}

void InstructionManager::function_prologue()
{
}

void InstructionManager::function_epilogue()
{
}

void InstructionManager::add_to_ESP(int value)
{
}

void InstructionManager::top_to_EAX()
{
}

void InstructionManager::alloc_array(Type *type)
{
}

void InstructionManager::alloc_object(Type *type)
{
}

void InstructionManager::pop_top_to_var(int offset)
{
}

void InstructionManager::pop_sec_top_to_addr_on_top()
{
}

void InstructionManager::increment_var_on_stack(int offset, int inc_by)
{
}

void InstructionManager::increment_var_addr_on_top(int inc_by)
{
}

void InstructionManager::add_on_stack()
{
}

void InstructionManager::sub_on_stack()
{
}

void InstructionManager::mul_on_stack()
{
}

void InstructionManager::div_on_stack()
{
}

void InstructionManager::mod_on_stack()
{
}

void InstructionManager::add_const_string(std::string & str)
{
}

void InstructionManager::new_function_block(std::string name)
{
}

void InstructionManager::jump(int id)
{
}

void InstructionManager::pop_add_to_ESI()
{
}

void InstructionManager::dereference_ESI_to_stack()
{
}

void InstructionManager::push_literal(int value)
{
}

void InstructionManager::pop_to_addr_from_ESI()
{
}

void InstructionManager::increment_ESI(int inc_by)
{
}

void InstructionManager::increment_in_ESI(int inc_by)
{
}

void InstructionManager::add_to_ESI_val_address(int value_position)
{
}

void InstructionManager::dereference_ESI()
{
}

void InstructionManager::add_to_ESI(int value)
{
}

void InstructionManager::neg_on_top()
{
}

void InstructionManager::pop_to_addr_from_EDI()
{
}

} /* namespace backend */
