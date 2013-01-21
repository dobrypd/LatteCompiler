/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <ostream>
#include "Absyn.H"
#include "global.h"
#include "InstructionManager.h"

namespace backend
{


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


InstructionManager::InstructionManager(frontend::Environment& fr_env) : fr_env(fr_env)
{
    block_ptr_t block_0(new Block(""));
    this->blocks.push_back(block_0);
}

void InstructionManager::write_to_stream(std::ostream& stream)
{
    for (list_it_t it = this->blocks.begin(); it != this->blocks.end(); it++)
    {
        if ((*it)->get_name() != "")
            stream << (*it)->get_name() << ":" << std::endl;
        for (Block::list_it_t i_it = (*it)->begin(); i_it != (*it)->end();
                i_it++)
        {
            stream << "\t" << (*i_it)->str() << std::endl;
        }
    }
}

void InstructionManager::new_block(int id)
{
    block_ptr_t new_blc(new Block(Block::ident_prefix + int2str(id)));
    this->blocks.push_back(new_blc);
}

void InstructionManager::new_function_block(std::string name)
{
    block_ptr_t new_blc(new Block(name));
    this->blocks.push_back(new_blc);
}

void InstructionManager::add(Block::instr_ptr_t instruction)
{
    this->blocks.back()->add(instruction);
}

void InstructionManager::add(Block::instr_ptr_t i1, Block::instr_ptr_t i2)
{
    this->blocks.back()->add(i1);
    this->blocks.back()->add(i2);
}
void InstructionManager::add(Block::instr_ptr_t i1, Block::instr_ptr_t i2, Block::instr_ptr_t i3)
{
    this->blocks.back()->add(i1);
    this->blocks.back()->add(i2);
    this->blocks.back()->add(i3);

}
void InstructionManager::add(Block::instr_ptr_t i1, Block::instr_ptr_t i2, Block::instr_ptr_t i3, Block::instr_ptr_t i4)
{
    this->blocks.back()->add(i1);
    this->blocks.back()->add(i2);
    this->blocks.back()->add(i3);
    this->blocks.back()->add(i4);
}

InstructionManager::list_it_t InstructionManager::begin()
{
    return this->blocks.begin();
}

InstructionManager::list_it_t InstructionManager::end()
{
    return this->blocks.end();
}

void InstructionManager::alloc_default(Type *type)
{
        Block::instr_ptr_t instr(new instruction::Push(arg(CONSTANT_FIELD, 0)));
        this->add(instr);
    }

    void InstructionManager::function_prologue()
    {
        Block::instr_ptr_t push(new instruction::Push(arg(REGISTER, EBP)));
        Block::instr_ptr_t new_ebp(new instruction::Mov(arg(REGISTER, ESP), arg(REGISTER, EBP)));
        this->add(push, new_ebp);
    }

    void InstructionManager::function_epilogue()
    {
        Block::instr_ptr_t restore_esp(new instruction::Mov(arg(REGISTER, EBP), arg(REGISTER, ESP)));
        Block::instr_ptr_t restore_ebp(new instruction::Pop(arg(REGISTER, EBP)));
        Block::instr_ptr_t ret(new instruction::Ret());
        this->add(restore_esp, restore_ebp, ret);
    }

    void InstructionManager::add_to_ESP(int value)
    {
        if ((debug) and (value == 0))
            std::cerr << "adding 0 to esp has no sense "
            << __FILE__ << ":" << __LINE__ << std::endl;
        Block::instr_ptr_t add(new instruction::Add(arg(CONSTANT_FIELD, value * 4), arg(REGISTER, ESP)));
        this->add(add);
    }

    void InstructionManager::pop_to_EAX()
    {
        Block::instr_ptr_t ret_value(new instruction::Pop(arg(REGISTER, EAX)));
        this->add(ret_value);
    }

    void InstructionManager::alloc_array(Type *type)
    {
    }

    void InstructionManager::alloc_object(Type *type)
    {
    }

    void InstructionManager::add_on_stack()
    {
        Block::instr_ptr_t load(new instruction::Pop(arg(REGISTER, EAX)));
        Block::instr_ptr_t add(new instruction::Add(arg(REGISTER, EAX), arg(MEMORY, ESP)));
        this->add(load, add);
    }

    void InstructionManager::sub_on_stack()
    {
        Block::instr_ptr_t load(new instruction::Pop(arg(REGISTER, EAX)));
        Block::instr_ptr_t sub(new instruction::Sub(arg(REGISTER, EAX), arg(MEMORY, ESP)));
        this->add(load, sub);
    }

    void InstructionManager::mul_on_stack()
    {
        Block::instr_ptr_t load(new instruction::Pop(arg(REGISTER, EAX)));
        Block::instr_ptr_t mul(new instruction::Imul(arg(REGISTER, EAX), arg(MEMORY, ESP)));
        this->add(load, mul);
    }

    void InstructionManager::div_on_stack()
    {
        Block::instr_ptr_t load1(new instruction::Pop(arg(REGISTER, EAX)));
        Block::instr_ptr_t load2(new instruction::Pop(arg(REGISTER, EDX)));
        Block::instr_ptr_t cpy(new instruction::Mov(arg(REGISTER, EAX), arg(REGISTER, EDX)));
        Block::instr_ptr_t sign(new instruction::Sar(arg(CONSTANT_FIELD, 31), arg(REGISTER, EDX)));
        Block::instr_ptr_t div(new instruction::Idiv(arg(MEMORY, ESP)));
        this->add(load1, load2, cpy, sign);
        this->add(div);
    }

    void InstructionManager::mod_on_stack()
    {
        Block::instr_ptr_t load(new instruction::Pop(arg(REGISTER, EAX)));
        Block::instr_ptr_t cpy(new instruction::Mov(arg(REGISTER, EAX), arg(REGISTER, EDX)));
        Block::instr_ptr_t sign(new instruction::Sar(arg(CONSTANT_FIELD, 31), arg(REGISTER, EDX)));
        Block::instr_ptr_t div(new instruction::Idiv(arg(MEMORY, ESP)));
        Block::instr_ptr_t store_result(new instruction::Mov(arg(REGISTER, EAX), arg(MEMORY, ESP)));
        this->add(load, cpy, sign, div);
        this->add(store_result);
    }

    int InstructionManager::cstr_add(std::string & str)
    {
        int no = this->constant_strings.size();
        this->constant_strings.push_back(str);
        return no;
    }

    void InstructionManager::add_const_string(std::string & str)
    {
        int id = this->cstr_add(str);
        Block::instr_ptr_t store(new instruction::Push(arg(CONSTANT_FIELD, id, 1)));
    }

    void InstructionManager::jump(int id)
    {
        Block::instr_ptr_t instr(new instruction::Jump(Block::ident_prefix + int2str(id)));
        this->add(instr);
    }

    void InstructionManager::pop_add_to_ESI()
    {
        Block::instr_ptr_t add(new instruction::Add(arg(MEMORY, ESP), arg(REGISTER, ESI)));
        Block::instr_ptr_t pop_stack(new instruction::Inc(arg(REGISTER, ESP)));
        this->add(add, pop_stack);
    }

    void InstructionManager::dereference_ESI_to_stack()
    {
        Block::instr_ptr_t instr(new instruction::Push(arg(MEMORY, ESI)));
        this->add(instr);
    }

    void InstructionManager::push_literal(int value)
    {
        Block::instr_ptr_t instr(new instruction::Push(arg(CONSTANT_FIELD, value)));
        this->add(instr);
    }

    void InstructionManager::pop_to_addr_from_ESI()
    {
        Block::instr_ptr_t instr(new instruction::Pop(arg(MEMORY, ESI)));
        this->add(instr);
    }

    void InstructionManager::increment_in_ESI(int inc_by)
    {
        Block::instr_ptr_t instr;
        switch (inc_by){
            case -1:
                instr = Block::instr_ptr_t(new instruction::Dec(arg(MEMORY, ESI)));
                break;
            case 0:
                return;
            case 1:
                instr = Block::instr_ptr_t(new instruction::Inc(arg(MEMORY, ESI)));
                break;
            default:
                instr = Block::instr_ptr_t(new instruction::Add(arg(CONSTANT_FIELD, inc_by), arg(MEMORY, ESI)));
                break;
        }
        this->add(instr);
    }

    void InstructionManager::add_to_ESI_val_address(int var_pos)
    {
        Block::instr_ptr_t instr(new instruction::Lea(arg(MEMORY, EBP, -4 * var_pos), arg(REGISTER, ESI)));
        this->add(instr);
    }

    void InstructionManager::dereference_ESI()
    {
        Block::instr_ptr_t instr(new instruction::Mov(arg(MEMORY, ESI), arg(REGISTER, ESI)));
        this->add(instr);
    }

    void InstructionManager::add_to_ESI(int value)
    {
        Block::instr_ptr_t instr(new instruction::Mov(arg(CONSTANT_FIELD, value), arg(REGISTER, ESI)));
        this->add(instr);
    }

    void InstructionManager::neg_on_top()
    {
        Block::instr_ptr_t instr(new instruction::Neg(arg(MEMORY, ESP)));
        this->add(instr);
    }

    void InstructionManager::compare_strings_on_stack()
    {
    }

    void InstructionManager::jump_if(cmp_val_t type, int label_id)
    {
        std::string type_to_str;
        switch (type){
            case LTH:
                type_to_str = "l";
                break;
            case LE:
                type_to_str = "le";
                break;
            case GTH:
                type_to_str = "g";
                break;
            case GE:
                type_to_str = "ge";
                break;
            case EQU:
                type_to_str = "e";
                break;
            case NE:
                type_to_str = "ne";
                break;
            default:
                type_to_str = "ump";
                break;
        }
        Block::instr_ptr_t instr(new instruction::ConditionJump(Block::ident_prefix + int2str(label_id), type_to_str));
        this->add(instr);
    }

    void InstructionManager::jump_if_0(int label_id)
    {
        Block::instr_ptr_t decrease_stack(new instruction::Add(arg(CONSTANT_FIELD, 4), arg(REGISTER, ESP)));
        Block::instr_ptr_t instr(new instruction::Cmp(arg(MEMORY, ESP, 4), arg(CONSTANT_FIELD, 0)));
        this->add(decrease_stack, instr);
        this->jump_if(InstructionManager::EQU, label_id);
    }

    void InstructionManager::jump_if_not0(int label_id)
    {
        Block::instr_ptr_t decrease_stack(new instruction::Add(arg(CONSTANT_FIELD, 4), arg(REGISTER, ESP)));
        Block::instr_ptr_t instr(new instruction::Cmp(arg(MEMORY, ESP), arg(CONSTANT_FIELD, 0)));
        this->add(decrease_stack, instr);
        this->jump_if(InstructionManager::NE, label_id);
    }

    void InstructionManager::cmp_stack()
    {
        // USE EAX
        Block::instr_ptr_t decrease_stack(new instruction::Add(arg(CONSTANT_FIELD, 8), arg(REGISTER, ESP)));
        Block::instr_ptr_t store(new instruction::Mov(arg(MEMORY, ESP, 8), arg(REGISTER, EAX)));
        Block::instr_ptr_t cmp(new instruction::Cmp(arg(MEMORY, ESP, 4), arg(REGISTER, EAX)));
        this->add(decrease_stack, store, cmp);
    }

    void InstructionManager::concat_str_on_stack()
    {
    }

    void InstructionManager::loop(int label_id)
    {
        Block::instr_ptr_t loop(new instruction::Loop(Block::ident_prefix + int2str(label_id)));
        this->add(loop);
    }

    void InstructionManager::dereference_from_ESI_to_ECX_minus_1()
    {
        Block::instr_ptr_t load_array_len(new instruction::Mov(arg(MEMORY, ESI), arg(REGISTER, ECX)));
        Block::instr_ptr_t decrease(new instruction::Dec(arg(REGISTER, ECX)));
        this->add(load_array_len, decrease);
    }

    void InstructionManager::push_ECX()
    {
        Block::instr_ptr_t instr(new instruction::Push(arg(REGISTER, ECX)));
        this->add(instr);
    }

    void InstructionManager::pop_ECX()
    {
        Block::instr_ptr_t instr(new instruction::Pop(arg(REGISTER, ECX)));
        this->add(instr);
    }

    void InstructionManager::push_ESI()
    {
        Block::instr_ptr_t instr(new instruction::Push(arg(REGISTER, ESI)));
        this->add(instr);
    }

    void InstructionManager::dereference_var_to_var(int var_1_pos, int var_2_pos)
    {
    Block::instr_ptr_t mov1(
            new instruction::Mov(
                    arg(MEMORY, EBP, -4 * var_1_pos), arg(REGISTER, EAX)));
    Block::instr_ptr_t mov2(
            new instruction::Mov(
                    arg(REGISTER, EAX), arg(MEMORY, EBP, -4 * var_2_pos)));
    this->add(mov1, mov2);
}

void InstructionManager::var_to_ECX(int var_pos)
{
    Block::instr_ptr_t mov(
            new instruction::Mov(
                    arg(MEMORY, EBP, -4 * var_pos), arg(REGISTER, ECX)));
    this->add(mov);
}

void InstructionManager::add_to_var(int var_pos, int literal)
{
    Block::instr_ptr_t instr;
    arg_t variable = arg(MEMORY, EBP, -4 * var_pos);
    switch(literal) {
    case -1:
        instr = Block::instr_ptr_t(new instruction::Dec(variable));
        break;
    case 0:
        return;
    case 1:
        instr = Block::instr_ptr_t(new instruction::Inc(variable));
        break;
    default:
        instr = Block::instr_ptr_t(new instruction::Add(arg(CONSTANT_FIELD, literal), variable));
        break;
    }
    this->add(instr);
}

} /* namespace backend */
