/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "TreeOptimizer.h"
#include "ErrorHandler.h"

namespace frontend
{

TreeOptimizer::TreeOptimizer()
{
}

void TreeOptimizer::optimize(Visitable* v)
{
    v->accept(this);
}

void TreeOptimizer::visitProg(Prog* t) { } //abstract class
void TreeOptimizer::visitTopDef(TopDef* t) {} //abstract class
void TreeOptimizer::visitArg(Arg* t) {} //abstract class
void TreeOptimizer::visitBlk(Blk* t) {} //abstract class
void TreeOptimizer::visitStmt(Stmt* t) {} //abstract class
void TreeOptimizer::visitItem(Item* t) {} //abstract class
void TreeOptimizer::visitType(Type* t) {} //abstract class
void TreeOptimizer::visitExpr(Expr* t) {} //abstract class
void TreeOptimizer::visitAddOp(AddOp* t) {} //abstract class
void TreeOptimizer::visitMulOp(MulOp* t) {} //abstract class
void TreeOptimizer::visitRelOp(RelOp* t) {} //abstract class

void TreeOptimizer::visitProgram(Program* program)
{
    program->listtopdef_->accept(this);
}

void TreeOptimizer::visitFnDef(FnDef* fndef)
{
    fndef->type_->accept(this);
    visitIdent(fndef->ident_);
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);

}

void TreeOptimizer::visitArgument(Argument* argument)
{
    argument->type_->accept(this);
    visitIdent(argument->ident_);
}

void TreeOptimizer::visitStmBlock(StmBlock* stmblock)
{
    stmblock->liststmt_->accept(this);
}

void TreeOptimizer::visitStmEmpty(StmEmpty* stmempty)
{
}

void TreeOptimizer::visitStmBStmt(StmBStmt* stmbstmt)
{
    stmbstmt->blk_->accept(this);
}

void TreeOptimizer::visitStmDecl(StmDecl* stmdecl)
{
    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);
}

void TreeOptimizer::visitStmAss(StmAss* stmass)
{
    visitIdent(stmass->ident_);
    stmass->expr_->accept(this);
}

void TreeOptimizer::visitStmIncr(StmIncr* stmincr)
{
    visitIdent(stmincr->ident_);
}

void TreeOptimizer::visitStmDecr(StmDecr* stmdecr)
{
    visitIdent(stmdecr->ident_);
}

void TreeOptimizer::visitStmRet(StmRet* stmret)
{
    stmret->expr_->accept(this);
}

void TreeOptimizer::visitStmVRet(StmVRet* stmvret)
{
}

void TreeOptimizer::visitStmCond(StmCond* stmcond)
{
    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);
}

void TreeOptimizer::visitStmCondElse(StmCondElse* stmcondelse)
{
    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);
}

void TreeOptimizer::visitStmWhile(StmWhile* stmwhile)
{
    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);
}

void TreeOptimizer::visitStmSExp(StmSExp* stmsexp)
{
    stmsexp->expr_->accept(this);
}

void TreeOptimizer::visitStmNoInit(StmNoInit* stmnoinit)
{
    visitIdent(stmnoinit->ident_);
}

void TreeOptimizer::visitStmInit(StmInit* stminit)
{
    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);
}

void TreeOptimizer::visitInt(Int* integer)
{
}

void TreeOptimizer::visitStr(Str* str)
{
}

void TreeOptimizer::visitBool(Bool* boolean)
{
}

void TreeOptimizer::visitVoid(Void* void_field)
{
}

void TreeOptimizer::visitFun(Fun* fun)
{
    fun->type_->accept(this);
    fun->listtype_->accept(this);
}

void TreeOptimizer::visitEVar(EVar* evar)
{
    visitIdent(evar->ident_);
}

void TreeOptimizer::visitELitInt(ELitInt* elitint)
{
    visitInteger(elitint->integer_);
}

void TreeOptimizer::visitELitTrue(ELitTrue* elittrue)
{
}

void TreeOptimizer::visitELitFalse(ELitFalse* elitfalse)
{
}

void TreeOptimizer::visitEApp(EApp* eapp)
{
    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);
}

void TreeOptimizer::visitEString(EString* estring)
{
    visitString(estring->string_);
}

void TreeOptimizer::visitNeg(Neg* neg)
{
    neg->expr_->accept(this);
}

void TreeOptimizer::visitNot(Not* not_field)
{
    not_field->expr_->accept(this);
}

void TreeOptimizer::visitEMul(EMul* emul)
{
    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);
}

void TreeOptimizer::visitEAdd(EAdd* eadd)
{
    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);
}

void TreeOptimizer::visitERel(ERel* erel)
{
    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);

}

void TreeOptimizer::visitEAnd(EAnd* eand)
{
    eand->expr_1->accept(this);
    eand->expr_2->accept(this);
}

void TreeOptimizer::visitEOr(EOr* eor)
{
    eor->expr_1->accept(this);
    eor->expr_2->accept(this);
}

void TreeOptimizer::visitPlus(Plus* plus)
{
}

void TreeOptimizer::visitMinus(Minus* minus)
{
}

void TreeOptimizer::visitTimes(Times* times)
{
}

void TreeOptimizer::visitDiv(Div* div)
{
}

void TreeOptimizer::visitMod(Mod* mod)
{
}

void TreeOptimizer::visitLTH(LTH* lth)
{
}

void TreeOptimizer::visitLE(LE* le)
{
}

void TreeOptimizer::visitGTH(GTH* gth)
{
}

void TreeOptimizer::visitGE(GE* ge)
{
}

void TreeOptimizer::visitEQU(EQU* equ)
{
}

void TreeOptimizer::visitNE(NE* ne)
{
}


void TreeOptimizer::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListArg(ListArg* listarg)
{
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListStmt(ListStmt* liststmt)
{
    for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListItem(ListItem* listitem)
{
    for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListType(ListType* listtype)
{
    for (ListType::iterator i = listtype->begin() ; i != listtype->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void TreeOptimizer::visitInteger(Integer x)
{
}

void TreeOptimizer::visitChar(Char x)
{
}

void TreeOptimizer::visitDouble(Double x)
{
}

void TreeOptimizer::visitString(String x)
{
}

void TreeOptimizer::visitIdent(Ident x)
{
}


} /* namespace frontend */
