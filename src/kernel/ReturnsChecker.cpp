/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include "ReturnsChecker.h"
#include "ErrorHandler.h"

namespace frontend
{

ReturnsChecker::ReturnsChecker(ErrorHandler& error_handler, Environment& env)
    : error_handler(error_handler), env(env)
{
    // TODO Auto-generated constructor stub

}

void ReturnsChecker::check(Visitable* v)
{
    std::cout << &(this->env) << std::endl;
    v->accept(this);
}

void ReturnsChecker::visitProg(Prog* t) { } //abstract class
void ReturnsChecker::visitTopDef(TopDef* t) {} //abstract class
void ReturnsChecker::visitArg(Arg* t) {} //abstract class
void ReturnsChecker::visitBlk(Blk* t) {} //abstract class
void ReturnsChecker::visitStmt(Stmt* t) {} //abstract class
void ReturnsChecker::visitItem(Item* t) {} //abstract class
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
    /* Code For FnDef Goes Here*/
    fndef->type_->accept(this);
    visitIdent(fndef->ident_);
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);

}

void ReturnsChecker::visitArgument(Argument* argument)
{
    /* Code For Argument Goes Here*/
    argument->type_->accept(this);
    visitIdent(argument->ident_);

}

void ReturnsChecker::visitStmBlock(StmBlock* stmblock)
{
    /* Code For StmBlock Goes Here*/

    stmblock->liststmt_->accept(this);

}

void ReturnsChecker::visitStmEmpty(StmEmpty* stmempty)
{
    /* Code For StmEmpty Goes Here*/


}

void ReturnsChecker::visitStmBStmt(StmBStmt* stmbstmt)
{
    /* Code For StmBStmt Goes Here*/

    stmbstmt->blk_->accept(this);

}

void ReturnsChecker::visitStmDecl(StmDecl* stmdecl)
{
    /* Code For StmDecl Goes Here*/

    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);

}

void ReturnsChecker::visitStmAss(StmAss* stmass)
{
    /* Code For StmAss Goes Here*/

    visitIdent(stmass->ident_);
    stmass->expr_->accept(this);

}

void ReturnsChecker::visitStmIncr(StmIncr* stmincr)
{
    /* Code For StmIncr Goes Here*/

    visitIdent(stmincr->ident_);

}

void ReturnsChecker::visitStmDecr(StmDecr* stmdecr)
{
    /* Code For StmDecr Goes Here*/

    visitIdent(stmdecr->ident_);

}

void ReturnsChecker::visitStmRet(StmRet* stmret)
{
    /* Code For StmRet Goes Here*/

    stmret->expr_->accept(this);

}

void ReturnsChecker::visitStmVRet(StmVRet* stmvret)
{
    /* Code For StmVRet Goes Here*/


}

void ReturnsChecker::visitStmCond(StmCond* stmcond)
{
    /* Code For StmCond Goes Here*/

    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);

}

void ReturnsChecker::visitStmCondElse(StmCondElse* stmcondelse)
{
    /* Code For StmCondElse Goes Here*/

    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);

}

void ReturnsChecker::visitStmWhile(StmWhile* stmwhile)
{
    /* Code For StmWhile Goes Here*/

    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);

}

void ReturnsChecker::visitStmSExp(StmSExp* stmsexp)
{
    /* Code For StmSExp Goes Here*/

    stmsexp->expr_->accept(this);

}

void ReturnsChecker::visitStmNoInit(StmNoInit* stmnoinit)
{
    /* Code For StmNoInit Goes Here*/

    visitIdent(stmnoinit->ident_);

}

void ReturnsChecker::visitStmInit(StmInit* stminit)
{
    /* Code For StmInit Goes Here*/

    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);

}

void ReturnsChecker::visitInt(Int* integer)
{
    /* Code For Int Goes Here*/


}

void ReturnsChecker::visitStr(Str* str)
{
    /* Code For Str Goes Here*/


}

void ReturnsChecker::visitBool(Bool* boolean)
{
    /* Code For Bool Goes Here*/


}

void ReturnsChecker::visitVoid(Void* void_field)
{
    /* Code For Void Goes Here*/


}

void ReturnsChecker::visitFun(Fun* fun)
{
    /* Code For Fun Goes Here*/

    fun->type_->accept(this);
    fun->listtype_->accept(this);

}

void ReturnsChecker::visitEVar(EVar* evar)
{
    /* Code For EVar Goes Here*/

    visitIdent(evar->ident_);

}

void ReturnsChecker::visitELitInt(ELitInt* elitint)
{
    /* Code For ELitInt Goes Here*/

    visitInteger(elitint->integer_);

}

void ReturnsChecker::visitELitTrue(ELitTrue* elittrue)
{
    /* Code For ELitTrue Goes Here*/


}

void ReturnsChecker::visitELitFalse(ELitFalse* elitfalse)
{
    /* Code For ELitFalse Goes Here*/


}

void ReturnsChecker::visitEApp(EApp* eapp)
{
    /* Code For EApp Goes Here*/

    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);

}

void ReturnsChecker::visitEString(EString* estring)
{
    /* Code For EString Goes Here*/

    visitString(estring->string_);

}

void ReturnsChecker::visitNeg(Neg* neg)
{
    /* Code For Neg Goes Here*/

    neg->expr_->accept(this);

}

void ReturnsChecker::visitNot(Not* not_field)
{
    /* Code For Not Goes Here*/

    not_field->expr_->accept(this);

}

void ReturnsChecker::visitEMul(EMul* emul)
{
    /* Code For EMul Goes Here*/

    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);

}

void ReturnsChecker::visitEAdd(EAdd* eadd)
{
    /* Code For EAdd Goes Here*/

    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);

}

void ReturnsChecker::visitERel(ERel* erel)
{
    /* Code For ERel Goes Here*/

    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);

}

void ReturnsChecker::visitEAnd(EAnd* eand)
{
    /* Code For EAnd Goes Here*/

    eand->expr_1->accept(this);
    eand->expr_2->accept(this);

}

void ReturnsChecker::visitEOr(EOr* eor)
{
    /* Code For EOr Goes Here*/

    eor->expr_1->accept(this);
    eor->expr_2->accept(this);

}

void ReturnsChecker::visitPlus(Plus* plus)
{
    /* Code For Plus Goes Here*/


}

void ReturnsChecker::visitMinus(Minus* minus)
{
    /* Code For Minus Goes Here*/


}

void ReturnsChecker::visitTimes(Times* times)
{
    /* Code For Times Goes Here*/


}

void ReturnsChecker::visitDiv(Div* div)
{
    /* Code For Div Goes Here*/


}

void ReturnsChecker::visitMod(Mod* mod)
{
    /* Code For Mod Goes Here*/


}

void ReturnsChecker::visitLTH(LTH* lth)
{
    /* Code For LTH Goes Here*/


}

void ReturnsChecker::visitLE(LE* le)
{
    /* Code For LE Goes Here*/


}

void ReturnsChecker::visitGTH(GTH* gth)
{
    /* Code For GTH Goes Here*/


}

void ReturnsChecker::visitGE(GE* ge)
{
    /* Code For GE Goes Here*/


}

void ReturnsChecker::visitEQU(EQU* equ)
{
    /* Code For EQU Goes Here*/


}

void ReturnsChecker::visitNE(NE* ne)
{
    /* Code For NE Goes Here*/


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

void ReturnsChecker::visitListType(ListType* listtype)
{
    for (ListType::iterator i = listtype->begin() ; i != listtype->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ReturnsChecker::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void ReturnsChecker::visitInteger(Integer x)
{
    /* Code for Integer Goes Here*/
}

void ReturnsChecker::visitChar(Char x)
{
    /* Code for Char Goes Here*/
}

void ReturnsChecker::visitDouble(Double x)
{
    /* Code for Double Goes Here*/
}

void ReturnsChecker::visitString(String x)
{
    /* Code for String Goes Here*/
}

void ReturnsChecker::visitIdent(Ident x)
{
}


} /* namespace frontend */
