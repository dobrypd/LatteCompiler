/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef JVMGENERATOR_H_
#define JVMGENERATOR_H_

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Absyn.H"
#include "Environment.h"
#include "JVMEnvironment.h"

namespace jvm
{

class JVMGenerator : public Visitor
{
private:
    static const char* runtime_lib;

    std::string jasmin_file_name;
    std::stringstream* jvm;

    std::string class_name;

    frontend::Environment& env; // Only for functions.
    JVMEnvironment jvm_e;

    int current_function_locals_size;
    int current_function_stack_size;
    int max_stack_size;

    Type* last_declaration_type;
    Type* last_function_type;
    Type* last_type; // In expression.

    // Literal types;
    Int literal_int;
    Bool literal_bool;
    Str literal_string;

    int next_label;
    int last_true_label;
    int last_false_label;
    bool last_is_zero;
    bool pop_if_zero;
    bool e_was_rel;  // True if not in stack.

    std::string type_to_jvm_type(Type* type, bool is_arg);
    void bool_expr_to_stack(int label_t, int label_f);

public:
    JVMGenerator(std::string output_file_name, frontend::Environment& env);

    void generate(Visitable* ast_root);


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

} /* namespace jvm */
#endif /* JVMGENERATOR_H_ */
