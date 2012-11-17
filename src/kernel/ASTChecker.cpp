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
    : error_handler(error_handler), env(env),
      last_declaration_type(0), last_type(0),
      last_function_ident(pr_name)
{
}

void ASTChecker::check(Visitable* v)
{
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

    this->add_variable(argument->type_, argument->ident_, argument->line_number);
}

void ASTChecker::visitStmBlock(StmBlock* stmblock)
{
    // Do not add new env because it should be already done.
    stmblock->liststmt_->accept(this);
}

void ASTChecker::visitStmEmpty(StmEmpty* stmempty)
{
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
    stmdecl->type_->accept(this);
    this->last_declaration_type = stmdecl->type_;
    stmdecl->listitem_->accept(this);
    this->last_declaration_type = 0;
}

void ASTChecker::visitStmAss(StmAss* stmass)
{
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
    this->last_type = 0;
    stmsexp->expr_->accept(this);
}

void ASTChecker::visitStmNoInit(StmNoInit* stmnoinit)
{
    if (this->last_declaration_type == 0)
        throw "AST error, variable declaration.";

    visitIdent(stmnoinit->ident_);
    this->add_variable(this->last_declaration_type, stmnoinit->ident_,
            stmnoinit->line_number);
}

void ASTChecker::visitStmInit(StmInit* stminit)
{
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
    visitInteger(elitint->integer_);
    this->last_type = &(this->lineral_int);
}

void ASTChecker::visitELitTrue(ELitTrue* elittrue)
{
    this->last_type = &(this->literal_bool);
}

void ASTChecker::visitELitFalse(ELitFalse* elitfalse)
{
    this->last_type = &(this->literal_bool);
}

void ASTChecker::visitEApp(EApp* eapp)
{
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

    eapp->listexpr_->accept(this);
    // Check if arguments are ok.
    bool argument_types_ok = true;
    ListExpr::iterator app_it = eapp->listexpr_->begin();
    Ident argument("funciton `");
    argument += eapp->ident_;
    argument += "` argument";
    for(std::vector<Environment::VarInfoPtr>::iterator it = fun_ptr->arguments.begin();
            it != fun_ptr->arguments.end(); it++)
    {
        if (app_it == eapp->listexpr_->end()) {
            // Not enough in application.
            argument_types_ok = false;
            break;
        }
        /* check type */
        this->check_type(argument, (*it)->type, app_it->)
        app_it++;
    }
    argument_types_ok &&= (app_it == eapp->listexpr_->end());

}

void ASTChecker::visitEString(EString* estring)
{
    visitString(estring->string_);
    this->last_type = &(this->literal_string);
}

void ASTChecker::visitNeg(Neg* neg)
{
    neg->expr_->accept(this);

}

void ASTChecker::visitNot(Not* not_field)
{
    /* Code For Not Goes Here*/

    not_field->expr_->accept(this);

}

void ASTChecker::visitEMul(EMul* emul)
{
    /* Code For EMul Goes Here*/

    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);

}

void ASTChecker::visitEAdd(EAdd* eadd)
{
    /* Code For EAdd Goes Here*/

    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);

}

void ASTChecker::visitERel(ERel* erel)
{
    /* Code For ERel Goes Here*/

    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);

}

void ASTChecker::visitEAnd(EAnd* eand)
{
    /* Code For EAnd Goes Here*/

    eand->expr_1->accept(this);
    eand->expr_2->accept(this);

}

void ASTChecker::visitEOr(EOr* eor)
{
    /* Code For EOr Goes Here*/

    eor->expr_1->accept(this);
    eor->expr_2->accept(this);

}

void ASTChecker::visitPlus(Plus* plus)
{
    /* Code For Plus Goes Here*/


}

void ASTChecker::visitMinus(Minus* minus)
{
    /* Code For Minus Goes Here*/


}

void ASTChecker::visitTimes(Times* times)
{
    /* Code For Times Goes Here*/


}

void ASTChecker::visitDiv(Div* div)
{
    /* Code For Div Goes Here*/


}

void ASTChecker::visitMod(Mod* mod)
{
    /* Code For Mod Goes Here*/


}

void ASTChecker::visitLTH(LTH* lth)
{
    /* Code For LTH Goes Here*/


}

void ASTChecker::visitLE(LE* le)
{
    /* Code For LE Goes Here*/


}

void ASTChecker::visitGTH(GTH* gth)
{
    /* Code For GTH Goes Here*/


}

void ASTChecker::visitGE(GE* ge)
{
    /* Code For GE Goes Here*/


}

void ASTChecker::visitEQU(EQU* equ)
{
    /* Code For EQU Goes Here*/


}

void ASTChecker::visitNE(NE* ne)
{
    /* Code For NE Goes Here*/


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

void ASTChecker::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void ASTChecker::visitInteger(Integer x)
{
    /* Code for Integer Goes Here*/
}

void ASTChecker::visitChar(Char x)
{
    /* Code for Char Goes Here*/
}

void ASTChecker::visitDouble(Double x)
{
    /* Code for Double Goes Here*/
}

void ASTChecker::visitString(String x)
{
    /* Code for String Goes Here*/
}

void ASTChecker::visitIdent(Ident x)
{
}

} /* namespace frontend*/
