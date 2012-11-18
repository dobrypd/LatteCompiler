/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "TreeOptimizer.h"
#include "ErrorHandler.h"

namespace frontend
{

TreeOptimizer::TreeOptimizer()
{
}

void TreeOptimizer::optimize(Visitable* v)
{
    v->accept(this);
}

void TreeOptimizer::visitProg(Prog* t) { } //abstract class
void TreeOptimizer::visitTopDef(TopDef* t) {} //abstract class
void TreeOptimizer::visitArg(Arg* t) {} //abstract class
void TreeOptimizer::visitBlk(Blk* t) {} //abstract class
void TreeOptimizer::visitStmt(Stmt* t) {} //abstract class
void TreeOptimizer::visitItem(Item* t) {} //abstract class
void TreeOptimizer::visitType(Type* t) {} //abstract class
void TreeOptimizer::visitExpr(Expr* t) {} //abstract class
void TreeOptimizer::visitAddOp(AddOp* t) {} //abstract class
void TreeOptimizer::visitMulOp(MulOp* t) {} //abstract class
void TreeOptimizer::visitRelOp(RelOp* t) {} //abstract class

void TreeOptimizer::visitProgram(Program* program)
{
    program->listtopdef_->accept(this);
}

void TreeOptimizer::visitFnDef(FnDef* fndef)
{
    fndef->type_->accept(this);
    visitIdent(fndef->ident_);
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);

}

void TreeOptimizer::visitArgument(Argument* argument)
{
    /* Code For Argument Goes Here*/
    argument->type_->accept(this);
    visitIdent(argument->ident_);

}

void TreeOptimizer::visitStmBlock(StmBlock* stmblock)
{
    /* Code For StmBlock Goes Here*/

    stmblock->liststmt_->accept(this);

}

void TreeOptimizer::visitStmEmpty(StmEmpty* stmempty)
{
    /* Code For StmEmpty Goes Here*/


}

void TreeOptimizer::visitStmBStmt(StmBStmt* stmbstmt)
{
    /* Code For StmBStmt Goes Here*/

    stmbstmt->blk_->accept(this);

}

void TreeOptimizer::visitStmDecl(StmDecl* stmdecl)
{
    /* Code For StmDecl Goes Here*/

    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);

}

void TreeOptimizer::visitStmAss(StmAss* stmass)
{
    /* Code For StmAss Goes Here*/

    visitIdent(stmass->ident_);
    stmass->expr_->accept(this);

}

void TreeOptimizer::visitStmIncr(StmIncr* stmincr)
{
    /* Code For StmIncr Goes Here*/

    visitIdent(stmincr->ident_);

}

void TreeOptimizer::visitStmDecr(StmDecr* stmdecr)
{
    /* Code For StmDecr Goes Here*/

    visitIdent(stmdecr->ident_);

}

void TreeOptimizer::visitStmRet(StmRet* stmret)
{
    /* Code For StmRet Goes Here*/

    stmret->expr_->accept(this);

}

void TreeOptimizer::visitStmVRet(StmVRet* stmvret)
{
    /* Code For StmVRet Goes Here*/


}

void TreeOptimizer::visitStmCond(StmCond* stmcond)
{
    /* Code For StmCond Goes Here*/

    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);

}

void TreeOptimizer::visitStmCondElse(StmCondElse* stmcondelse)
{
    /* Code For StmCondElse Goes Here*/

    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);

}

void TreeOptimizer::visitStmWhile(StmWhile* stmwhile)
{
    /* Code For StmWhile Goes Here*/

    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);

}

void TreeOptimizer::visitStmSExp(StmSExp* stmsexp)
{
    /* Code For StmSExp Goes Here*/

    stmsexp->expr_->accept(this);

}

void TreeOptimizer::visitStmNoInit(StmNoInit* stmnoinit)
{
    /* Code For StmNoInit Goes Here*/

    visitIdent(stmnoinit->ident_);

}

void TreeOptimizer::visitStmInit(StmInit* stminit)
{
    /* Code For StmInit Goes Here*/

    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);

}

void TreeOptimizer::visitInt(Int* integer)
{
    /* Code For Int Goes Here*/


}

void TreeOptimizer::visitStr(Str* str)
{
    /* Code For Str Goes Here*/


}

void TreeOptimizer::visitBool(Bool* boolean)
{
    /* Code For Bool Goes Here*/


}

void TreeOptimizer::visitVoid(Void* void_field)
{
    /* Code For Void Goes Here*/


}

void TreeOptimizer::visitFun(Fun* fun)
{
    /* Code For Fun Goes Here*/

    fun->type_->accept(this);
    fun->listtype_->accept(this);

}

void TreeOptimizer::visitEVar(EVar* evar)
{
    /* Code For EVar Goes Here*/

    visitIdent(evar->ident_);

}

void TreeOptimizer::visitELitInt(ELitInt* elitint)
{
    /* Code For ELitInt Goes Here*/

    visitInteger(elitint->integer_);

}

void TreeOptimizer::visitELitTrue(ELitTrue* elittrue)
{
    /* Code For ELitTrue Goes Here*/


}

void TreeOptimizer::visitELitFalse(ELitFalse* elitfalse)
{
    /* Code For ELitFalse Goes Here*/


}

void TreeOptimizer::visitEApp(EApp* eapp)
{
    /* Code For EApp Goes Here*/

    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);

}

void TreeOptimizer::visitEString(EString* estring)
{
    /* Code For EString Goes Here*/

    visitString(estring->string_);

}

void TreeOptimizer::visitNeg(Neg* neg)
{
    /* Code For Neg Goes Here*/

    neg->expr_->accept(this);

}

void TreeOptimizer::visitNot(Not* not_field)
{
    /* Code For Not Goes Here*/

    not_field->expr_->accept(this);

}

void TreeOptimizer::visitEMul(EMul* emul)
{
    /* Code For EMul Goes Here*/

    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);

}

void TreeOptimizer::visitEAdd(EAdd* eadd)
{
    /* Code For EAdd Goes Here*/

    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);

}

void TreeOptimizer::visitERel(ERel* erel)
{
    /* Code For ERel Goes Here*/

    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);

}

void TreeOptimizer::visitEAnd(EAnd* eand)
{
    /* Code For EAnd Goes Here*/

    eand->expr_1->accept(this);
    eand->expr_2->accept(this);

}

void TreeOptimizer::visitEOr(EOr* eor)
{
    /* Code For EOr Goes Here*/

    eor->expr_1->accept(this);
    eor->expr_2->accept(this);

}

void TreeOptimizer::visitPlus(Plus* plus)
{
    /* Code For Plus Goes Here*/


}

void TreeOptimizer::visitMinus(Minus* minus)
{
    /* Code For Minus Goes Here*/


}

void TreeOptimizer::visitTimes(Times* times)
{
    /* Code For Times Goes Here*/


}

void TreeOptimizer::visitDiv(Div* div)
{
    /* Code For Div Goes Here*/


}

void TreeOptimizer::visitMod(Mod* mod)
{
    /* Code For Mod Goes Here*/


}

void TreeOptimizer::visitLTH(LTH* lth)
{
    /* Code For LTH Goes Here*/


}

void TreeOptimizer::visitLE(LE* le)
{
    /* Code For LE Goes Here*/


}

void TreeOptimizer::visitGTH(GTH* gth)
{
    /* Code For GTH Goes Here*/


}

void TreeOptimizer::visitGE(GE* ge)
{
    /* Code For GE Goes Here*/


}

void TreeOptimizer::visitEQU(EQU* equ)
{
    /* Code For EQU Goes Here*/


}

void TreeOptimizer::visitNE(NE* ne)
{
    /* Code For NE Goes Here*/


}


void TreeOptimizer::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListArg(ListArg* listarg)
{
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListStmt(ListStmt* liststmt)
{
    for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListItem(ListItem* listitem)
{
    for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListType(ListType* listtype)
{
    for (ListType::iterator i = listtype->begin() ; i != listtype->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TreeOptimizer::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void TreeOptimizer::visitInteger(Integer x)
{
    /* Code for Integer Goes Here*/
}

void TreeOptimizer::visitChar(Char x)
{
    /* Code for Char Goes Here*/
}

void TreeOptimizer::visitDouble(Double x)
{
    /* Code for Double Goes Here*/
}

void TreeOptimizer::visitString(String x)
{
    /* Code for String Goes Here*/
}

void TreeOptimizer::visitIdent(Ident x)
{
}


} /* namespace frontend */
