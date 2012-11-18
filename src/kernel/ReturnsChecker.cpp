/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include "ReturnsChecker.h"
#include "ErrorHandler.h"
#include "global.h"

namespace frontend
{

ReturnsChecker::ReturnsChecker(ErrorHandler& error_handler, Environment& env)
    : error_handler(error_handler), env(env)
{
}

void ReturnsChecker::check(Visitable* v)
{
    v->accept(this);
}

void ReturnsChecker::visitProg(Prog* t) { } //abstract class
void ReturnsChecker::visitTopDef(TopDef* t) {} //abstract class
void ReturnsChecker::visitArg(Arg* t) {} //abstract class
void ReturnsChecker::visitBlk(Blk* t) {} //abstract class
void ReturnsChecker::visitStmt(Stmt* t) {} //abstract class
void ReturnsChecker::visitItem(Item* t) {} //abstract class
void ReturnsChecker::visitType(Type* t) {} //abstract class
void ReturnsChecker::visitExpr(Expr* t) {} //abstract class
void ReturnsChecker::visitAddOp(AddOp* t) {} //abstract class
void ReturnsChecker::visitMulOp(MulOp* t) {} //abstract class
void ReturnsChecker::visitRelOp(RelOp* t) {} //abstract class

void ReturnsChecker::visitProgram(Program* program)
{
    program->listtopdef_->accept(this);
}

void ReturnsChecker::visitFnDef(FnDef* fndef)
{
    this->r_flag = false;
    fndef->blk_->accept(this);
    Environment::FunInfoPtr f = this->env.get_function(fndef->ident_);
    if ((!this->r_flag) && (!(check_is<Void *>(f->ret_type))))
    {
        std::string msg;
        msg += "Function : `";
        msg += fndef->ident_;
        msg += "` should returns [";
        msg += type_pretty_print(f->ret_type);
        msg += "].";
        this->error_handler.error(fndef->type_->line_number, msg);
    }
}

void ReturnsChecker::visitArgument(Argument* argument)
{
    argument->type_->accept(this);
    visitIdent(argument->ident_);
}

void ReturnsChecker::visitStmBlock(StmBlock* stmblock)
{
    stmblock->liststmt_->accept(this);
}

void ReturnsChecker::visitStmEmpty(StmEmpty* stmempty)
{
}

void ReturnsChecker::visitStmBStmt(StmBStmt* stmbstmt)
{
    stmbstmt->blk_->accept(this);
}

void ReturnsChecker::visitStmDecl(StmDecl* stmdecl)
{
    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);
}

void ReturnsChecker::visitStmAss(StmAss* stmass)
{
    visitIdent(stmass->ident_);
    stmass->expr_->accept(this);
}

void ReturnsChecker::visitStmIncr(StmIncr* stmincr)
{
    visitIdent(stmincr->ident_);
}

void ReturnsChecker::visitStmDecr(StmDecr* stmdecr)
{
    visitIdent(stmdecr->ident_);
}

void ReturnsChecker::visitStmRet(StmRet* stmret)
{
    stmret->expr_->accept(this);
    this->r_flag = true;
}

void ReturnsChecker::visitStmVRet(StmVRet* stmvret)
{
    this->r_flag = true;
}

void ReturnsChecker::visitStmCond(StmCond* stmcond)
{
    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);
}

void ReturnsChecker::visitStmCondElse(StmCondElse* stmcondelse)
{
    bool r = this->r_flag;
    if (!r){
        stmcondelse->expr_->accept(this);
        stmcondelse->stmt_1->accept(this);
        r = r || this->r_flag;
        stmcondelse->stmt_2->accept(this);
        r = r && this->r_flag;
    }
    this->r_flag = r;
}

void ReturnsChecker::visitStmWhile(StmWhile* stmwhile)
{
    // expr always positive -> infty loop -> this same as return.
    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);
}

void ReturnsChecker::visitStmSExp(StmSExp* stmsexp)
{
    stmsexp->expr_->accept(this);
}

void ReturnsChecker::visitStmNoInit(StmNoInit* stmnoinit)
{
    visitIdent(stmnoinit->ident_);
}

void ReturnsChecker::visitStmInit(StmInit* stminit)
{
    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);
}

void ReturnsChecker::visitInt(Int* integer)
{
}

void ReturnsChecker::visitStr(Str* str)
{
}

void ReturnsChecker::visitBool(Bool* boolean)
{
}

void ReturnsChecker::visitVoid(Void* void_field)
{
}

void ReturnsChecker::visitFun(Fun* fun)
{
    fun->type_->accept(this);
    fun->listtype_->accept(this);
}

void ReturnsChecker::visitEVar(EVar* evar)
{
    visitIdent(evar->ident_);
}

void ReturnsChecker::visitELitInt(ELitInt* elitint)
{
    visitInteger(elitint->integer_);
}

void ReturnsChecker::visitELitTrue(ELitTrue* elittrue)
{
    this->expression_optimization = 1;
}

void ReturnsChecker::visitELitFalse(ELitFalse* elitfalse)
{
    this->expression_optimization = -1;
}

void ReturnsChecker::visitEApp(EApp* eapp)
{
    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);
}

void ReturnsChecker::visitEString(EString* estring)
{
    visitString(estring->string_);
}

void ReturnsChecker::visitNeg(Neg* neg)
{
    neg->expr_->accept(this);
    this->expression_optimization = 0;
}

void ReturnsChecker::visitNot(Not* not_field)
{
    this->expression_optimization = 0;
    not_field->expr_->accept(this);
    this->expression_optimization = -(this->expression_optimization);
}

void ReturnsChecker::visitEMul(EMul* emul)
{
    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);
    this->expression_optimization = 0;
}

void ReturnsChecker::visitEAdd(EAdd* eadd)
{
    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);
    this->expression_optimization = 0;

}

void ReturnsChecker::visitERel(ERel* erel)
{
    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);
    // TODO: expression optimization
    this->expression_optimization = 0;
}

void ReturnsChecker::visitEAnd(EAnd* eand)
{
    eand->expr_1->accept(this);
    short eo1 = this->expression_optimization;
    eand->expr_2->accept(this);
    short eo2 = this->expression_optimization;

    if ((eo1 == 1) && (eo2 == 1))
    {
        this->expression_optimization = 1;
    }
    else if ((eo1 == 0) && (eo2 == 0))
    {
        this->expression_optimization = -1;
    }
    else
    {
        this->expression_optimization = 0;
    }
}

void ReturnsChecker::visitEOr(EOr* eor)
{
    eor->expr_1->accept(this);
    short eo1 = this->expression_optimization;
    eor->expr_2->accept(this);
    short eo2 = this->expression_optimization;
    if ((eo1 == 1) || (eo2 == 1))
    {
        this->expression_optimization = 1;
    }
    else if ((eo1 == 0) && (eo2 == 0))
    {
        this->expression_optimization = -1;
    }
    else
    {
        this->expression_optimization = 0;
    }
}

void ReturnsChecker::visitPlus(Plus* plus)
{
}

void ReturnsChecker::visitMinus(Minus* minus)
{
}

void ReturnsChecker::visitTimes(Times* times)
{
}

void ReturnsChecker::visitDiv(Div* div)
{
}

void ReturnsChecker::visitMod(Mod* mod)
{
}

void ReturnsChecker::visitLTH(LTH* lth)
{
}

void ReturnsChecker::visitLE(LE* le)
{
}

void ReturnsChecker::visitGTH(GTH* gth)
{
}

void ReturnsChecker::visitGE(GE* ge)
{
}

void ReturnsChecker::visitEQU(EQU* equ)
{
}

void ReturnsChecker::visitNE(NE* ne)
{
}


void ReturnsChecker::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ReturnsChecker::visitListArg(ListArg* listarg)
{
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ReturnsChecker::visitListStmt(ListStmt* liststmt)
{
    for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ReturnsChecker::visitListItem(ListItem* listitem)
{
    for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ReturnsChecker::visitListType(ListType* listtype)
{
    for (ListType::iterator i = listtype->begin() ; i != listtype->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ReturnsChecker::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void ReturnsChecker::visitInteger(Integer x)
{
    /* Code for Integer Goes Here*/
}

void ReturnsChecker::visitChar(Char x)
{
    /* Code for Char Goes Here*/
}

void ReturnsChecker::visitDouble(Double x)
{
    /* Code for Double Goes Here*/
}

void ReturnsChecker::visitString(String x)
{
    /* Code for String Goes Here*/
}

void ReturnsChecker::visitIdent(Ident x)
{
}


} /* namespace frontend */
