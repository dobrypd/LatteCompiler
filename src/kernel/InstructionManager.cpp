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

InstructionManager::InstructionManager()
{
    block_ptr_t block_0(new Block(Block::new_ident()));
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

void InstructionManager::new_block(std::string name)
{
    block_ptr_t new_blc(new Block(name));
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

long Block::current_ident = 0;
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

std::string Block::new_ident()
{
    Block::current_ident++;
    std::stringstream ss;
    ss << Block::current_ident;
    std::string r_str(Block::ident_prefix);
    return r_str + ss.str();
}

void InstructionManager::alloc_var(Type *type)
{
}

void InstructionManager::alloc_default(Type *type)
{
}

void InstructionManager::alloc_array(size_t size)
{
}

void InstructionManager::pop_from_the_stack()
{
}

void InstructionManager::pop_deeper_on_stack(int offset)
{
}

void InstructionManager::get_addr_to_EDI(ListStructuredIdent *ident)
{
}

void InstructionManager::assign_scalar_from_top(int var_pos)
{
}

void InstructionManager::pop_to_addr_from_EDI()
{
}

} /* namespace backend */
