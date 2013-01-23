/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef FUNCTIONLOADER_H_
#define FUNCTIONLOADER_H_

#include "Absyn.H"
#include "Environment.h"
#include "ErrorHandler.h"

namespace frontend
{

class FunctionLoader : public Visitor
{
private:
    ErrorHandler& error_handler;
    Environment& env;

    bool loaded_classes_no_ext_and_fun;
    static std::string default_cls_name;
    std::string& last_cls_name;

public:
    FunctionLoader(ErrorHandler& error_handler, Environment& env);
    void check(Visitable* v);

    void visitProg(Prog* p);
    void visitTopDef(TopDef* p);
    void visitArg(Arg* p);
    void visitClsDef(ClsDef* p);
    void visitBlk(Blk* p);
    void visitStmt(Stmt* p);
    void visitItem(Item* p);
    void visitStructuredIdent(StructuredIdent* p);
    void visitType(Type* p);
    void visitExpr(Expr* p);
    void visitAddOp(AddOp* p);
    void visitMulOp(MulOp* p);
    void visitRelOp(RelOp* p);
    void visitProgram(Program* p);
    void visitFnDef(FnDef* p);
    void visitClsDefNoInher(ClsDefNoInher* p);
    void visitClsDefInher(ClsDefInher* p);
    void visitArgument(Argument* p);
    void visitMethodDef(MethodDef* p);
    void visitFieldDef(FieldDef* p);
    void visitStmBlock(StmBlock* p);
    void visitStmEmpty(StmEmpty* p);
    void visitStmBStmt(StmBStmt* p);
    void visitStmDecl(StmDecl* p);
    void visitStmNoInit(StmNoInit* p);
    void visitStmInit(StmInit* p);
    void visitStmInitArray(StmInitArray* p);
    void visitStmInitObj(StmInitObj* p);
    void visitSingleIdent(SingleIdent* p);
    void visitObjectIdent(ObjectIdent* p);
    void visitTableVal(TableVal* p);
    void visitSelfIdent(SelfIdent* p);
    void visitStmAss(StmAss* p);
    void visitStmAssArr(StmAssArr* p);
    void visitStmAssObj(StmAssObj* p);
    void visitStmIncr(StmIncr* p);
    void visitStmDecr(StmDecr* p);
    void visitStmRet(StmRet* p);
    void visitStmVRet(StmVRet* p);
    void visitStmCond(StmCond* p);
    void visitStmCondElse(StmCondElse* p);
    void visitStmWhile(StmWhile* p);
    void visitStmForeach(StmForeach* p);
    void visitStmSExp(StmSExp* p);
    void visitClass(Class* p);
    void visitInt(Int* p);
    void visitStr(Str* p);
    void visitBool(Bool* p);
    void visitVoid(Void* p);
    void visitTType(TType* p);
    void visitEVar(EVar* p);
    void visitELitInt(ELitInt* p);
    void visitELitTrue(ELitTrue* p);
    void visitELitFalse(ELitFalse* p);
    void visitELitNull(ELitNull* p);
    void visitEApp(EApp* p);
    void visitEMethodApp(EMethodApp* p);
    void visitEString(EString* p);
    void visitNeg(Neg* p);
    void visitNot(Not* p);
    void visitEDynamicCast(EDynamicCast* p);
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
    void visitListClsDef(ListClsDef* p);
    void visitListStmt(ListStmt* p);
    void visitListItem(ListItem* p);
    void visitListStructuredIdent(ListStructuredIdent* p);
    void visitListExpr(ListExpr* p);

    void visitInteger(Integer x);
    void visitChar(Char x);
    void visitDouble(Double x);
    void visitString(String x);
    void visitIdent(Ident x);
};

} /* namespace frontend */
#endif /* FUNCTIONLOADER_H_ */
