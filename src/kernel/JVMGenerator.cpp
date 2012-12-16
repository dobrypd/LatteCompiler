/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "JVMGenerator.h"
#include "global.h"
#include "Environment.h"
#include "JVMEnvironment.h"

using std::endl;

namespace jvm
{


#define JVM (*(this->jvm))


const char* JVMGenerator::runtime_lib = "Runtime";

JVMGenerator::JVMGenerator(std::string output_file_name,
        frontend::Environment& env)
    : jasmin_file_name(output_file_name),
      jvm(0),
      env(env)
{
    size_t found_dot = this->jasmin_file_name.rfind('.');
    size_t found_slash = this->jasmin_file_name.rfind('/');
    if (found_slash == std::string::npos) found_slash = 0; else found_slash++;
    this->class_name = this->jasmin_file_name.substr(found_slash,
            found_dot - found_slash);
}

void JVMGenerator::generate(Visitable *ast_root)
{
    // Open file.
    std::ofstream file;
    file.open(this->jasmin_file_name.c_str());

    std::stringstream jvm_current_block(std::stringstream::in | std::stringstream::out);
    this->jvm = &jvm_current_block;

    // Generate.
    ast_root->accept(this);

    // Stream to file;
    file << (this->jvm->str());

    // Close file.
    file.close();
}


std::string JVMGenerator::type_to_jvm_type(Type* type, bool is_arg=false)
{
    if (frontend::check_is<Int*>(type)) {
        if (is_arg)
            return "I";
        return "i";
    }
    else if (frontend::check_is<Str*>(type)) {
        if (is_arg)
            return "Ljava/lang/String;";
        return "a";
    }
    else if (frontend::check_is<Bool*>(type)) {
        if (is_arg)
            return "I";  // JVM use integer field to store booleans.
        return "i";
    }
    else if (frontend::check_is<Void*>(type)) {
        if (is_arg)
            return "V";
        return "";
    }
    return "";
}

void JVMGenerator::bool_expr_to_stack(int label_t, int label_f)
{
    int label_end = this->next_label++;
    JVM << "L" << label_t << ":" << endl;
    JVM << "    iconst_1" << endl;
    JVM << "    goto L" << label_end << endl;
    JVM << "L" << label_f << ":" << endl;
    JVM << "    iconst_0" << endl;
    JVM << "L" << label_end << ":" << endl;
}



void JVMGenerator::visitProg(Prog* t) { } //abstract class
void JVMGenerator::visitTopDef(TopDef* t) {} //abstract class
void JVMGenerator::visitArg(Arg* t) {} //abstract class
void JVMGenerator::visitBlk(Blk* t) {} //abstract class
void JVMGenerator::visitStmt(Stmt* t) {} //abstract class
void JVMGenerator::visitItem(Item* t) {} //abstract class
void JVMGenerator::visitType(Type* t) {} //abstract class
void JVMGenerator::visitExpr(Expr* t) {} //abstract class
void JVMGenerator::visitAddOp(AddOp* t) {} //abstract class
void JVMGenerator::visitMulOp(MulOp* t) {} //abstract class
void JVMGenerator::visitRelOp(RelOp* t) {} //abstract class

void JVMGenerator::visitProgram(Program* program)
{
    // Only file name.
    size_t found = this->jasmin_file_name.rfind('/');
    if (found == std::string::npos) found = 0; else found++;
    JVM << ".source " << this->jasmin_file_name.substr(found) << endl;
    JVM << ".class public " << this->class_name << endl;
    JVM << ".super java/lang/Object" << endl;
    JVM << endl << endl;
    /*
     * Standard constructor.
     */
    JVM << "; standard initializer" << endl;
    JVM << ".method public <init>()V" << endl;
    JVM << "    aload_0" << endl;
    JVM << "    invokenonvirtual java/lang/Object/<init>()V" << endl;
    JVM << "    return" << endl;
    JVM << ".end method" << endl;
    /*
     * Default Java main function declaration. (it's ok to have another main)
     * Just invoke main()I in it.
     */
    JVM << ".method public static main([Ljava/lang/String;)V" << endl;
    JVM << ".limit stack 1" << endl;
    JVM << "    invokestatic " << this->class_name << "/main()I" << endl;
    JVM << "    return" << endl;
    JVM << ".end method" << endl;
    JVM << endl << endl;

    this->next_label = 0;
    this->max_stack_size = 0;
    this->e_was_rel = false;
    program->listtopdef_->accept(this);
}

void JVMGenerator::visitFnDef(FnDef* fndef)
{
    std::stringstream* tmp_jvm = this->jvm;

    std::stringstream current_block_jvm(std::stringstream::in | std::stringstream::out);
    std::stringstream arguments_types_jvm(std::stringstream::in | std::stringstream::out);

    this->current_function_locals_size = 0;
    this->current_function_stack_size = 0;

    fndef->type_->accept(this);
    this->last_function_type = fndef->type_;
    visitIdent(fndef->ident_);

    this->jvm_e.prepare();
    this->jvm = &arguments_types_jvm;
    fndef->listarg_->accept(this);
    this->jvm = &current_block_jvm;
    fndef->blk_->accept(this);
    this->jvm_e.back();

    this->jvm = tmp_jvm;
    JVM << ".method static public " << fndef->ident_ << "(";
    JVM << arguments_types_jvm.str();
    JVM << ")" << this->type_to_jvm_type(fndef->type_, true) << endl;
    JVM << ".limit locals " << this->current_function_locals_size << endl;
    JVM << ".limit stack " << this->max_stack_size << endl;
    JVM << current_block_jvm.str();
    //If last character is not ':' do not add 'nop'.
    JVM << "    nop" << endl;
    JVM << ".end method"<< endl;
    JVM << endl << endl;
}

void JVMGenerator::visitArgument(Argument* argument)
{
    argument->type_->accept(this);
    JVM << this->type_to_jvm_type(argument->type_, true);
    this->jvm_e.add_variable(argument->type_, argument->ident_,
            this->current_function_locals_size++);
    visitIdent(argument->ident_);
}

void JVMGenerator::visitStmBlock(StmBlock* stmblock)
{
    stmblock->liststmt_->accept(this);
}

void JVMGenerator::visitStmEmpty(StmEmpty* stmempty)
{
}

void JVMGenerator::visitStmBStmt(StmBStmt* stmbstmt)
{
    this->jvm_e.prepare();
    stmbstmt->blk_->accept(this);
    this->jvm_e.back();
}

void JVMGenerator::visitStmDecl(StmDecl* stmdecl)
{
    stmdecl->type_->accept(this);
    this->last_declaration_type = stmdecl->type_;
    stmdecl->listitem_->accept(this);
}

void JVMGenerator::visitStmAss(StmAss* stmass)
{
    visitIdent(stmass->ident_);
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    stmass->expr_->accept(this);
    if (this->e_was_rel)
        this->bool_expr_to_stack(label_t, label_f);
    assert(this->current_function_stack_size > 0);
    JVMEnvironment::VarInfoPtr var_info = this->jvm_e.get_variable(stmass->ident_);

    JVM << "    " << this->type_to_jvm_type(var_info->type, false)
            << "store "
            << var_info->local_v << endl;
}

void JVMGenerator::visitStmIncr(StmIncr* stmincr)
{
    JVMEnvironment::VarInfoPtr var_info = this->jvm_e.get_variable(
            stmincr->ident_);

    visitIdent(stmincr->ident_);

    JVM << "    iinc " << var_info->local_v << " 1" << endl;
}

void JVMGenerator::visitStmDecr(StmDecr* stmdecr)
{
    JVMEnvironment::VarInfoPtr var_info = this->jvm_e.get_variable(
            stmdecr->ident_);

    visitIdent(stmdecr->ident_);

    JVM << "    iinc " << var_info->local_v << " -1" << endl;
}

void JVMGenerator::visitStmRet(StmRet* stmret)
{
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    stmret->expr_->accept(this);
    if (this->e_was_rel)
        this->bool_expr_to_stack(label_t, label_f);
    assert(this->current_function_stack_size > 0);
    JVM << "    " << this->type_to_jvm_type(this->last_function_type, false)
            << "return" << endl;
}

void JVMGenerator::visitStmVRet(StmVRet* stmvret)
{
    JVM << "    return" << endl;
}

void JVMGenerator::visitStmCond(StmCond* stmcond)
{
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;

    stmcond->expr_->accept(this);
    JVM << "L" << label_t << ":" << endl;
    stmcond->stmt_->accept(this);
    JVM << "L" << label_f << ":" << endl;
}

void JVMGenerator::visitStmCondElse(StmCondElse* stmcondelse)
{
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    int label_end = this->next_label++;

    this->last_false_label = label_f;
    this->last_true_label = label_t;
    stmcondelse->expr_->accept(this);
    JVM << "L" << label_t << ":" << endl;
    stmcondelse->stmt_1->accept(this);
    JVM << "    goto " << "L" << label_end << endl;
    JVM << "L" << label_f << ":" << endl;
    stmcondelse->stmt_2->accept(this);
    JVM << "L" << label_end << ":" << endl;
}

void JVMGenerator::visitStmWhile(StmWhile* stmwhile)
{
    int before_lbl = this->next_label++;
    int after_lbl = this->next_label++;
    int block_start = this->next_label++;

    JVM << "L" << before_lbl << ":" << endl;
    this->last_false_label = after_lbl;
    this->last_true_label = block_start;
    stmwhile->expr_->accept(this);
    JVM << "L" << block_start << ":" << endl;
    stmwhile->stmt_->accept(this);
    JVM << "    goto " << "L" << before_lbl << endl;
    JVM << "L" << after_lbl << ":" << endl;
}

void JVMGenerator::visitStmSExp(StmSExp* stmsexp)
{
    stmsexp->expr_->accept(this);
    if ((!this->e_was_rel) && (!frontend::check_is<Void*>(this->last_type)))
        JVM << "    pop" << endl;
}

void JVMGenerator::visitStmNoInit(StmNoInit* stmnoinit)
{
    visitIdent(stmnoinit->ident_);
    int var_id = this->current_function_locals_size++;
    this->current_function_stack_size =
            std::max(this->current_function_stack_size, 1);
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);

    this->jvm_e.add_variable(this->last_declaration_type, stmnoinit->ident_,
            var_id);
    /*
     * Declaration with default values.
     */
    if (frontend::check_is<Int*>(this->last_declaration_type)) {
        JVM << "    iconst_0" << endl;
        JVM << "    istore " << var_id << endl;
    }
    else if (frontend::check_is<Str*>(this->last_declaration_type)) {
        JVM << "    ldc \"\"" << endl;
        JVM << "    astore " << var_id << endl;
    }
    else if (frontend::check_is<Bool*>(this->last_declaration_type)) {
        JVM << "    iconst_0" << endl;
        JVM << "    istore " << var_id << endl;
    }
}

void JVMGenerator::visitStmInit(StmInit* stminit)
{
    visitIdent(stminit->ident_);
    int var_id = this->current_function_locals_size++;

    this->jvm_e.add_variable(this->last_declaration_type, stminit->ident_,
                var_id);
    if (frontend::check_is<Int*>(this->last_declaration_type)) {
        stminit->expr_->accept(this);
        assert(this->current_function_stack_size > 0);
        JVM << "    istore " << var_id << endl;
    } else if (frontend::check_is<Str*>(this->last_declaration_type)) {
        stminit->expr_->accept(this);
        assert(this->current_function_stack_size > 0);
        JVM << "    astore " << var_id << endl;
    } else if (frontend::check_is<Bool*>(this->last_declaration_type)) {
        int label_t = this->next_label++;
        int label_f = this->next_label++;
        this->last_true_label = label_t;
        this->last_false_label = label_f;
        stminit->expr_->accept(this);
        if (this->e_was_rel)
            this->bool_expr_to_stack(label_t, label_f);
        JVM << "    istore " << var_id << endl;
    }

    this->current_function_stack_size--;
}

void JVMGenerator::visitInt(Int* integer)
{
}

void JVMGenerator::visitStr(Str* str)
{
}

void JVMGenerator::visitBool(Bool* boolean)
{
}

void JVMGenerator::visitVoid(Void* void_field)
{
}

void JVMGenerator::visitFun(Fun* fun)
{
    fun->type_->accept(this);
    fun->listtype_->accept(this);
}

void JVMGenerator::visitEVar(EVar* evar)
{
    visitIdent(evar->ident_);

    this->current_function_stack_size += 1;
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
    JVMEnvironment::VarInfoPtr var_info = this->jvm_e.get_variable(
                evar->ident_);
    JVM << "    " << this->type_to_jvm_type(var_info->type, false)
            << "load " << var_info->local_v << endl;
    this->last_type = var_info->type;

    if (frontend::check_is<Bool*>(var_info->type)) {
        JVM << "    ifne L" << this->last_true_label << endl;
        JVM << "    goto L" << this->last_false_label << endl;
        this->e_was_rel = true;
    } else {
        this->e_was_rel = false;
    }
}

void JVMGenerator::visitELitInt(ELitInt* elitint)
{
    visitInteger(elitint->integer_);
    if (this->pop_if_zero && (elitint->integer_ == 0)) {
        this->last_is_zero = true;
    } else {
        this->current_function_stack_size += 1;
        this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
        JVM << "    ldc " << elitint->integer_ << endl;
    }
    this->last_type = &(this->literal_int);
    this->e_was_rel = false;
}

void JVMGenerator::visitELitTrue(ELitTrue* elittrue)
{
    this->current_function_stack_size += 1;
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
    JVM << "    iconst_1" << endl;
    this->last_type = &(this->literal_bool);
    this->e_was_rel = false;
}

void JVMGenerator::visitELitFalse(ELitFalse* elitfalse)
{
    this->current_function_stack_size += 1;
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
    JVM << "    iconst_0" << endl;
    this->last_type = &(this->literal_bool);
    this->e_was_rel = false;
}

void JVMGenerator::visitEApp(EApp* eapp)
{
    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);

    this->current_function_stack_size =
            std::max(this->current_function_stack_size, 1);
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);

    frontend::Environment::FunInfoPtr fun =
            this->env.get_function(eapp->ident_);

    if (fun->is_extern) {
        JVM << "    invokestatic " << JVMGenerator::runtime_lib
                << "/" << eapp->ident_ << "(";
    } else {
        JVM << "    invokestatic " << this->class_name
                << "/" << eapp->ident_ << "(";
    }

    // Collect arguments
    for (std::vector<frontend::Environment::VarInfoPtr>::iterator it =
            fun->arguments.begin(); it != fun->arguments.end(); it++){
        JVM << this->type_to_jvm_type((*it)->type, true);
        this->current_function_stack_size -= 1;
    }

    JVM << ")" << this->type_to_jvm_type(fun->ret_type, true) << endl;
    if (!frontend::check_is<Void *>(fun->ret_type))
        this->current_function_stack_size += 1;
    this->last_type = fun->ret_type;
    this->e_was_rel = false;
}

void JVMGenerator::visitEString(EString* estring)
{
    visitString(estring->string_);
    JVM << "    ldc \"" << estring->string_ << "\"" << endl;
    this->current_function_stack_size += 1;
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
    this->last_type = &(this->literal_string);
    this->e_was_rel = false;
}

void JVMGenerator::visitNeg(Neg* neg)
{
    neg->expr_->accept(this);
    assert(this->current_function_stack_size > 0);
    JVM << "    ineg" << endl;
}

void JVMGenerator::visitNot(Not* not_field)
{
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    not_field->expr_->accept(this);
    if (this->e_was_rel)
        this->bool_expr_to_stack(label_t, label_f);
    int labelT = this->next_label++;
    int labelF = this->next_label++;
    JVM << "    ifne L" << labelF << endl;
    JVM << "    iconst_1" << endl;
    JVM << "    goto L" << labelT << endl;
    JVM << "L" << labelF << ":" << endl;
    JVM << "    iconst_0" << endl;
    JVM << "L" << labelT << ":" << endl;
    this->e_was_rel = false;
}

void JVMGenerator::visitEMul(EMul* emul)
{
    emul->expr_1->accept(this);
    assert(this->current_function_stack_size > 0);
    emul->expr_2->accept(this);
    assert(this->current_function_stack_size > 1);

    emul->mulop_->accept(this);
    this->current_function_stack_size -= 1;
}

void JVMGenerator::visitEAdd(EAdd* eadd)
{
    std::stringstream* tmp_jvm = this->jvm;
    std::stringstream current_block_jvm_e1(std::stringstream::in | std::stringstream::out);
    std::stringstream current_block_jvm_e2(std::stringstream::in | std::stringstream::out);

    this->jvm = &current_block_jvm_e1;
    eadd->expr_1->accept(this);
    assert(this->current_function_stack_size > 0);
    this->jvm = &current_block_jvm_e2;
    eadd->expr_2->accept(this);
    assert(this->current_function_stack_size > 1);

    this->jvm = tmp_jvm;

    if (frontend::check_is<Str *>(this->last_type)) {
        // Add two strings.
        JVM << "    new java/lang/StringBuilder" << endl;
        JVM << "    dup" << endl;
        JVM << "    invokespecial java/lang/StringBuilder/<init>()V" << endl;
        JVM << current_block_jvm_e1.str();
        JVM << "    invokevirtual java/lang/StringBuilder/append(Ljava/lang/String;)Ljava/lang/StringBuilder;" << endl;
        JVM << current_block_jvm_e2.str();
        JVM << "    invokevirtual java/lang/StringBuilder/append(Ljava/lang/String;)Ljava/lang/StringBuilder;" << endl;
        JVM << "    invokevirtual java/lang/StringBuilder/toString()Ljava/lang/String;" << endl;
    }
    else
    {
        JVM << current_block_jvm_e1.str();
        JVM << current_block_jvm_e2.str();
        eadd->addop_->accept(this);
        this->current_function_stack_size -= 1;
    }
}

void JVMGenerator::visitERel(ERel* erel)
{
    int this_last_f_label = this->last_false_label;
    int this_last_t_label = this->last_true_label;

    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    erel->expr_1->accept(this);
    if (this->e_was_rel)
        this->bool_expr_to_stack(label_t, label_f);
    this->last_is_zero = false;
    this->pop_if_zero = true;
    label_t = this->next_label++;
    label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    erel->expr_2->accept(this);

    if (this->e_was_rel)
        this->bool_expr_to_stack(label_t, label_f);

    if (this->last_is_zero) {
        JVM << "    if";
    } else {
        if (frontend::check_is<Str*>(this->last_type)){
            JVM << "    if_acmp";
        } else {
            JVM << "    if_icmp";
        }
    }
    this->pop_if_zero = false;
    this->last_is_zero = false;

    erel->relop_->accept(this);
    JVM << " L" << this_last_t_label << endl;
    JVM << "    goto L" << this_last_f_label << endl;

    this->current_function_stack_size -= 1; // maybe two
    this->last_type = &(this->literal_bool);
    this->e_was_rel = true;
    this->last_false_label = this_last_f_label;
    this->last_true_label = this_last_t_label;
}

void JVMGenerator::visitEAnd(EAnd* eand)
{
    int this_true_label = this->last_true_label;
    int this_false_label = this->last_false_label;

    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    eand->expr_1->accept(this);
    if (this->e_was_rel)
        this->bool_expr_to_stack(label_t, label_f);
    JVM << "    ifeq L" << this_false_label << endl;

    label_t = this->next_label++;
    label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    eand->expr_2->accept(this);
    if (this->e_was_rel)
        this->bool_expr_to_stack(label_t, label_f);
    JVM << "    ifeq L" << this_false_label << endl;
    JVM << "    goto L" << this_true_label << endl;
    this->current_function_stack_size -= 1;

    this->last_true_label = this_true_label;
    this->last_false_label = this_false_label;
    this->e_was_rel = true;
}

void JVMGenerator::visitEOr(EOr* eor)
{
    int this_true_label = this->last_true_label;
    int this_false_label = this->last_false_label;

    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    eor->expr_1->accept(this);
    if (this->e_was_rel)
        this->bool_expr_to_stack(label_t, label_f);
    JVM << "    ifne L" << this_true_label << endl;


    label_t = this->next_label++;
    label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    eor->expr_2->accept(this);
    if (this->e_was_rel)
        this->bool_expr_to_stack(label_t, label_f);
    JVM << "    ifeq L" << this_false_label << endl;
    JVM << "    goto L" << this_true_label << endl;

    this->last_true_label = this_true_label;
    this->last_false_label = this_false_label;
    this->e_was_rel = true;
}

void JVMGenerator::visitPlus(Plus* plus)
{
    //if (!frontend::check_is<Str *>(this->last_type)){
        JVM << "    iadd" << endl;
    //}
}

void JVMGenerator::visitMinus(Minus* minus)
{
    JVM << "    isub" << endl;
}

void JVMGenerator::visitTimes(Times* times)
{
    JVM << "    imul" << endl;
}

void JVMGenerator::visitDiv(Div* div)
{
    JVM << "    idiv" << endl;
}

void JVMGenerator::visitMod(Mod* mod)
{
    JVM << "    irem" << endl;
}

void JVMGenerator::visitLTH(LTH* lth)
{
    JVM << "lt";
}

void JVMGenerator::visitLE(LE* le)
{
    JVM << "le";
}

void JVMGenerator::visitGTH(GTH* gth)
{
    JVM << "gt";
}

void JVMGenerator::visitGE(GE* ge)
{
    JVM << "ge";
}

void JVMGenerator::visitEQU(EQU* equ)
{
    JVM << "eq";
}

void JVMGenerator::visitNE(NE* ne)
{
    JVM << "ne";
}


void JVMGenerator::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void JVMGenerator::visitListArg(ListArg* listarg)
{
    // Add arguments to jvm by types.
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void JVMGenerator::visitListStmt(ListStmt* liststmt)
{
    for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void JVMGenerator::visitListItem(ListItem* listitem)
{
    for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void JVMGenerator::visitListType(ListType* listtype)
{
    for (ListType::iterator i = listtype->begin() ; i != listtype->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void JVMGenerator::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        int label_t = this->next_label++;
        int label_f = this->next_label++;
        this->last_true_label = label_t;
        this->last_false_label = label_f;
        (*i)->accept(this);
        if (this->e_was_rel)
            this->bool_expr_to_stack(label_t, label_f);
    }
}


void JVMGenerator::visitInteger(Integer x)
{
}

void JVMGenerator::visitChar(Char x)
{
}

void JVMGenerator::visitDouble(Double x)
{
}

void JVMGenerator::visitString(String x)
{
}

void JVMGenerator::visitIdent(Ident x)
{
}

} /* namespace jvm */
