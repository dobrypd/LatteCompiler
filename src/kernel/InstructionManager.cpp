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

void InstructionManager::to_stream(std::ostream& stream)
{
    stream << "test";
}

} /* namespace backend */
