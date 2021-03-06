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
    : error_handler(error_handler), env(env), r_flag(false), expression_optimization(0),
      const_int_value(0), e1(0), e2(0), const_bool_value(0), remove(false), to_the_end(false)
{
}

void ReturnsChecker::check(Visitable* v)
{
    this->expression_optimization = 0;
    this->remove = false;
    this->to_the_end = false;
    this->const_int_value = 0;
    this->e1 = 0;
    this->e2 = 0;
    this->const_bool_value = false;

    v->accept(this);
}

void ReturnsChecker::visitProg(Prog* t) {} //abstract class
void ReturnsChecker::visitTopDef(TopDef* t) {} //abstract class
void ReturnsChecker::visitArg(Arg* t) {} //abstract class
void ReturnsChecker::visitClsDef(ClsDef* t) {} //abstract class
void ReturnsChecker::visitBlk(Blk* t) {} //abstract class
void ReturnsChecker::visitStmt(Stmt* t) {} //abstract class
void ReturnsChecker::visitItem(Item* t) {} //abstract class
void ReturnsChecker::visitStructuredIdent(StructuredIdent* t) {} //abstract class
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
        msg += "function : `";
        msg += fndef->ident_;
        msg += "` should returns [";
        msg += type_pretty_print(f->ret_type);
        msg += "].";
        this->error_handler.error(fndef->type_->line_number, msg);
    }
}

void ReturnsChecker::visitClsDefNoInher(ClsDefNoInher *clsdefnoinher)
{
    visitIdent(clsdefnoinher->ident_);
    this->last_class_ident = clsdefnoinher->ident_;
    clsdefnoinher->listclsdef_->accept(this);
}

void ReturnsChecker::visitClsDefInher(ClsDefInher *clsdefinher)
{
    visitIdent(clsdefinher->ident_1);
    this->last_class_ident = clsdefinher->ident_1;
    visitIdent(clsdefinher->ident_2);
    clsdefinher->listclsdef_->accept(this);

}

void ReturnsChecker::visitArgument(Argument* argument)
{
    argument->type_->accept(this);
    visitIdent(argument->ident_);
}

void ReturnsChecker::visitMethodDef(MethodDef *methoddef)
{
    methoddef->type_->accept(this);
    visitIdent(methoddef->ident_);
    methoddef->listarg_->accept(this);



    this->r_flag = false;
    methoddef->blk_->accept(this);
    Environment::FunInfoPtr f = this->env.get_method(methoddef->ident_, this->last_class_ident);
    if ((!this->r_flag) && (!(check_is<Void *>(f->ret_type))))
    {
        std::string msg;
        msg += "function : `";
        msg += methoddef->ident_;
        msg += "` should returns [";
        msg += type_pretty_print(f->ret_type);
        msg += "].";
        this->error_handler.error(methoddef->type_->line_number, msg);
    }

}

void ReturnsChecker::visitFieldDef(FieldDef *fielddef)
{
    fielddef->type_->accept(this);
    visitIdent(fielddef->ident_);
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
    stmass->liststructuredident_->accept(this);
    this->expression_optimization = 0;
    stmass->expr_->accept(this);
    this->expression_optimization = 0;
}

void ReturnsChecker::visitStmAssArr(StmAssArr *stmassarr)
{
    stmassarr->liststructuredident_->accept(this);
    stmassarr->type_->accept(this);
    stmassarr->expr_->accept(this);

}

void ReturnsChecker::visitStmAssObj(StmAssObj *stmassobj)
{
    stmassobj->liststructuredident_->accept(this);
    stmassobj->type_->accept(this);

}

void ReturnsChecker::visitStmIncr(StmIncr *stmincr)
{
    stmincr->liststructuredident_->accept(this);
}

void ReturnsChecker::visitStmDecr(StmDecr *stmdecr)
{
    stmdecr->liststructuredident_->accept(this);
}

void ReturnsChecker::visitStmRet(StmRet* stmret)
{
    this->expression_optimization = 0;
    stmret->expr_->accept(this);
    if (this->expression_optimization != 0)
    {
        this->to_the_end = true;
        this->remove = true;
    }
    this->r_flag = true;
    this->expression_optimization = 0;
}

void ReturnsChecker::visitStmVRet(StmVRet* stmvret)
{
    this->r_flag = true;
    this->to_the_end = true;
    this->remove = true;
}

void ReturnsChecker::visitStmCond(StmCond* stmcond)
{
    this->expression_optimization = 0;
    stmcond->expr_->accept(this);
    if (this->expression_optimization == 1)
    {
        stmcond->stmt_->accept(this);
    }
    this->expression_optimization = 0;
}

void ReturnsChecker::visitStmCondElse(StmCondElse* stmcondelse)
{
    this->expression_optimization = 0;
    stmcondelse->expr_->accept(this);

    if (this->expression_optimization == 1)
        stmcondelse->stmt_1->accept(this);
    else if (this->expression_optimization == -1)
        stmcondelse->stmt_2->accept(this);
    else
    {
        bool r = true;
        stmcondelse->stmt_1->accept(this);
        r = r && this->r_flag;
        stmcondelse->stmt_2->accept(this);
        r = r && this->r_flag;
        this->r_flag = r;
    }

    this->expression_optimization = 0;
}

void ReturnsChecker::visitStmWhile(StmWhile* stmwhile)
{
    this->expression_optimization = 0;
    stmwhile->expr_->accept(this);
    if (this->expression_optimization == 1)
        this->r_flag = true; // infinity loop
    else if (this->expression_optimization == -1)
    {
        ;
        // do not accept, and do not set flag
    }
    else
    {
        stmwhile->stmt_->accept(this);
    }
    this->expression_optimization = 0;
}

void ReturnsChecker::visitStmForeach(StmForeach *stmforeach)
{

    stmforeach->type_->accept(this);
    visitIdent(stmforeach->ident_);
    stmforeach->liststructuredident_->accept(this);
    stmforeach->stmt_->accept(this);
    this->expression_optimization = 0;

}

void ReturnsChecker::visitStmSExp(StmSExp* stmsexp)
{
    this->expression_optimization = 0;
    stmsexp->expr_->accept(this);
    this->expression_optimization = 0;
}

void ReturnsChecker::visitStmNoInit(StmNoInit* stmnoinit)
{
    visitIdent(stmnoinit->ident_);
}

void ReturnsChecker::visitStmInit(StmInit* stminit)
{
    visitIdent(stminit->ident_);
    this->expression_optimization = 0;
    stminit->expr_->accept(this);
    if (this->expression_optimization != 0)
    {
        ;
    }
}

void ReturnsChecker::visitStmInitArray(StmInitArray *stminitarray)
{
    visitIdent(stminitarray->ident_);
    stminitarray->type_->accept(this);
    this->expression_optimization = 0;
    stminitarray->expr_->accept(this);

}

void ReturnsChecker::visitStmInitObj(StmInitObj *stminitobj)
{
    visitIdent(stminitobj->ident_);
    stminitobj->type_->accept(this);

}

void ReturnsChecker::visitSingleIdent(SingleIdent *singleident)
{
    visitIdent(singleident->ident_);

}

void ReturnsChecker::visitArrayIdent(ArrayIdent *tableval)
{
    visitIdent(tableval->ident_);
    tableval->expr_->accept(this);
}

void ReturnsChecker::visitSelfIdent(SelfIdent *selfident)
{
}

void ReturnsChecker::visitClass(Class* _class)
{
    visitIdent(_class->ident_);
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

void ReturnsChecker::visitTType(TType *ttype)
{
    ttype->type_->accept(this);
}

void ReturnsChecker::visitEVar(EVar *evar)
{
    evar->liststructuredident_->accept(this);
    this->expression_optimization = 0;

}

void ReturnsChecker::visitELitInt(ELitInt* elitint)
{
    visitInteger(elitint->integer_);
    this->expression_optimization = 2;
    this->const_int_value = elitint->integer_;
}

void ReturnsChecker::visitELitTrue(ELitTrue* elittrue)
{
    this->expression_optimization = 1;
    this->const_bool_value = true;
}

void ReturnsChecker::visitELitFalse(ELitFalse* elitfalse)
{
    this->expression_optimization = -1;
    this->const_bool_value = false;
}

void ReturnsChecker::visitELitNull(ELitNull *elitnull)
{
}


void ReturnsChecker::visitEApp(EApp *eapp)
{
    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);

}

void ReturnsChecker::visitEMethodApp(EMethodApp *emethodapp)
{
    emethodapp->liststructuredident_->accept(this);
    emethodapp->listexpr_->accept(this);
}

void ReturnsChecker::visitEString(EString* estring)
{
    visitString(estring->string_);
    this->expression_optimization = 2;
    this->const_str_value = estring->string_;
}

void ReturnsChecker::visitNeg(Neg* neg)
{
    neg->expr_->accept(this);
    if (this->expression_optimization == 2)
        this->const_int_value = -this->const_int_value;
}

void ReturnsChecker::visitNot(Not* not_field)
{
    this->expression_optimization = 0;
    not_field->expr_->accept(this);
    this->expression_optimization = -(this->expression_optimization);
}

void ReturnsChecker::visitEDynamicCast(EDynamicCast *edynamiccast)
{
    visitIdent(edynamiccast->ident_);
    edynamiccast->expr_->accept(this);

}

void ReturnsChecker::visitEMul(EMul* emul)
{
    this->expression_optimization = 0;
    int e1 = 0, e2 = 0;
    bool optimize = true;
    emul->expr_1->accept(this);
    if (this->expression_optimization == 2)
    {
        e1 = this->const_int_value;
    }
    else
        optimize = false;

    emul->mulop_->accept(this);
    emul->expr_2->accept(this);
    if (this->expression_optimization == 2)
    {
        e2 = this->const_int_value;
    }
    else
        optimize = false;

    if (optimize)
    {
        if (check_is<Times*>(emul)){
            this->const_int_value = e1 * e2;
        } else if (check_is<Div*>(emul)){
            this->const_int_value = e1 / e2;
        } else if (check_is<Mod*>(emul)){
            this->const_int_value = e1 % e2;
        } else {
            this->expression_optimization = 0;
        }
    } else
        this->expression_optimization = 0;
}

void ReturnsChecker::visitEAdd(EAdd* eadd)
{
    this->expression_optimization = 0;
    int e1 = 0, e2 = 0;
    bool optimize = true;
    eadd->expr_1->accept(this);
    if (this->expression_optimization == 2)
    {
        e1 = this->const_int_value;
    }
    else
        optimize = false;
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);
    if (this->expression_optimization == 2)
    {
        e2 = this->const_int_value;
    }
    else
        optimize = false;

    // TOOD: WARNING! literal strings!
    if (optimize)
    {
        if (check_is<Plus*>(eadd)){
            this->const_int_value = e1 + e2;
        } else if (check_is<Minus*>(eadd)){
            this->const_int_value = e1 - e2;
        } else {
            this->expression_optimization = 0;
        }
    } else
        this->expression_optimization = 0;
}

void ReturnsChecker::visitERel(ERel* erel)
{
    this->expression_optimization = 0;
    bool optimize = true;
    erel->expr_1->accept(this);
    if (this->expression_optimization != 0)
    {
        this->e1 = this->const_int_value;
        if (this->expression_optimization != 2)
            this->e1 = this->expression_optimization;
    }
    else
        optimize = false;
    erel->expr_2->accept(this);
    if (this->expression_optimization != 0)
    {
        this->e2 = this->const_int_value;
        if (this->expression_optimization != 2)
            this->e2 = this->expression_optimization;
    }
    else
        optimize = false;

    if (!optimize){
        this->expression_optimization = 0;
    }
    erel->relop_->accept(this);
}

void ReturnsChecker::visitEAnd(EAnd* eand)
{
    this->expression_optimization = 0;
    eand->expr_1->accept(this);
    short eo1 = this->expression_optimization;
    eand->expr_2->accept(this);
    short eo2 = this->expression_optimization;

    if ((eo1 == 1) && (eo2 == 1))
    {
        this->expression_optimization = 1;
    }
    else if ((eo1 == -1) || (eo2 == -1))
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
    if (this->expression_optimization != 0){
        if (this->e1 < this->e2){
            this->expression_optimization = 1;
        } else {
            this->expression_optimization = -1;
        }
    }
}

void ReturnsChecker::visitLE(LE* le)
{
    if (this->expression_optimization != 0){
        if (this->e1 <= this->e2){
            this->expression_optimization = 1;
        } else {
            this->expression_optimization = -1;
        }
    }
}

void ReturnsChecker::visitGTH(GTH* gth)
{
    if (this->expression_optimization != 0){
        if (this->e1 > this->e2){
            this->expression_optimization = 1;
        } else {
            this->expression_optimization = -1;
        }
    }
}

void ReturnsChecker::visitGE(GE* ge)
{
    if (this->expression_optimization != 0){
        if (this->e1 >= this->e2){
            this->expression_optimization = 1;
        } else {
            this->expression_optimization = -1;
        }
    }
}

void ReturnsChecker::visitEQU(EQU* equ)
{
    if (this->expression_optimization != 0){
        if (this->e1 == this->e2){
            this->expression_optimization = 1;
        } else {
            this->expression_optimization = -1;
        }
    }
}

void ReturnsChecker::visitNE(NE* ne)
{
    if (this->expression_optimization != 0){
        if (this->e1 != this->e2){
            this->expression_optimization = 1;
        } else {
            this->expression_optimization = -1;
        }
    }
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

void ReturnsChecker::visitListClsDef(ListClsDef* listclsdef)
{
    /* Latte++ */
    for (ListClsDef::iterator i = listclsdef->begin() ; i != listclsdef->end() ; ++i)
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

void ReturnsChecker::visitListStructuredIdent(ListStructuredIdent* liststructuredident)
{
    /* Latte++ */
    for (ListStructuredIdent::iterator i = liststructuredident->begin() ; i != liststructuredident->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ReturnsChecker::visitListExpr(ListExpr* listexpr)
{
    /* Latte++ */
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void ReturnsChecker::visitInteger(Integer x)
{
}

void ReturnsChecker::visitChar(Char x)
{
}

void ReturnsChecker::visitDouble(Double x)
{
}

void ReturnsChecker::visitString(String x)
{
}

void ReturnsChecker::visitIdent(Ident x)
{
}


} /* namespace frontend */
