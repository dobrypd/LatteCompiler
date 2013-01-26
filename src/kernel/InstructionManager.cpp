/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <ostream>
#include "Absyn.H"
#include "global.h"
#include "InstructionManager.h"
#include "Environment.h"
#include "Creator_x86.h"

namespace backend
{


const char * Block::ident_prefix = "._L";

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


const char * InstructionManager::malloc_name = "malloc";
// Add strings from runtime
const char * InstructionManager::add_strings = "addStrings_name_with_name_mangling_5594478149272763309697";

InstructionManager::InstructionManager(): constant_strings_no(0)
{
    block_ptr_t block_0(new Block(""));
    this->blocks.push_back(block_0);
}


void InstructionManager::write_constant_strings(std::ostream& stream)
{
    if (this->constant_strings.size() > 0)
        stream << "\t.section .rodata" << std::endl;
    for (std::map<std::string, int>::iterator
            it = this->constant_strings.begin();
            it != this->constant_strings.end(); it++) {
        stream << "C" << it->second << ":" << std::endl;
        stream << "\t.string \"" << it->first << "\"" << std::endl;
    }
}

void InstructionManager::write_virtual_tables(std::ostream& stream)
{
    for (std::map<std::string, frontend::Environment::MethodsPtr>::iterator
            it = this->virtual_tables.begin();
            it != this->virtual_tables.end(); it++) {
        stream << Creator_x86::v_table_ident(it->first) << ":" << std::endl;
        for (std::vector<frontend::Environment::PairOfStrPtr>::iterator
                methods_it = it->second->begin();
                methods_it != it->second->end(); methods_it++) {
            stream << "\t.long " << Creator_x86::method_ident(*(methods_it->first), *(methods_it->second)) << std::endl;
        }
    }
}


void InstructionManager::write_to_stream(std::ostream& stream)
{
    this->write_constant_strings(stream);
    stream << "\t.text" << std::endl;
    this->write_virtual_tables(stream);
    stream << ".globl main" << std::endl;
    stream << "\t.type main, @function" << std::endl;

    for (list_it_t it = this->blocks.begin(); it != this->blocks.end(); it++)
    {
        if ((*it)->get_name() != "")
            stream << (*it)->get_name() << ":" << std::endl;
        for (Block::list_it_t i_it = (*it)->begin(); i_it != (*it)->end();
                i_it++)
        {
            stream << "\t";
            if (check_is<instruction::Jump*>((*i_it).get())) {
                stream << dynamic_cast<instruction::Jump*>((*i_it).get())->str();
            } else  if (check_is<instruction::Loop*>((*i_it).get())) {
                stream << dynamic_cast<instruction::Loop*>((*i_it).get())->str();
            } else  if (check_is<instruction::ConditionJump*>((*i_it).get())) {
                stream << dynamic_cast<instruction::ConditionJump*>((*i_it).get())->str();
            } else  if (check_is<instruction::Call*>((*i_it).get())) {
                stream << dynamic_cast<instruction::Call*>((*i_it).get())->str();
            } else  if (check_is<instruction::Mov*>((*i_it).get())) {
                stream << dynamic_cast<instruction::Mov*>((*i_it).get())->str();
            } else {
                stream << (*i_it)->str();
            }
            stream << std::endl;
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

void InstructionManager::new_vtable(std::string class_name, frontend::Environment::MethodsPtr list_of_methods)
{
    this->virtual_tables[class_name] = list_of_methods;
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
    Block::instr_ptr_t instr;
    if (check_is<Str*>(type)) {
        std::map<std::string, int>::iterator it = this->constant_strings.find("");

        int identifier = this->constant_strings_no;
        if (it == this->constant_strings.end()) {
            this->constant_strings[""] = this->constant_strings_no++;
        } else {
            identifier = it->second;
        }
        //  Maybe copy this one.
        instr = Block::instr_ptr_t(new instruction::Push(arg(CONSTANT_FIELD,
                identifier, 1)));
    } else {
        instr = Block::instr_ptr_t(new instruction::Push(arg(CONSTANT_FIELD, 0)));
    }
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

void InstructionManager::function_call(std::string ident)
{
    Block::instr_ptr_t call(new instruction::Call(ident));
    this->add(call);
}

void InstructionManager::method_call(int position)
{
    // VTABLE IS AT FIRST POSITION IN OBJECT!
    // CALCULATE ON EAX
    Block::instr_ptr_t load_vtable_to_EAX(new instruction::Mov(arg(MEMORY, ESI), arg(REGISTER, EAX)));
    Block::instr_ptr_t add_offset(new instruction::Add(arg(CONSTANT_FIELD, position * 4), arg(REGISTER, EAX)));
    Block::instr_ptr_t dereference(new instruction::Mov(arg(MEMORY, EAX), arg(REGISTER, EAX)));
    Block::instr_ptr_t call(new instruction::Call(arg(REGISTER, EAX)));
    this->add(load_vtable_to_EAX, add_offset, dereference, call);
}

void InstructionManager::add_to_ESP(int variables)
{
//    if ((debug) and (value == 0))
//        std::cerr << "adding 0 to esp has no sense "
//        << __FILE__ << ":" << __LINE__ << std::endl;
    Block::instr_ptr_t add(new instruction::Add(arg(CONSTANT_FIELD, variables * 4), arg(REGISTER, ESP)));
    this->add(add);
}

void InstructionManager::pop_to_EAX()
{
    Block::instr_ptr_t ret_value(new instruction::Pop(arg(REGISTER, EAX)));
    this->add(ret_value);
}


// Arrays will be inited always with zeros (in case of non primitives it means
// that it'll be filled with nulls!)
// Got len on top of a stack. Remove from it.
void InstructionManager::alloc_array()
{

    // Mov stack top to  EAX
    // EAX + 1
    // // EAX * 4
    // push EAX
    // call
    // pop / not used /
    // pop EBX (size)
    // move EBX to addr
    // push EAX // alloc returned address


    // DO NOT FORGET ABOUT ARRAY SIZE
    Block::instr_ptr_t size_to_EAX(new instruction::Mov(arg(MEMORY, ESP), arg(REGISTER, EAX)));
    Block::instr_ptr_t add_arr_size1(new instruction::Inc(arg(REGISTER, EAX)));
    //Block::instr_ptr_t mul_by_word_size(new instruction::Imul(arg(CONSTANT_FIELD, 4), arg(REGISTER, EAX)));
    Block::instr_ptr_t push_argument(new instruction::Push(arg(REGISTER, EAX)));
    Block::instr_ptr_t call_malloc(new instruction::Call(InstructionManager::malloc_name));
    //this->add(size_to_EAX, add_arr_size1, mul_by_word_size, push_argument);
    this->add(size_to_EAX, add_arr_size1, push_argument);
    this->add(call_malloc);
    // Assign size of array
    // Remove this increased size
    Block::instr_ptr_t remove_increased_size_from_stack(new instruction::Add(arg(CONSTANT_FIELD, 4), arg(REGISTER, ESP)));
    Block::instr_ptr_t get_size(new instruction::Pop(arg(REGISTER, EBX)));
    Block::instr_ptr_t add_address(new instruction::Mov(arg(REGISTER, EBX), arg(MEMORY, EAX)));
    Block::instr_ptr_t push_addr(new instruction::Push(arg(REGISTER, EAX)));
    this->add(remove_increased_size_from_stack, get_size, add_address, push_addr);
}

void InstructionManager::alloc_object(std::string class_name, int all_fields)
{
    // all_fields contains pointer to virtual table (this pointer is in FIRST field)
    Block::instr_ptr_t obj_size_on_stack(new instruction::Push(arg(CONSTANT_FIELD, all_fields * 4)));
    Block::instr_ptr_t call_malloc(new instruction::Call(InstructionManager::malloc_name));
    Block::instr_ptr_t remove_size_from_stack(new instruction::Inc(arg(REGISTER, ESP)));
    Block::instr_ptr_t push_address(new instruction::Push(arg(REGISTER, EAX)));
    this->add(obj_size_on_stack, call_malloc, remove_size_from_stack, push_address);

    // assign vtable to object (addres is in EAX)
    Block::instr_ptr_t vtable(new instruction::Mov(class_name, arg(MEMORY, EAX)));
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
    // TODO:
    Block::instr_ptr_t sub(new instruction::Sub(arg(REGISTER, EAX), arg(MEMORY, ESP)));
    this->add(load, sub);
}

void InstructionManager::mul_on_stack()
{
    Block::instr_ptr_t load(new instruction::Pop(arg(REGISTER, EAX)));
    Block::instr_ptr_t move(new instruction::Mov(arg(MEMORY, ESP), arg(REGISTER, ECX)));
    Block::instr_ptr_t mul(new instruction::Imul(arg(REGISTER, EAX), arg(REGISTER, ECX)));
    Block::instr_ptr_t move_result(new instruction::Mov(arg(REGISTER, ECX), arg(MEMORY, ESP)));
    this->add(load, move, mul, move_result);
}

void InstructionManager::div_on_stack()
{
    // a / b
    Block::instr_ptr_t loadb(new instruction::Pop(arg(REGISTER, ECX))); // b
    Block::instr_ptr_t loada(new instruction::Pop(arg(REGISTER, EAX))); // a
    Block::instr_ptr_t cpy(new instruction::Mov(arg(REGISTER, EAX), arg(REGISTER, EDX)));
    Block::instr_ptr_t sign(new instruction::Sar(arg(CONSTANT_FIELD, 31), arg(REGISTER, EDX)));
    Block::instr_ptr_t div(new instruction::Idiv(arg(REGISTER, ECX)));
    Block::instr_ptr_t store_result(new instruction::Push(arg(REGISTER, EAX)));
    this->add(loadb, loada, cpy, sign);
    this->add(div, store_result);
}

void InstructionManager::mod_on_stack()
{
    // a % b
    Block::instr_ptr_t loadb(new instruction::Pop(arg(REGISTER, ECX))); // b
    Block::instr_ptr_t loada(new instruction::Pop(arg(REGISTER, EAX))); // a
    Block::instr_ptr_t cpy(new instruction::Mov(arg(REGISTER, EAX), arg(REGISTER, EDX)));
    Block::instr_ptr_t sign(new instruction::Sar(arg(CONSTANT_FIELD, 31), arg(REGISTER, EDX)));
    Block::instr_ptr_t div(new instruction::Idiv(arg(REGISTER, ECX)));
    Block::instr_ptr_t store_result(new instruction::Push(arg(REGISTER, EDX)));
    this->add(loadb, loada, cpy, sign);
    this->add(div, store_result);
}

int InstructionManager::cstr_add(std::string & str)
{
    std::map<std::string, int>::iterator it = this->constant_strings.find(str);
    int identifier = this->constant_strings_no;
    if (it == this->constant_strings.end()) {
        this->constant_strings[str] = this->constant_strings_no++;
    } else {
        identifier = it->second;
    }
    return identifier;
}

void InstructionManager::add_const_string(std::string & str)
{
    int id = this->cstr_add(str);
    Block::instr_ptr_t store(new instruction::Push(arg(CONSTANT_FIELD, id, 1)));
    this->add(store);
}

void InstructionManager::jump(int id)
{
    Block::instr_ptr_t instr(new instruction::Jump(Block::ident_prefix + int2str(id)));
    this->add(instr);
}

void InstructionManager::pop_add_to_ESI()
{
    Block::instr_ptr_t pop(new instruction::Pop(arg(REGISTER, EAX)));
    Block::instr_ptr_t load_cnst(new instruction::Mov(arg(CONSTANT_FIELD, 4), arg(REGISTER, ECX)));
    Block::instr_ptr_t mul(new instruction::Imul(arg(REGISTER, ECX), arg(REGISTER, EAX)));
    Block::instr_ptr_t add(new instruction::Add(arg(REGISTER, EAX), arg(REGISTER, ESI)));
    this->add(pop, load_cnst, mul, add);
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
    Block::instr_ptr_t instr(new instruction::Add(arg(CONSTANT_FIELD, value), arg(REGISTER, ESI)));
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
    Block::instr_ptr_t pop(new instruction::Pop(arg(REGISTER, EAX)));
    Block::instr_ptr_t instr(new instruction::Cmp(arg(CONSTANT_FIELD, 0), arg(REGISTER, EAX)));
    this->add(pop, instr);
    this->jump_if(InstructionManager::EQU, label_id);
}

void InstructionManager::jump_if_not0(int label_id)
{
    Block::instr_ptr_t pop(new instruction::Pop(arg(REGISTER, EAX)));
    Block::instr_ptr_t instr(new instruction::Cmp(arg(CONSTANT_FIELD, 0), arg(REGISTER, EAX)));
    this->add(pop, instr);
    this->jump_if(InstructionManager::NE, label_id);
}

void InstructionManager::cmp_stack()
{
    // USE EDX and ECX
    //Block::instr_ptr_t decrease_stack(new instruction::Add(arg(CONSTANT_FIELD, 8), arg(REGISTER, ESP)));
    //Block::instr_ptr_t store(new instruction::Mov(arg(MEMORY, ESP, -8), arg(REGISTER, EAX)));
    Block::instr_ptr_t pop1(new instruction::Pop(arg(REGISTER, EDX)));
    Block::instr_ptr_t pop2(new instruction::Pop(arg(REGISTER, ECX)));
    //Block::instr_ptr_t cmp(new instruction::Cmp(arg(REGISTER, EAX), arg(MEMORY, ESP, -4))); // XXX: check it
    Block::instr_ptr_t cmp(new instruction::Cmp(arg(REGISTER, EDX), arg(REGISTER, ECX)));
    this->add(pop1, pop2, cmp);
}

void InstructionManager::concat_str_on_stack()
{
    // call concatenating function
    Block::instr_ptr_t pop1(new instruction::Pop(arg(REGISTER, EDX)));
    Block::instr_ptr_t pop2(new instruction::Pop(arg(REGISTER, ECX)));
    Block::instr_ptr_t push1(new instruction::Push(arg(REGISTER, EDX)));
    Block::instr_ptr_t push2(new instruction::Push(arg(REGISTER, ECX)));
    this->add(pop1, pop2, push1, push2);
    Block::instr_ptr_t call_concatingfunction(new instruction::Call(InstructionManager::add_strings));
    Block::instr_ptr_t remove_args(new instruction::Add(arg(CONSTANT_FIELD, 8), arg(REGISTER, ESP)));
    Block::instr_ptr_t push_result(new instruction::Push(arg(REGISTER, EAX)));
    this->add(call_concatingfunction, remove_args, push_result);

}

void InstructionManager::loop(int label_id)
{
    Block::instr_ptr_t loop(new instruction::Loop(Block::ident_prefix + int2str(label_id)));
    this->add(loop);
}

void InstructionManager::dereference_from_ESI_to_ECX_minus_1()
{
    Block::instr_ptr_t load_array_len(new instruction::Mov(arg(MEMORY, ESI), arg(REGISTER, ECX)));
    //Block::instr_ptr_t deref(new instruction::Mov(arg(MEMORY, ECX), arg(REGISTER, ECX)));
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

void InstructionManager::pop_ESI()
{
    Block::instr_ptr_t instr(new instruction::Pop(arg(REGISTER, ESI)));
    this->add(instr);
}

void InstructionManager::push_EAX()
{
    Block::instr_ptr_t instr(new instruction::Push(arg(REGISTER, EAX)));
    this->add(instr);
}

void InstructionManager::pop_stack_snd_to_ESI()
{
    Block::instr_ptr_t PopFST(new instruction::Pop(arg(REGISTER, EAX)));
    Block::instr_ptr_t PopSND(new instruction::Pop(arg(REGISTER, ESI)));
    Block::instr_ptr_t PushFST(new instruction::Push(arg(REGISTER, EAX)));
    this->add(PopFST, PopSND, PushFST);
}

void InstructionManager::dereference_var_to_var(int var_1_pos, int var_2_pos)
{
    Block::instr_ptr_t mov1(
        new instruction::Mov(
                arg(MEMORY, EBP, -4 * var_1_pos), arg(REGISTER, EAX)));
    //Block::instr_ptr_t deref(new instruction::Mov(arg(MEMORY, EAX), arg(REGISTER, EAX)));
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
