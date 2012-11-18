/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef RETURNSCHECKER_H_
#define RETURNSCHECKER_H_

#include "Absyn.H"
#include "ErrorHandler.h"
#include "Environment.h"

namespace frontend
{

class ReturnsChecker : public Visitor
{
private:
    ErrorHandler& error_handler;
    Environment& env;

    bool r_flag; // return was found in this function.

    // 0 - do not know, -1 - always false, 1 -  always true, 2 - constant variable
    short expression_optimization;

    int const_int_value;
    int e1, e2;
    bool const_bool_value;
    std::string const_str_value;

    bool remove;
    bool to_the_end;

public:
    ReturnsChecker(ErrorHandler& error_handler, Environment& env);
    void check(Visitable* v);

    void visitProg(Prog* p);
    void visitTopDef(TopDef* p);
    void visitArg(Arg* p);
    void visitBlk(Blk* p);
    void visitStmt(Stmt* p);
    void visitItem(Item* p);
    void visitType(Type* p);
    void visitExpr(Expr* p);
    void visitAddOp(AddOp* p);
    void visitMulOp(MulOp* p);
    void visitRelOp(RelOp* p);
    void visitProgram(Program* p);
    void visitFnDef(FnDef* p);
    void visitArgument(Argument* p);
    void visitStmBlock(StmBlock* p);
    void visitStmEmpty(StmEmpty* p);
    void visitStmBStmt(StmBStmt* p);
    void visitStmDecl(StmDecl* p);
    void visitStmNoInit(StmNoInit* p);
    void visitStmInit(StmInit* p);
    void visitStmAss(StmAss* p);
    void visitStmIncr(StmIncr* p);
    void visitStmDecr(StmDecr* p);
    void visitStmRet(StmRet* p);
    void visitStmVRet(StmVRet* p);
    void visitStmCond(StmCond* p);
    void visitStmCondElse(StmCondElse* p);
    void visitStmWhile(StmWhile* p);
    void visitStmSExp(StmSExp* p);
    void visitInt(Int* p);
    void visitStr(Str* p);
    void visitBool(Bool* p);
    void visitVoid(Void* p);
    void visitFun(Fun* p);
    void visitEVar(EVar* p);
    void visitELitInt(ELitInt* p);
    void visitELitTrue(ELitTrue* p);
    void visitELitFalse(ELitFalse* p);
    void visitEApp(EApp* p);
    void visitEString(EString* p);
    void visitNeg(Neg* p);
    void visitNot(Not* p);
    void visitEMul(EMul* p);
    void visitEAdd(EAdd* p);
    void visitERel(ERel* p);
    void visitEAnd(EAnd* p);
    void visitEOr(EOr* p);
    void visitPlus(Plus* p);
    void visitMinus(Minus* p);
    void visitTimes(Times* p);
    void visitDiv(Div* p);
    void visitMod(Mod* p);
    void visitLTH(LTH* p);
    void visitLE(LE* p);
    void visitGTH(GTH* p);
    void visitGE(GE* p);
    void visitEQU(EQU* p);
    void visitNE(NE* p);
    void visitListTopDef(ListTopDef* p);
    void visitListArg(ListArg* p);
    void visitListStmt(ListStmt* p);
    void visitListItem(ListItem* p);
    void visitListType(ListType* p);
    void visitListExpr(ListExpr* p);

    void visitInteger(Integer x);
    void visitChar(Char x);
    void visitDouble(Double x);
    void visitString(String x);
    void visitIdent(Ident x);
};

} /* namespace frontend */
#endif /* RETURNSCHECKER_H_ */
