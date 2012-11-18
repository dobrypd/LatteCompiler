/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <string>
#include <iostream>
#include <vector>
#include "ASTChecker.h"
#include "ErrorHandler.h"
#include "global.h"

namespace frontend
{

ASTChecker::ASTChecker(ErrorHandler& error_handler, Environment& env, Ident& pr_name)
    : error_handler(error_handler), env(env), last_line_number(0),
      last_declaration_type(0), last_type(0),
      last_function_ident(pr_name),
      last_arguments_iterator(0), last_arguments_end(0)
{
}

void ASTChecker::check(Visitable* v)
{
    // TODO: renew state.
    last_line_number = 0;
    v->accept(this);
}

void ASTChecker::add_variable(Type* t, Ident& ident, int line_number)
{
    if (this->env.can_add_variable(ident))
        this->env.add_variable(t, ident);
    else
    {
        std::string msg;
        msg += "variable `";
        msg += ident;
        msg += "` already declared.";
        this->error_handler.error(line_number, msg);
    }
}

void ASTChecker::check_type(Ident& i1, Type* t1, Ident& i2, Type* t2, int line_number)
{
    if (t1 == 0)
    {
        std::string msg;
        msg += "Type error, cannot find type of : `";
        msg += i1;
        msg += "`.";
        this->error_handler.error(line_number, msg);
        return ;
    }
    if (t2 == 0)
    {
        std::string msg;
        msg += "Type error, cannot find type of : `";
        msg += i2;
        msg += "`.";
        this->error_handler.error(line_number, msg);
        return ;
    }

    if (*(t1) == *(t2))
    {
        // OK
        ;
    }
    else
    {
        std::string msg;
        msg += "Type error: `";
        msg += i1;
        msg += "` has different type [";
        msg += type_pretty_print(t1);
        msg += "] than `";
        msg += i2;
        msg += "` with type [";
        msg += type_pretty_print(t2);
        msg += "].";
        this->error_handler.error(line_number, msg);
    }
}

std::string ASTChecker::args_pretty_print(std::vector<Environment::VarInfoPtr>::const_iterator it,
            std::vector<Environment::VarInfoPtr>::const_iterator end)
{
    std::string s;
    for(; it != end; it++){
        s += type_pretty_print((*it)->type);
        if ((it + 1) != end)
            s += ", ";
    }
    return s;
}

void ASTChecker::visitProg(Prog* t) { } //abstract class
void ASTChecker::visitTopDef(TopDef* t) {} //abstract class
void ASTChecker::visitArg(Arg* t) {} //abstract class
void ASTChecker::visitBlk(Blk* t) {} //abstract class
void ASTChecker::visitStmt(Stmt* t) {} //abstract class
void ASTChecker::visitItem(Item* t) {} //abstract class
void ASTChecker::visitType(Type* t) {} //abstract class
void ASTChecker::visitExpr(Expr* t) {} //abstract class
void ASTChecker::visitAddOp(AddOp* t) {} //abstract class
void ASTChecker::visitMulOp(MulOp* t) {} //abstract class
void ASTChecker::visitRelOp(RelOp* t) {} //abstract class

void ASTChecker::visitProgram(Program* program)
{
    program->listtopdef_->accept(this);
}

void ASTChecker::visitFnDef(FnDef* fndef)
{
    this->last_function_type = 0;
    fndef->type_->accept(this);
    this->last_function_type = fndef->type_;
    visitIdent(fndef->ident_);
    std::cout << std::endl << std::endl << std::endl << fndef->ident_ << std::endl << std::endl << std::endl;
    std::cout << this->last_function_ident << std::endl;
    this->last_function_ident = fndef->ident_;

    this->env.prepare();
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);
    this->env.back();
    this->last_function_type = 0;
}

void ASTChecker::visitArgument(Argument* argument)
{
    argument->type_->accept(this);
    visitIdent(argument->ident_);
    this->last_line_number = argument->line_number;
    this->add_variable(argument->type_, argument->ident_, argument->line_number);
}

void ASTChecker::visitStmBlock(StmBlock* stmblock)
{
    // Do not add new env because it should be already done.
    stmblock->liststmt_->accept(this);
}

void ASTChecker::visitStmEmpty(StmEmpty* stmempty)
{
    this->last_line_number = stmempty->line_number;
    std::cerr << "hello" << std::endl;  // TODO:
}

void ASTChecker::visitStmBStmt(StmBStmt* stmbstmt)
{
    this->env.prepare();
    stmbstmt->blk_->accept(this);
    this->env.back();
}

void ASTChecker::visitStmDecl(StmDecl* stmdecl)
{
    this->last_line_number = stmdecl->line_number;
    stmdecl->type_->accept(this);
    this->last_declaration_type = stmdecl->type_;
    stmdecl->listitem_->accept(this);
    this->last_declaration_type = 0;
}

void ASTChecker::visitStmAss(StmAss* stmass)
{
    this->last_line_number = stmass->line_number;
    visitIdent(stmass->ident_);
    Environment::VarInfoPtr var_info = this->env.get_variable(stmass->ident_);

    this->last_type = 0;
    stmass->expr_->accept(this);

    if (var_info == 0) {
        std::string msg = "variable `";
        msg += stmass->ident_;
        msg += "` used before declared.";
        this->error_handler.error(stmass->line_number, msg);
    }
    else
    {
        Ident expevalident = Ident("type evaluation of expression");
        this->check_type(stmass->ident_, var_info->type,
                expevalident, this->last_type,
                stmass->line_number);
    }
}

void ASTChecker::visitStmIncr(StmIncr* stmincr)
{
    this->last_line_number = stmincr->line_number;
    visitIdent(stmincr->ident_);
    Environment::VarInfoPtr var_info = this->env.get_variable(stmincr->ident_);
    if (var_info == 0) {
        std::string msg = "variable `";
        msg += stmincr->ident_;
        msg += "` used before declared.";
        this->error_handler.error(stmincr->line_number, msg);
        return;
    }
    etypes t = type_to_enum(var_info->type);
    if (t != INT)
    {
        std::string msg = "variable `";
        msg += stmincr->ident_;
        msg += "` with type [";
        msg += type_pretty_print(var_info->type);
        msg += "] cannot be used with ++ operator.";
        this->error_handler.error(stmincr->line_number, msg);
    }
}

void ASTChecker::visitStmDecr(StmDecr* stmdecr)
{
    this->last_line_number = stmdecr->line_number;
    visitIdent(stmdecr->ident_);
    Environment::VarInfoPtr var_info = this->env.get_variable(stmdecr->ident_);
    if (var_info == 0) {
        std::string msg = "variable `";
        msg += stmdecr->ident_;
        msg += "` used before declared.";
        this->error_handler.error(stmdecr->line_number, msg);
        return;
    }
    etypes t = type_to_enum(var_info->type);
    if (t != INT)
    {
        std::string msg = "variable `";
        msg += stmdecr->ident_;
        msg += "` with type [";
        msg += type_pretty_print(var_info->type);
        msg += "] cannot be used with -- operator.";
        this->error_handler.error(stmdecr->line_number, msg);
    }
}

void ASTChecker::visitStmRet(StmRet* stmret)
{
    this->last_line_number = stmret->line_number;
    this->last_type = 0;
    stmret->expr_->accept(this);
    Ident f_i("function ");
    f_i += this->last_function_ident;
    Ident expevalident = Ident("type evaluation of expression");
    this->check_type(f_i, this->last_function_type, expevalident,
            this->last_type, stmret->line_number);

}

void ASTChecker::visitStmVRet(StmVRet* stmvret)
{
    this->last_line_number = stmvret->line_number;
    if (!(check_is<Void *>(this->last_function_type)))
    {
        std::string msg = "function `";
        msg += this->last_function_ident;
        msg += "` is not void type, should return [";
        msg += type_pretty_print(this->last_function_type);
        msg += "].";
        this->error_handler.error(stmvret->line_number, msg);
    }
}

void ASTChecker::visitStmCond(StmCond* stmcond)
{
    this->last_line_number = stmcond->line_number;
    this->env.prepare();
    this->last_type = 0;
    stmcond->expr_->accept(this);
    if (this->last_type == 0)
    {
        std::string msg = "cannot evaluate expression";
        this->error_handler.error(stmcond->expr_->line_number, msg);

    }
    else
    if (!(check_is<Bool *>(this->last_type)))
    {
        std::string msg = "expression must evaluate to boolean type instead of [";
        msg += type_pretty_print(this->last_type);
        msg += "].";
        this->error_handler.error(stmcond->expr_->line_number, msg);
    }
    stmcond->stmt_->accept(this);
    this->env.back();

}

void ASTChecker::visitStmCondElse(StmCondElse* stmcondelse)
{
    this->last_line_number = stmcondelse->line_number;
    this->env.prepare();
    this->last_type = 0;
    stmcondelse->expr_->accept(this);
    if (this->last_type == 0)
    {
        std::string msg = "cannot evaluate expression";
        this->error_handler.error(stmcondelse->expr_->line_number, msg);

    }
    else
    if (!(check_is<Bool *>(this->last_type)))
    {
        std::string msg = "expression must evaluate to boolean type instead of [";
        msg += type_pretty_print(this->last_type);
        msg += "].";
        this->error_handler.error(stmcondelse->expr_->line_number, msg);
    }
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);
    this->env.back();
}

void ASTChecker::visitStmWhile(StmWhile* stmwhile)
{
    this->last_line_number = stmwhile->line_number;
    this->env.prepare();
    this->last_type = 0;
    stmwhile->expr_->accept(this);
    if (this->last_type == 0)
    {
        std::string msg = "cannot evaluate expression";
        this->error_handler.error(stmwhile->expr_->line_number, msg);

    }
    else
    if (!(check_is<Bool *>(this->last_type)))
    {
        std::string msg = "expression must evaluate to boolean type instead of [";
        msg += type_pretty_print(this->last_type);
        msg += "].";
        this->error_handler.error(stmwhile->expr_->line_number, msg);
    }
    stmwhile->stmt_->accept(this);
    this->env.back();

}

void ASTChecker::visitStmSExp(StmSExp* stmsexp)
{
    this->last_line_number = stmsexp->line_number;
    this->last_type = 0;
    stmsexp->expr_->accept(this);
}

void ASTChecker::visitStmNoInit(StmNoInit* stmnoinit)
{
    this->last_line_number = stmnoinit->line_number;
    if (this->last_declaration_type == 0)
        throw "AST error, variable declaration.";

    visitIdent(stmnoinit->ident_);
    this->add_variable(this->last_declaration_type, stmnoinit->ident_,
            stmnoinit->line_number);
}

void ASTChecker::visitStmInit(StmInit* stminit)
{
    this->last_line_number = stminit->line_number;
    if (this->last_declaration_type == 0)
        throw "AST error, variable declaration.";

    visitIdent(stminit->ident_);

    stminit->expr_->accept(this);

    Ident evalident("type evaluation of expression");
    this->check_type(stminit->ident_, this->last_declaration_type, evalident, this->last_type, stminit->line_number);
    this->add_variable(this->last_declaration_type, stminit->ident_, stminit->line_number);
}

void ASTChecker::visitInt(Int* integer)
{
}

void ASTChecker::visitStr(Str* str)
{
}

void ASTChecker::visitBool(Bool* boolean)
{
}

void ASTChecker::visitVoid(Void* void_field)
{
}

void ASTChecker::visitFun(Fun* fun)
{
    // In this version it's not used.
    fun->type_->accept(this);
    fun->listtype_->accept(this);

}

void ASTChecker::visitEVar(EVar* evar)
{
    this->last_line_number = evar->line_number;
    Environment::VarInfoPtr var_info = this->env.get_variable(evar->ident_);
    visitIdent(evar->ident_);
    if (var_info == 0) {
        std::string msg = "variable `";
        msg += evar->ident_;
        msg += "` used before declaration.";
        this->error_handler.error(evar->line_number, msg);
    }
    else
    {
        this->last_type = var_info->type;
    }
}

void ASTChecker::visitELitInt(ELitInt* elitint)
{
    this->last_line_number = elitint->line_number;
    visitInteger(elitint->integer_);
    this->last_type = &(this->lineral_int);
}

void ASTChecker::visitELitTrue(ELitTrue* elittrue)
{
    this->last_line_number = elittrue->line_number;
    this->last_type = &(this->literal_bool);
}

void ASTChecker::visitELitFalse(ELitFalse* elitfalse)
{
    this->last_line_number = elitfalse->line_number;
    this->last_type = &(this->literal_bool);
}

void ASTChecker::visitEApp(EApp* eapp)
{
    this->last_line_number = eapp->line_number;
    visitIdent(eapp->ident_);
    Environment::FunInfoPtr fun_ptr = this->env.get_function(eapp->ident_);
    // Check if function exists.
    if (fun_ptr == 0)
    {
        std::string msg = "function `";
        msg += eapp->ident_;
        msg += "` does not exist.";
        this->error_handler.error(eapp->line_number, msg);
        return;
    }

    // Check arguments.
    this->last_arguments_iterator = fun_ptr->arguments.begin();
    this->last_arguments_end = fun_ptr->arguments.end();
    eapp->listexpr_->accept(this);
    if (this->last_arguments_iterator != this->last_arguments_end)
    {
        std::string msg = "function `";
        msg += eapp->ident_;
        msg += "` need ";
        msg += (fun_ptr->arguments.size());
        msg += " arguments: ";
        msg += this->args_pretty_print(fun_ptr->arguments.begin(), fun_ptr->arguments.end());
        msg += ", but you passed only ";
        msg += (this->last_arguments_iterator - fun_ptr->arguments.begin());
        this->error_handler.error(eapp->line_number, msg);
    }
    // Args checked.

    this->last_type = fun_ptr->ret_type;
    // TODO:
}

void ASTChecker::visitEString(EString* estring)
{
    this->last_line_number = estring->line_number;
    visitString(estring->string_);
    this->last_type = &(this->literal_string);
}

void ASTChecker::visitNeg(Neg* neg)
{
    this->last_line_number = neg->line_number;
    neg->expr_->accept(this);

}

void ASTChecker::visitNot(Not* not_field)
{
    this->last_line_number = not_field->line_number;
    not_field->expr_->accept(this);

}

void ASTChecker::visitEMul(EMul* emul)
{
    this->last_line_number = emul->line_number;
    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);

}

void ASTChecker::visitEAdd(EAdd* eadd)
{
    this->last_line_number = eadd->line_number;
    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);

}

void ASTChecker::visitERel(ERel* erel)
{
    this->last_line_number = erel->line_number;
    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);

}

void ASTChecker::visitEAnd(EAnd* eand)
{
    this->last_line_number = eand->line_number;
    eand->expr_1->accept(this);
    eand->expr_2->accept(this);

}

void ASTChecker::visitEOr(EOr* eor)
{
    this->last_line_number = eor->line_number;
    eor->expr_1->accept(this);
    eor->expr_2->accept(this);

}

void ASTChecker::visitPlus(Plus* plus)
{

}

void ASTChecker::visitMinus(Minus* minus)
{

}

void ASTChecker::visitTimes(Times* times)
{

}

void ASTChecker::visitDiv(Div* div)
{

}

void ASTChecker::visitMod(Mod* mod)
{

}

void ASTChecker::visitLTH(LTH* lth)
{

}

void ASTChecker::visitLE(LE* le)
{

}

void ASTChecker::visitGTH(GTH* gth)
{

}

void ASTChecker::visitGE(GE* ge)
{

}

void ASTChecker::visitEQU(EQU* equ)
{

}

void ASTChecker::visitNE(NE* ne)
{

}


void ASTChecker::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ASTChecker::visitListArg(ListArg* listarg)
{
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ASTChecker::visitListStmt(ListStmt* liststmt)
{
    for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ASTChecker::visitListItem(ListItem* listitem)
{
    for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ASTChecker::visitListType(ListType* listtype)
{
    for (ListType::iterator i = listtype->begin() ; i != listtype->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

/* Used to check arguments in time of function call,
 * and only in this case!
 */
void ASTChecker::visitListExpr(ListExpr* listexpr)
{
    Ident fun_arg("function ");
    fun_arg += this->last_function_ident;
    fun_arg += " argument";

    int n = 0;
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        n++;
        Ident arg_no("argument number ");
        arg_no += n;

        (*i)->accept(this);
        if (this->last_arguments_iterator == this->last_arguments_end)
            return;
        this->check_type(fun_arg, (*(this->last_arguments_iterator))->type, arg_no, this->last_type, this->last_line_number);
    }
}


void ASTChecker::visitInteger(Integer x)
{
}

void ASTChecker::visitChar(Char x)
{
}

void ASTChecker::visitDouble(Double x)
{
}

void ASTChecker::visitString(String x)
{
}

void ASTChecker::visitIdent(Ident x)
{
}

} /* namespace frontend*/
