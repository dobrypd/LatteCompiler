/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef ASTCHECKER_H_
#define ASTCHECKER_H_

#include <boost/shared_ptr.hpp>
#include "Absyn.H"
#include "ErrorHandler.h"
#include "Environment.h"

namespace frontend
{

class ASTChecker : public Visitor
{
private:
    ErrorHandler& error_handler;
    Environment& env;

    /* STATE */
    int last_line_number;
    bool error_flag;
    Type* last_declaration_type;
    Type* last_type;  // For expressions.
    Type* last_function_type;
    Type* ident_type; // For idents list.
    std::string* last_function_ident;
    std::string* last_class_ident;
    std::vector<Environment::VarInfoPtr>::const_iterator last_arguments_iterator;
    std::vector<Environment::VarInfoPtr>::const_iterator last_arguments_end;

    // Literal types;
    Int literal_int;
    Bool literal_bool;
    Str literal_string;
    Class literal_class;
    static std::string null_class;

    void add_variable(Type* t, Ident& ident, int line_number);
    void check_type(ListStructuredIdent* i1, Type* t1, ListStructuredIdent* i2, Type* t2, int line_number);
    std::string args_pretty_print(std::vector<Environment::VarInfoPtr>::const_iterator it,
            std::vector<Environment::VarInfoPtr>::const_iterator end);

    bool is_subclass(Class* base, Class* maybe_inher);

public:
    ASTChecker(ErrorHandler& error_handler, Environment& env, Ident& pr_name);
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
    void visitEArrVar(EArrVar* p);
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

#endif /* ASTCHECKER_H_ */
