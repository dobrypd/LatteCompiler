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
{ // TODO: check it
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
        return "v";
    }
    return "";
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
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
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
    //TODO: if last character is ':' add 'nop'
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
    // TODO: free variables.
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
    stmass->expr_->accept(this);
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
    stmret->expr_->accept(this);
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
    int label = this->next_label++;
    stmcond->expr_->accept(this);
    assert(this->current_function_stack_size > 0);
    JVM << "    if_icmpge?? " << label << endl; // TODO:!

    stmcond->stmt_->accept(this);
    JVM << "L" << label << ":" << endl;
}

void JVMGenerator::visitStmCondElse(StmCondElse* stmcondelse)
{
    int label1 = this->next_label++, label2 = this->next_label++;
    stmcondelse->expr_->accept(this);
    assert(this->current_function_stack_size > 0);
    JVM << "    if_icmpge?? " << label1 << endl; // TODO:

    stmcondelse->stmt_1->accept(this);
    JVM << "    goto " << "L" << label2 << endl;

    JVM << "L" << label1 << ":" << endl;
    stmcondelse->stmt_2->accept(this);
    JVM << "L" << label2 << ":" << endl;
}

void JVMGenerator::visitStmWhile(StmWhile* stmwhile)
{
    int before_lbl = this->next_label++;
    int after_lbl = this->next_label++;

    JVM << "L" << before_lbl << ":" << endl;
    stmwhile->expr_->accept(this);
    assert(this->current_function_stack_size > 0);
    JVM << "    if_icmpge?? " << after_lbl << endl; // TODO:

    stmwhile->stmt_->accept(this);
    JVM << "    goto " << "L" << before_lbl << endl;
    JVM << "L" << after_lbl << ":" << endl; // TODO: look if next is .end method!
}

void JVMGenerator::visitStmSExp(StmSExp* stmsexp)
{
    stmsexp->expr_->accept(this);
    //JVM << "    pop" << endl;
    // TODO: check it! not always because it could be void
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
    stminit->expr_->accept(this);
    assert(this->current_function_stack_size > 0);
    // Should has initial value on stack.

    this->jvm_e.add_variable(this->last_declaration_type, stminit->ident_,
                var_id);
    if (frontend::check_is<Int*>(this->last_declaration_type))
        JVM << "    istore " << var_id << endl;
    else if (frontend::check_is<Str*>(this->last_declaration_type))
        JVM << "    astore " << var_id << endl;
    else if (frontend::check_is<Bool*>(this->last_declaration_type))
        JVM << "    istore " << var_id << endl;

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
}

void JVMGenerator::visitELitInt(ELitInt* elitint)
{
    visitInteger(elitint->integer_);
    this->current_function_stack_size += 1;
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
    JVM << "    ldc " << elitint->integer_ << endl;
}

void JVMGenerator::visitELitTrue(ELitTrue* elittrue)
{
    this->current_function_stack_size += 1;
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
    JVM << "    iconst_1" << endl;
}

void JVMGenerator::visitELitFalse(ELitFalse* elitfalse)
{
    this->current_function_stack_size += 1;
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
    JVM << "    iconst_0" << endl;
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

    JVM << "    invokestatic " << this->class_name << "/" << eapp->ident_ // TODO: or runtime
            << "(";

    // Collect arguments
    for (std::vector<frontend::Environment::VarInfoPtr>::iterator it =
            fun->arguments.begin(); it != fun->arguments.end(); it++){
        JVM << this->type_to_jvm_type((*it)->type, true);
        this->current_function_stack_size -= 1;
    }

    JVM << ")" << this->type_to_jvm_type(fun->ret_type, true) << endl;
    if (!frontend::check_is<Void *>(fun->ret_type))
        this->current_function_stack_size += 1;
}

void JVMGenerator::visitEString(EString* estring)
{
    visitString(estring->string_);
    JVM << "    ldc \"" << estring->string_ << "\"" << endl;
    this->current_function_stack_size += 1;
    this->max_stack_size =
            std::max(this->current_function_stack_size, this->max_stack_size);
}

void JVMGenerator::visitNeg(Neg* neg)
{
    neg->expr_->accept(this);
    assert(this->current_function_stack_size > 0);
    JVM << "    ineg" << endl;
}

void JVMGenerator::visitNot(Not* not_field)
{
    not_field->expr_->accept(this);
    int labelT = this->next_label++;
    int labelF = this->next_label++;
    JVM << "    ifne L" << labelF << endl;
    JVM << "    iconst_1" << endl;
    JVM << "    goto L" << labelT << endl;
    JVM << "L" << labelF << ":" << endl;
    JVM << "    iconst_0" << endl;
    JVM << "L" << labelT << ":" << endl;
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
    // save string stream, and check type
    eadd->expr_1->accept(this);
    assert(this->current_function_stack_size > 0);
    eadd->expr_2->accept(this);
    assert(this->current_function_stack_size > 1);

    /*if (frontend::check_is<Str *>(this->last_type)) {
        // Add two strings.
        JVM << "    new     #4; //class java/lang/StringBuilder" << endl;
        JVM << "    dup" << endl;
        JVM << "    invokespecial   #5; //Method java/lang/StringBuilder.\"<init>\":()V" << endl;
        JVM << "    aload_0" << endl;
        JVM << "    invokevirtual   #6; //Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;" << endl;
        JVM << "    aload_1" << endl;
        JVM << "    invokevirtual   #6; //Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;" << endl;
        JVM << "    invokevirtual   #7; //Method java/lang/StringBuilder.toString:()Ljava/lang/String;" << endl;
    }*/

    eadd->addop_->accept(this);
    this->current_function_stack_size -= 1;
}

void JVMGenerator::visitERel(ERel* erel)
{
    erel->expr_1->accept(this);
    erel->expr_2->accept(this);

    erel->relop_->accept(this);
    // TODO: check if in condition.
    this->current_function_stack_size -= 1;
}

void JVMGenerator::visitEAnd(EAnd* eand)
{
    int labelT = this->next_label++;
    int labelF = this->next_label++;

    eand->expr_1->accept(this);
    JVM << "    ifeq L" << labelF << endl;
    eand->expr_2->accept(this);
    JVM << "    ifeq L" << labelF << endl;
    JVM << "    iconst_1" << endl;
    JVM << "    goto L" << labelT << endl;
    JVM << "  L" << labelF << ":" << endl;
    JVM << "    iconst_0" << endl;
    JVM << "  L" << labelT << ":" << endl;
    this->current_function_stack_size -= 1;
}

void JVMGenerator::visitEOr(EOr* eor)
{
    int labelE = this->next_label++;
    int labelTT = this->next_label++;
    int labelF = this->next_label++;

    eor->expr_1->accept(this);
    JVM << "    ifne L" << labelTT << endl;
    eor->expr_2->accept(this);
    JVM << "    ifeq L" << labelF << endl;
    JVM << "  L " << labelTT << endl;
    JVM << "    iconst_1" << endl;
    JVM << "    goto L " << labelE << endl;
    JVM << "  L" << labelF << ":" << endl;
    JVM << "    iconst_0" << endl;
    JVM << "  L" << labelE << ":" << endl;
    this->current_function_stack_size -= 1;
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
    // TODO:
}

void JVMGenerator::visitLE(LE* le)
{
    // TODO:
}

void JVMGenerator::visitGTH(GTH* gth)
{
    // TODO:
}

void JVMGenerator::visitGE(GE* ge)
{
    // TODO:
}

void JVMGenerator::visitEQU(EQU* equ)
{
    // TODO:
}

void JVMGenerator::visitNE(NE* ne)
{
    // TODO:
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
        (*i)->accept(this);
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
