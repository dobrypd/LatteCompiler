/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "ASTChecker.h"
#include "ErrorHandler.h"
#include "global.h"

namespace frontend
{

ASTChecker::ASTChecker(ErrorHandler& error_handler, Environment& env, Ident& pr_name)
    : error_handler(error_handler), env(env), last_line_number(0),
      error_flag(false), last_declaration_type(0), last_type(0),
      last_arguments_iterator(0), last_arguments_end(0)
{
}

void ASTChecker::check(Visitable* v)
{
    this->last_line_number = 0;
    this->error_flag = false;
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

void ASTChecker::check_type(ListStructuredIdent* i1, Type* t1, ListStructuredIdent* i2, Type* t2, int line_number)
{
    if (t1 == 0)
    {
        std::string msg;
        msg += "Type error, cannot find type of : `";
        msg += ident_to_string(i1);
        msg += "`.";
        this->error_handler.error(line_number, msg);
        return ;
    }
    if (t2 == 0)
    {
        std::string msg;
        msg += "Type error, cannot find type of : `";
        msg += ident_to_string(i2);
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
        msg += ident_to_string(i1);
        msg += "` has different type [";
        msg += type_pretty_print(t1);
        msg += "] than `";
        msg += ident_to_string(i2);
        msg += "` with type [";
        msg += type_pretty_print(t2);
        msg += "].";
        this->error_handler.error(line_number, msg);
    }
}

std::string ASTChecker::args_pretty_print(std::vector<Environment::VarInfoPtr>::const_iterator it,
            std::vector<Environment::VarInfoPtr>::const_iterator end)
{
    std::string s("[");
    for(; it != end; it++){
        s += type_pretty_print((*it)->type);
        if ((it + 1) != end)
            s += ", ";
    }
    s += "]";
    return s;
}


void ASTChecker::visitProg(Prog* t) {} //abstract class
void ASTChecker::visitTopDef(TopDef* t) {} //abstract class
void ASTChecker::visitArg(Arg* t) {} //abstract class
void ASTChecker::visitClsDef(ClsDef* t) {} //abstract class
void ASTChecker::visitBlk(Blk* t) {} //abstract class
void ASTChecker::visitStmt(Stmt* t) {} //abstract class
void ASTChecker::visitItem(Item* t) {} //abstract class
void ASTChecker::visitStructuredIdent(StructuredIdent* t) {} //abstract class
void ASTChecker::visitArrayIndex(ArrayIndex* t) {} //abstract class
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
    //this->last_function_ident = fndef->ident_;
    //TODO last fnction ident

    this->env.prepare();
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);
    this->env.back();
    this->last_function_type = 0;
}

void ASTChecker::visitClsDefNoInher(ClsDefNoInher *clsdefnoinher)
{
  /* Latte++ */

  visitIdent(clsdefnoinher->ident_);
  clsdefnoinher->listclsdef_->accept(this);

}

void ASTChecker::visitClsDefInher(ClsDefInher *clsdefinher)
{
  /* Latte++ */

  visitIdent(clsdefinher->ident_1);
  visitIdent(clsdefinher->ident_2);
  clsdefinher->listclsdef_->accept(this);

}

void ASTChecker::visitArgument(Argument* argument)
{
    argument->type_->accept(this);
    visitIdent(argument->ident_);
    this->last_line_number = argument->line_number;
    this->add_variable(argument->type_, argument->ident_, argument->line_number);
}

void ASTChecker::visitMethodDef(MethodDef *methoddef)
{
    /* Latte++ */

  methoddef->type_->accept(this);
  visitIdent(methoddef->ident_);
  methoddef->listarg_->accept(this);
  methoddef->blk_->accept(this);

}

void ASTChecker::visitFieldDef(FieldDef *fielddef)
{
    /* Latte++ */

  fielddef->type_->accept(this);
  visitIdent(fielddef->ident_);

}

void ASTChecker::visitStmBlock(StmBlock* stmblock)
{
    // Do not add new env because it should be already done.
    stmblock->liststmt_->accept(this);
}

void ASTChecker::visitStmEmpty(StmEmpty* stmempty)
{
    this->last_line_number = stmempty->line_number;
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
    stmass->liststructuredident_->accept(this);
//    Environment::VarInfoPtr var_info = this->env.get_variable(stmass->liststructuredident_);
//
//    this->last_type = 0;
//    stmass->expr_->accept(this);
//
//    if (!var_info) {
//        std::string msg = "variable `";
//        msg += ident_to_string(stmass->liststructuredident_);
//        msg += "` used before declared.";
//        this->error_handler.error(stmass->line_number, msg);
//    }
//    else
//    {
//        //Ident expevalident = Ident("type evaluation of expression");
//        // TODO: second ident name
//        this->check_type(stmass->liststructuredident_, var_info->type,
//                stmass->liststructuredident_, this->last_type,
//                stmass->line_number);
//    }
}

void ASTChecker::visitStmAssArr(StmAssArr *stmassarr)
{
  /* Latte++ */

    stmassarr->liststructuredident_->accept(this);
    stmassarr->type_->accept(this);
    stmassarr->expr_->accept(this);

}

void ASTChecker::visitStmAssObj(StmAssObj *stmassobj)
{
    /* Latte++ */

    stmassobj->liststructuredident_->accept(this);
    stmassobj->type_->accept(this);

}

void ASTChecker::visitStmIncr(StmIncr* stmincr)
{
//    this->last_line_number = stmincr->line_number;
//    stmincr->liststructuredident_->accept(this);
//    Environment::VarInfoPtr var_info =
//            this->env.get_variable(stmincr->liststructuredident_);
//    if (!var_info) {
//        std::string msg = "variable `";
//        msg += ident_to_string(stmincr->liststructuredident_);
//        msg += "` used before declared.";
//        this->error_handler.error(stmincr->line_number, msg);
//        return;
//    }
//    if (!check_is<Int*>(var_info->type))
//    {
//        std::string msg = "variable `";
//        msg += ident_to_string(stmincr->liststructuredident_);
//        msg += "` with type [";
//        msg += type_pretty_print(var_info->type);
//        msg += "] cannot be used with ++ operator.";
//        this->error_handler.error(stmincr->line_number, msg);
//    }
}

void ASTChecker::visitStmDecr(StmDecr* stmdecr)
{
//    this->last_line_number = stmdecr->line_number;
//    stmdecr->liststructuredident_->accept(this);
//    Environment::VarInfoPtr var_info =
//            this->env.get_variable(stmdecr->liststructuredident_);
//    if (!var_info) {
//        std::string msg = "variable `";
//        msg += ident_to_string(stmdecr->liststructuredident_);
//        msg += "` used before declared.";
//        this->error_handler.error(stmdecr->line_number, msg);
//        return;
//    }
//    if (!check_is<Int*>(var_info->type))
//    {
//        std::string msg = "variable `";
//        //msg += stmdecr->ident_;
//        msg += ident_to_string(stmdecr->liststructuredident_);
//        msg += "` with type [";
//        msg += type_pretty_print(var_info->type);
//        msg += "] cannot be used with -- operator.";
//        this->error_handler.error(stmdecr->line_number, msg);
//    }
}

void ASTChecker::visitStmRet(StmRet* stmret)
{
    this->last_line_number = stmret->line_number;
    this->last_type = 0;
    stmret->expr_->accept(this);
    //Ident f_i("function ");
    //f_i += this->last_function_ident;
    //Ident expevalident = Ident("type evaluation of expression");
    //this->check_type(f_i, this->last_function_type, expevalident,
    //        this->last_type, stmret->line_number);

}

void ASTChecker::visitStmVRet(StmVRet* stmvret)
{
    this->last_line_number = stmvret->line_number;
    if (!(check_is<Void *>(this->last_function_type)))
    {
        std::string msg = "function `";
        msg += ident_to_string(this->last_function_ident);
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

void ASTChecker::visitStmForeach(StmForeach *stmforeach)
{
    /* latte++ */

    stmforeach->type_->accept(this);
    visitIdent(stmforeach->ident_);
    stmforeach->liststructuredident_->accept(this);
    stmforeach->stmt_->accept(this);

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
    //this->check_type(stminit->ident_, this->last_declaration_type, evalident, this->last_type, stminit->line_number);
    //TODO: check types
    this->add_variable(this->last_declaration_type, stminit->ident_, stminit->line_number);
}

void ASTChecker::visitStmInitArray(StmInitArray *stminitarray)
{
    /* Latte++ */

    visitIdent(stminitarray->ident_);
    stminitarray->type_->accept(this);
    stminitarray->expr_->accept(this);

}

void ASTChecker::visitStmInitObj(StmInitObj *stminitobj)
{
    /* Latte++ */

    visitIdent(stminitobj->ident_);
    stminitobj->type_->accept(this);

}

void ASTChecker::visitSingleIdent(SingleIdent *singleident)
{
    /* Latte++ */

    visitIdent(singleident->ident_);

}

void ASTChecker::visitTableVal(TableVal *tableval)
{
    /* Latte++ */

    visitIdent(tableval->ident_);
    tableval->listarrayindex_->accept(this);

}

void ASTChecker::visitSelfIdent(SelfIdent *selfident)
{
  /* Code For SelfIdent Goes Here */


}

void ASTChecker::visitExprIndex(ExprIndex *exprindex)
{
    /* Latte++ */

    exprindex->expr_->accept(this);

}

void ASTChecker::visitClass(Class *_class)
{
    /* Latte++ */

    visitIdent(_class->ident_);

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

void ASTChecker::visitTType(TType *ttype)
{
  /* Latte++ */

    ttype->type_->accept(this);

}

void ASTChecker::visitEVar(EVar* evar)
{
//    this->last_line_number = evar->line_number;
//    evar->liststructuredident_->accept(this);
//    Environment::VarInfoPtr var_info =
//            this->env.get_variable(evar->liststructuredident_);
//    if (!var_info) {
//        std::string msg = "variable `";
//        msg += ident_to_string(evar->liststructuredident_);
//        msg += "` used before declaration.";
//        this->error_handler.error(evar->line_number, msg);
//    }
//    else
//    {
//        this->last_type = var_info->type;
//    }
}

void ASTChecker::visitELitInt(ELitInt* elitint)
{
    this->last_line_number = elitint->line_number;
    visitInteger(elitint->integer_);
    this->last_type = &(this->literal_int);
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

void ASTChecker::visitELitNull(ELitNull *elitnull)
{
  /* Code For ELitNull Goes Here */


}

void ASTChecker::visitEApp(EApp* eapp)
{
//    this->last_line_number = eapp->line_number;
//    eapp->liststructuredident_->accept(this);
//    Environment::FunInfoPtr fun_ptr =
//            this->env.get_function(eapp->liststructuredident_);
//    // Check if function exists.
//    if (!fun_ptr)
//    {
//        std::string msg = "function `";
//        msg += ident_to_string(eapp->liststructuredident_);
//        msg += "` does not exist.";
//        this->error_handler.error(eapp->line_number, msg);
//        return;
//    }
//
//    // Check arguments.
//    this->last_arguments_iterator = fun_ptr->arguments.begin();
//    this->last_arguments_end = fun_ptr->arguments.end();
//    eapp->listexpr_->accept(this);
//    if ((this->last_arguments_iterator != this->last_arguments_end) || this->error_flag)
//    {
//        std::string msg = "function `";
//        msg += ident_to_string(eapp->liststructuredident_);
//        msg += "` need ";
//        std::ostringstream ss;
//        ss << fun_ptr->arguments.size();
//        msg += ss.str();
//        msg += " arguments: ";
//        msg += this->args_pretty_print(fun_ptr->arguments.begin(), fun_ptr->arguments.end());
//        this->error_handler.error(eapp->line_number, msg);
//        this->error_flag = false;
//    }
//    // Args checked.

//    this->last_type = fun_ptr->ret_type;
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
    if (!(check_is<Int *>(this->last_type)))
    {
        std::string msg = "negation should be only applied to numbers not [";
        msg += type_pretty_print(this->last_type);
        msg += "].";
        this->error_handler.error(neg->line_number, msg);
    }
    this->last_type = &(this->literal_int);
}

void ASTChecker::visitNot(Not* not_field)
{
    this->last_line_number = not_field->line_number;
    not_field->expr_->accept(this);
    if (!(check_is<Bool *>(this->last_type)))
    {
        std::string msg = "logical negation should be only applied to boolieans not [";
        msg += type_pretty_print(this->last_type);
        msg += "].";
        this->error_handler.error(not_field->line_number, msg);
    }
    this->last_type = &(this->literal_bool);
}

void ASTChecker::visitEDynamicCast(EDynamicCast *edynamiccast)
{
    /* Latte++ */

    visitIdent(edynamiccast->ident_);
    edynamiccast->expr_->accept(this);

}

void ASTChecker::visitEMul(EMul* emul)
{
    this->last_line_number = emul->line_number;
    this->last_type = 0;
    emul->expr_1->accept(this);
    Type* type1 = this->last_type;
    Ident left("left side of * / %");
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);
    Ident right("right side of * / %");
    Type* type2 = this->last_type;
    //this->check_type(left, type1, right, type2, emul->line_number);
    //TODO: type check
    if ((!(check_is<Int *>(type1))) || (!(check_is<Int *>(type2))))
    {
        std::string msg = "* / % should be only applied to numbers not [";
        msg += type_pretty_print(type1);
        msg += ", ";
        msg += type_pretty_print(type2);
        msg += "].";
        this->error_handler.error(emul->line_number, msg);
    }
    this->last_type = &(this->literal_int);
}

void ASTChecker::visitEAdd(EAdd* eadd)
{
    this->last_line_number = eadd->line_number;
    this->last_type = 0;
    eadd->expr_1->accept(this);
    Type* type1 = this->last_type;
    Ident left("left side of + -");
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);
    Ident right("right side of + -");
    Type* type2 = this->last_type;
    //this->check_type(left, type1, right, type2, eadd->line_number);
    //TODO: type check
    if (((!(check_is<Int *>(type1))) || (!(check_is<Int *>(type2))))
        && (! ( check_is<Str *>(type1) && check_is<Str *>(type2) && check_is<Plus *>(eadd->addop_)) )
        )
    {
        std::string msg = "+ - should be only applied to numbers (or + and string) not [";
        msg += type_pretty_print(type1);
        msg += ", ";
        msg += type_pretty_print(type2);
        msg += "].";
        this->error_handler.error(eadd->line_number, msg);
    }
    this->last_type = &(this->literal_int);
    if (check_is<Str *>(type1))
        this->last_type = &(this->literal_string);
}

void ASTChecker::visitERel(ERel* erel)
{
    this->last_line_number = erel->line_number;
    this->last_type = 0;
    erel->expr_1->accept(this);
    Type* type1 = this->last_type;
    Ident left("left side of relation");
    erel->relop_->accept(this);
    erel->expr_2->accept(this);
    Ident right("right side of relation");
    Type* type2 = this->last_type;
    //this->check_type(left, type1, right, type2, erel->line_number);
    //TOOD: type check
    if (    ((!(check_is<Int *>(type1))) || (!(check_is<Int *>(type2)))) &&
            ((!(check_is<Bool *>(type1))) || (!(check_is<Bool *>(type2))))
            )
    {
        std::string msg = "relation should be only applied to booleans or numbers not [";
        msg += type_pretty_print(type1);
        msg += ", ";
        msg += type_pretty_print(type2);
        msg += "].";
        this->error_handler.error(erel->line_number, msg);
    }
    this->last_type = &(this->literal_bool);
}

void ASTChecker::visitEAnd(EAnd* eand)
{
    this->last_line_number = eand->line_number;
    this->last_type = 0;
    eand->expr_1->accept(this);
    Type* type1 = this->last_type;
    Ident left("left side of &&");
    eand->expr_2->accept(this);
    Ident right("right side of &&");
    Type* type2 = this->last_type;
    // this->check_type(left, type1, right, type2, eand->line_number);
    // TODO: type check
    if ((!(check_is<Bool *>(type1))) || (!(check_is<Bool *>(type2))))
    {
        std::string msg = "&& should be only applied to booleans not [";
        msg += type_pretty_print(type1);
        msg += ", ";
        msg += type_pretty_print(type2);
        msg += "].";
        this->error_handler.error(eand->line_number, msg);
    }
    this->last_type = &(this->literal_bool);
}

void ASTChecker::visitEOr(EOr* eor)
{
    this->last_line_number = eor->line_number;
    this->last_type = 0;
    eor->expr_1->accept(this);
    Type* type1 = this->last_type;
    Ident left("left side of ||");
    eor->expr_2->accept(this);
    Ident right("right side of ||");
    Type* type2 = this->last_type;
    //this->check_type(left, type1, right, type2, eor->line_number);
    // TODO: type check
    if ((!(check_is<Bool *>(type1))) || (!(check_is<Bool *>(type2))))
    {
        std::string msg = "|| should be only applied to booleans and numbers not [";
        msg += type_pretty_print(type1);
        msg += ", ";
        msg += type_pretty_print(type2);
        msg += "].";
        this->error_handler.error(eor->line_number, msg);
    }
    this->last_type = &(this->literal_bool);
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

void ASTChecker::visitListClsDef(ListClsDef* listclsdef)
{
    for (ListClsDef::iterator i = listclsdef->begin() ; i != listclsdef->end() ; ++i)
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

void ASTChecker::visitListArrayIndex(ListArrayIndex* listarrayindex)
{
  for (ListArrayIndex::iterator i = listarrayindex->begin() ; i != listarrayindex->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void ASTChecker::visitListStructuredIdent(ListStructuredIdent* liststructuredident)
{
    /* Latte++ */
    for (ListStructuredIdent::iterator i = liststructuredident->begin() ; i != liststructuredident->end() ; ++i)
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
    fun_arg += ident_to_string(this->last_function_ident);
    fun_arg += " argument";

    int n = 0;
    std::vector<Environment::VarInfoPtr>::const_iterator arguments_iterator = this->last_arguments_iterator;
    std::vector<Environment::VarInfoPtr>::const_iterator arguments_end = this->last_arguments_end;

    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        n++;
        Ident arg_no("argument number ");
        arg_no += n;

        if (arguments_iterator == arguments_end)
        {
            this->error_flag = true;
            this->last_arguments_iterator = arguments_iterator;
            this->last_arguments_end = arguments_end;
            return;
        }

        (*i)->accept(this);

        //this->check_type(fun_arg, (*(arguments_iterator))->type, arg_no, this->last_type, this->last_line_number);
        // TODO: check type
        arguments_iterator++;
    }
    this->last_arguments_iterator = arguments_iterator;
    this->last_arguments_end = arguments_end;
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
