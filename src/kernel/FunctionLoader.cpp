/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include "FunctionLoader.h"
#include "global.h"

namespace frontend
{

FunctionLoader::FunctionLoader(ErrorHandler& error_handler, Environment& env)
    : error_handler(error_handler), env(env)
{
}


void FunctionLoader::check(Visitable* v)
{
    v->accept(this);
}

void FunctionLoader::visitProg(Prog* t) { } //abstract class
void FunctionLoader::visitTopDef(TopDef* t) {} //abstract class
void FunctionLoader::visitArg(Arg* t) {} //abstract class
void FunctionLoader::visitBlk(Blk* t) {} //abstract class
void FunctionLoader::visitStmt(Stmt* t) {} //abstract class
void FunctionLoader::visitItem(Item* t) {} //abstract class
void FunctionLoader::visitType(Type* t) {} //abstract class
void FunctionLoader::visitExpr(Expr* t) {} //abstract class
void FunctionLoader::visitAddOp(AddOp* t) {} //abstract class
void FunctionLoader::visitMulOp(MulOp* t) {} //abstract class
void FunctionLoader::visitRelOp(RelOp* t) {} //abstract class

void FunctionLoader::visitProgram(Program* program)
{
    program->listtopdef_->accept(this);
}

void FunctionLoader::visitFnDef(FnDef* fndef)
{
    if (debug) {
        std::cout << fndef->line_number << " found function `"
                << fndef->ident_ << "` definition." << std::endl;
    }
    if (!this->env.lookup_function(fndef))
        this->env.add_function(fndef);
    else
    {
        std::string comunicate;
        comunicate += "function `";
        comunicate += fndef->ident_;
        comunicate += "` already declared.";
        this->error_handler.error(fndef->type_->line_number, comunicate);
    }
}

void FunctionLoader::visitArgument(Argument* argument)
{
    argument->type_->accept(this);
    visitIdent(argument->ident_);

}

void FunctionLoader::visitStmBlock(StmBlock* stmblock)
{
    stmblock->liststmt_->accept(this);
}

void FunctionLoader::visitStmEmpty(StmEmpty* stmempty)
{
}

void FunctionLoader::visitStmBStmt(StmBStmt* stmbstmt)
{
    stmbstmt->blk_->accept(this);
}

void FunctionLoader::visitStmDecl(StmDecl* stmdecl)
{
    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);
}

void FunctionLoader::visitStmAss(StmAss* stmass)
{
    visitIdent(stmass->ident_);
    stmass->expr_->accept(this);
}

void FunctionLoader::visitStmIncr(StmIncr* stmincr)
{
    visitIdent(stmincr->ident_);
}

void FunctionLoader::visitStmDecr(StmDecr* stmdecr)
{
visitIdent(stmdecr->ident_);
}

void FunctionLoader::visitStmRet(StmRet* stmret)
{
    stmret->expr_->accept(this);
}

void FunctionLoader::visitStmVRet(StmVRet* stmvret)
{
}

void FunctionLoader::visitStmCond(StmCond* stmcond)
{
    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);
}

void FunctionLoader::visitStmCondElse(StmCondElse* stmcondelse)
{
    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);
}

void FunctionLoader::visitStmWhile(StmWhile* stmwhile)
{
    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);
}

void FunctionLoader::visitStmSExp(StmSExp* stmsexp)
{
    stmsexp->expr_->accept(this);
}

void FunctionLoader::visitStmNoInit(StmNoInit* stmnoinit)
{
    visitIdent(stmnoinit->ident_);
}

void FunctionLoader::visitStmInit(StmInit* stminit)
{
    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);
}

void FunctionLoader::visitInt(Int* integer)
{
}

void FunctionLoader::visitStr(Str* str)
{
}

void FunctionLoader::visitBool(Bool* boolean)
{
}

void FunctionLoader::visitVoid(Void* void_field)
{
}

void FunctionLoader::visitFun(Fun* fun)
{
    fun->type_->accept(this);
    fun->listtype_->accept(this);
}

void FunctionLoader::visitEVar(EVar* evar)
{
    visitIdent(evar->ident_);
}

void FunctionLoader::visitELitInt(ELitInt* elitint)
{
    visitInteger(elitint->integer_);
}

void FunctionLoader::visitELitTrue(ELitTrue* elittrue)
{
}

void FunctionLoader::visitELitFalse(ELitFalse* elitfalse)
{
}

void FunctionLoader::visitEApp(EApp* eapp)
{
    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);
}

void FunctionLoader::visitEString(EString* estring)
{
    visitString(estring->string_);
}

void FunctionLoader::visitNeg(Neg* neg)
{
    neg->expr_->accept(this);
}

void FunctionLoader::visitNot(Not* not_field)
{
    not_field->expr_->accept(this);
}

void FunctionLoader::visitEMul(EMul* emul)
{
    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);
}

void FunctionLoader::visitEAdd(EAdd* eadd)
{
    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);
}

void FunctionLoader::visitERel(ERel* erel)
{
    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);
}

void FunctionLoader::visitEAnd(EAnd* eand)
{
    eand->expr_1->accept(this);
    eand->expr_2->accept(this);
}

void FunctionLoader::visitEOr(EOr* eor)
{
    eor->expr_1->accept(this);
    eor->expr_2->accept(this);
}

void FunctionLoader::visitPlus(Plus* plus)
{
}

void FunctionLoader::visitMinus(Minus* minus)
{
}

void FunctionLoader::visitTimes(Times* times)
{
}

void FunctionLoader::visitDiv(Div* div)
{
}

void FunctionLoader::visitMod(Mod* mod)
{
}

void FunctionLoader::visitLTH(LTH* lth)
{
}

void FunctionLoader::visitLE(LE* le)
{
}

void FunctionLoader::visitGTH(GTH* gth)
{
}

void FunctionLoader::visitGE(GE* ge)
{
}

void FunctionLoader::visitEQU(EQU* equ)
{
}

void FunctionLoader::visitNE(NE* ne)
{
}


void FunctionLoader::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListArg(ListArg* listarg)
{
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListStmt(ListStmt* liststmt)
{
    for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListItem(ListItem* listitem)
{
    for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListType(ListType* listtype)
{
    for (ListType::iterator i = listtype->begin() ; i != listtype->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void FunctionLoader::visitInteger(Integer x)
{
}

void FunctionLoader::visitChar(Char x)
{
}

void FunctionLoader::visitDouble(Double x)
{
}

void FunctionLoader::visitString(String x)
{
}

void FunctionLoader::visitIdent(Ident x)
{
}

} /* namespace frontend */
