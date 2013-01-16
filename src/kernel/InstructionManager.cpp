/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <ostream>
#include "InstructionManager.h"

namespace backend
{

InstructionManager::InstructionManager()
{
}

void InstructionManager::write_to_stream(std::ostream& stream)
{
    for (list_it_t it = this->blocks.begin(); it != this->blocks.end(); it++)
    {
        stream << (*it)->get_name() << ":" << std::endl;
        for (Block::list_it_t i_it = (*it)->begin(); i_it != (*it)->end();
                i_it++)
        {
            stream << "  " << (*i_it)->c_str() << std::endl;
        }
        stream << std::endl;
    }
}

void InstructionManager::new_block(std::string name)
{
    block_ptr_t new_blc(new Block(name));
    this->blocks.push_back(new_blc);
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

Block::Block()
{
}



Block::Block(std::string name) : block_name(name)
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

} /* namespace backend */
