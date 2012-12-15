/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "JVMGenerator.h"
#include "global.h"
#include "Environment.h"

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
{
    if (frontend::check_is<Int*>(type)) {
        if (is_arg)
            return "I";
        return "i";
    }
    else if (frontend::check_is<Str*>(type)) {
        if (is_arg)
            return "Ljava/lang/String;";
        return ""; // should not happend
    }
    else if (frontend::check_is<Bool*>(type)) {
        if (is_arg)
            return "B";
        return "b";
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
     * Default Java main function declaration.
     * Just invoke main()I in it.
     */
    JVM << ".method public static main([Ljava/lang/String;)V" << endl;
    JVM << ".limit stack 1" << endl;
    JVM << "    invokestatic " << this->class_name << "/main()I" << endl;
    JVM << "    return" << endl;
    JVM << ".end method" << endl;
    JVM << endl << endl;

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
    visitIdent(fndef->ident_);

    this->jvm = &arguments_types_jvm;
    fndef->listarg_->accept(this);

    this->jvm = &current_block_jvm;
    fndef->blk_->accept(this);

    this->jvm = tmp_jvm;
    JVM << ".method static public " << fndef->ident_ << "(";
    JVM << arguments_types_jvm.str();
    JVM << ")" << this->type_to_jvm_type(fndef->type_, true) << endl;
    JVM << ".limit locals " << this->current_function_locals_size << endl;
    JVM << ".limit stack " << this->current_function_stack_size << endl;
    JVM << current_block_jvm.str();
    JVM << ".end method"<< endl;
    JVM << endl << endl;

}

void JVMGenerator::visitArgument(Argument* argument)
{
    argument->type_->accept(this);
    JVM << this->type_to_jvm_type(argument->type_, true);
    this->current_function_locals_size += 1;
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
    stmbstmt->blk_->accept(this);
}

void JVMGenerator::visitStmDecl(StmDecl* stmdecl)
{
    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);
}

void JVMGenerator::visitStmAss(StmAss* stmass)
{
    visitIdent(stmass->ident_);
    stmass->expr_->accept(this);
    //JVM << "    " << this->type_to_jvm_type(this->env->variable_type(
    //        stmass->ident_))
    //        << "store "
    //        << this->env->variable_no(stmass->ident_) << endl;
}

void JVMGenerator::visitStmIncr(StmIncr* stmincr)
{
    visitIdent(stmincr->ident_);
}

void JVMGenerator::visitStmDecr(StmDecr* stmdecr)
{
    visitIdent(stmdecr->ident_);
}

void JVMGenerator::visitStmRet(StmRet* stmret)
{
    stmret->expr_->accept(this);
}

void JVMGenerator::visitStmVRet(StmVRet* stmvret)
{
}

void JVMGenerator::visitStmCond(StmCond* stmcond)
{
    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);
}

void JVMGenerator::visitStmCondElse(StmCondElse* stmcondelse)
{
    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);
}

void JVMGenerator::visitStmWhile(StmWhile* stmwhile)
{
    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);
}

void JVMGenerator::visitStmSExp(StmSExp* stmsexp)
{
    stmsexp->expr_->accept(this);
}

void JVMGenerator::visitStmNoInit(StmNoInit* stmnoinit)
{
    visitIdent(stmnoinit->ident_);
}

void JVMGenerator::visitStmInit(StmInit* stminit)
{
    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);
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
}

void JVMGenerator::visitELitInt(ELitInt* elitint)
{
    visitInteger(elitint->integer_);
}

void JVMGenerator::visitELitTrue(ELitTrue* elittrue)
{
}

void JVMGenerator::visitELitFalse(ELitFalse* elitfalse)
{
}

void JVMGenerator::visitEApp(EApp* eapp)
{
    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);
}

void JVMGenerator::visitEString(EString* estring)
{
    visitString(estring->string_);
}

void JVMGenerator::visitNeg(Neg* neg)
{
    neg->expr_->accept(this);
}

void JVMGenerator::visitNot(Not* not_field)
{
    not_field->expr_->accept(this);
}

void JVMGenerator::visitEMul(EMul* emul)
{
    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);
}

void JVMGenerator::visitEAdd(EAdd* eadd)
{
    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);
}

void JVMGenerator::visitERel(ERel* erel)
{
    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);

}

void JVMGenerator::visitEAnd(EAnd* eand)
{
    eand->expr_1->accept(this);
    eand->expr_2->accept(this);
}

void JVMGenerator::visitEOr(EOr* eor)
{
    eor->expr_1->accept(this);
    eor->expr_2->accept(this);
}

void JVMGenerator::visitPlus(Plus* plus)
{
}

void JVMGenerator::visitMinus(Minus* minus)
{
}

void JVMGenerator::visitTimes(Times* times)
{
}

void JVMGenerator::visitDiv(Div* div)
{
}

void JVMGenerator::visitMod(Mod* mod)
{
}

void JVMGenerator::visitLTH(LTH* lth)
{
}

void JVMGenerator::visitLE(LE* le)
{
}

void JVMGenerator::visitGTH(GTH* gth)
{
}

void JVMGenerator::visitGE(GE* ge)
{
}

void JVMGenerator::visitEQU(EQU* equ)
{
}

void JVMGenerator::visitNE(NE* ne)
{
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
