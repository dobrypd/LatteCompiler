/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef CREATOR_X86_H_
#define CREATOR_X86_H_

#include "Absyn.H"
#include "Environment.h"
#include "InstructionManager.h"
#include "CompilerEnvironment.h"


namespace backend
{

class Creator_x86 : public Visitor
{
private:
    InstructionManager& instruction_manager;
    CompilerEnvironment env;
    frontend::Environment& fr_env;

    // Traverse variables
    // Declarations:
    Type* declaration_type;
    std::string last_class_name;
    // Statements:
    bool current_var_on_stack = false;
    bool current_var_is_addr = false;
    // Expressions:


    // End of traverse vars

    static const char* self_name;

    static std::string method_ident(std::string& class_name,
            std::string& method_name);

public:
    Creator_x86(InstructionManager& instruction_manager,
            frontend::Environment& fr_env);


    void visitProg(Prog* p);
    void visitTopDef(TopDef* p);
    void visitArg(Arg* p);
    void visitClsDef(ClsDef* p);
    void visitBlk(Blk* p);
    void visitStmt(Stmt* p);
    void visitItem(Item* p);
    void visitStructuredIdent(StructuredIdent* p);
    void visitArrayIndex(ArrayIndex* p);
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
    void visitTableVal(TableVal* p);
    void visitSelfIdent(SelfIdent* p);
    void visitExprIndex(ExprIndex* p);
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
    void visitListArrayIndex(ListArrayIndex* p);
    void visitListStructuredIdent(ListStructuredIdent* p);
    void visitListExpr(ListExpr* p);

    void visitInteger(Integer x);
    void visitChar(Char x);
    void visitDouble(Double x);
    void visitString(String x);
    void visitIdent(Ident x);

};


} /* namespace backend */

#endif /* CREATOR_X86_H_ */
