/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <string>
#include <iostream>
#include "ASTChecker.h"
#include "ErrorHandler.h"

namespace frontend
{

ASTChecker::ASTChecker(ErrorHandler& error_handler) : error_handler(error_handler)
{
}

void ASTChecker::check(Visitable* v)
{
    v->accept(this);
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
    this->error_handler.warning(program->line_number, "hello!");
    program->listtopdef_->accept(this);
}

void ASTChecker::visitFnDef(FnDef* fndef)
{
    /* Code For FnDef Goes Here*/
    fndef->type_->accept(this);
    visitIdent(fndef->ident_);
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);

}

void ASTChecker::visitArgument(Argument* argument)
{
    ErrorHandler::error_type error = std::make_pair(true, std::string("Twoja stara"));
    this->error_handler.handle(argument->line_number, error);
    /* Code For Argument Goes Here*/
    argument->type_->accept(this);
    visitIdent(argument->ident_);

}

void ASTChecker::visitStmBlock(StmBlock* stmblock)
{
    /* Code For StmBlock Goes Here*/

    stmblock->liststmt_->accept(this);

}

void ASTChecker::visitStmEmpty(StmEmpty* stmempty)
{
    /* Code For StmEmpty Goes Here*/


}

void ASTChecker::visitStmBStmt(StmBStmt* stmbstmt)
{
    /* Code For StmBStmt Goes Here*/

    stmbstmt->blk_->accept(this);

}

void ASTChecker::visitStmDecl(StmDecl* stmdecl)
{
    /* Code For StmDecl Goes Here*/

    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);

}

void ASTChecker::visitStmAss(StmAss* stmass)
{
    /* Code For StmAss Goes Here*/

    visitIdent(stmass->ident_);
    stmass->expr_->accept(this);

}

void ASTChecker::visitStmIncr(StmIncr* stmincr)
{
    /* Code For StmIncr Goes Here*/

    visitIdent(stmincr->ident_);

}

void ASTChecker::visitStmDecr(StmDecr* stmdecr)
{
    /* Code For StmDecr Goes Here*/

    visitIdent(stmdecr->ident_);

}

void ASTChecker::visitStmRet(StmRet* stmret)
{
    /* Code For StmRet Goes Here*/

    stmret->expr_->accept(this);

}

void ASTChecker::visitStmVRet(StmVRet* stmvret)
{
    /* Code For StmVRet Goes Here*/


}

void ASTChecker::visitStmCond(StmCond* stmcond)
{
    /* Code For StmCond Goes Here*/

    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);

}

void ASTChecker::visitStmCondElse(StmCondElse* stmcondelse)
{
    /* Code For StmCondElse Goes Here*/

    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);

}

void ASTChecker::visitStmWhile(StmWhile* stmwhile)
{
    /* Code For StmWhile Goes Here*/

    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);

}

void ASTChecker::visitStmSExp(StmSExp* stmsexp)
{
    /* Code For StmSExp Goes Here*/

    stmsexp->expr_->accept(this);

}

void ASTChecker::visitStmNoInit(StmNoInit* stmnoinit)
{
    /* Code For StmNoInit Goes Here*/

    visitIdent(stmnoinit->ident_);

}

void ASTChecker::visitStmInit(StmInit* stminit)
{
    /* Code For StmInit Goes Here*/

    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);

}

void ASTChecker::visitInt(Int* integer)
{
    /* Code For Int Goes Here*/


}

void ASTChecker::visitStr(Str* str)
{
    /* Code For Str Goes Here*/


}

void ASTChecker::visitBool(Bool* boolean)
{
    /* Code For Bool Goes Here*/


}

void ASTChecker::visitVoid(Void* void_field)
{
    /* Code For Void Goes Here*/


}

void ASTChecker::visitFun(Fun* fun)
{
    /* Code For Fun Goes Here*/

    fun->type_->accept(this);
    fun->listtype_->accept(this);

}

void ASTChecker::visitEVar(EVar* evar)
{
    /* Code For EVar Goes Here*/

    visitIdent(evar->ident_);

}

void ASTChecker::visitELitInt(ELitInt* elitint)
{
    /* Code For ELitInt Goes Here*/

    visitInteger(elitint->integer_);

}

void ASTChecker::visitELitTrue(ELitTrue* elittrue)
{
    /* Code For ELitTrue Goes Here*/


}

void ASTChecker::visitELitFalse(ELitFalse* elitfalse)
{
    /* Code For ELitFalse Goes Here*/


}

void ASTChecker::visitEApp(EApp* eapp)
{
    /* Code For EApp Goes Here*/

    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);

}

void ASTChecker::visitEString(EString* estring)
{
    /* Code For EString Goes Here*/

    visitString(estring->string_);

}

void ASTChecker::visitNeg(Neg* neg)
{
    /* Code For Neg Goes Here*/

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
