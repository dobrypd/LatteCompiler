/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include "FunctionLoader.h"
#include "global.h"

namespace frontend
{

FunctionLoader::FunctionLoader(ErrorHandler& error_handler, Environment& env)
    : error_handler(error_handler), env(env)
{
}


void FunctionLoader::check(Visitable* v)
{
    v->accept(this);
}

void FunctionLoader::visitProg(Prog* t) { } //abstract class
void FunctionLoader::visitTopDef(TopDef* t) {} //abstract class
void FunctionLoader::visitArg(Arg* t) {} //abstract class
void FunctionLoader::visitBlk(Blk* t) {} //abstract class
void FunctionLoader::visitStmt(Stmt* t) {} //abstract class
void FunctionLoader::visitItem(Item* t) {} //abstract class
void FunctionLoader::visitType(Type* t) {} //abstract class
void FunctionLoader::visitExpr(Expr* t) {} //abstract class
void FunctionLoader::visitAddOp(AddOp* t) {} //abstract class
void FunctionLoader::visitMulOp(MulOp* t) {} //abstract class
void FunctionLoader::visitRelOp(RelOp* t) {} //abstract class

void FunctionLoader::visitProgram(Program* program)
{
    program->listtopdef_->accept(this);
}

void FunctionLoader::visitFnDef(FnDef* fndef)
{
    if (debug) {
        std::cout << fndef->line_number << " found function `"
                << fndef->ident_ << "` definition." << std::endl;
    }
    if (!this->env.lookup_function(fndef))
        this->env.add_function(fndef);
    else
    {
        std::string comunicate;
        comunicate += "funciton `";
        comunicate += fndef->ident_;
        comunicate += "` already declared.";
        this->error_handler.error(fndef->type_->line_number, comunicate);
    }
}

void FunctionLoader::visitArgument(Argument* argument)
{
    /* Code For Argument Goes Here*/
    argument->type_->accept(this);
    visitIdent(argument->ident_);

}

void FunctionLoader::visitStmBlock(StmBlock* stmblock)
{
    /* Code For StmBlock Goes Here*/

    stmblock->liststmt_->accept(this);

}

void FunctionLoader::visitStmEmpty(StmEmpty* stmempty)
{
    /* Code For StmEmpty Goes Here*/


}

void FunctionLoader::visitStmBStmt(StmBStmt* stmbstmt)
{
    /* Code For StmBStmt Goes Here*/

    stmbstmt->blk_->accept(this);

}

void FunctionLoader::visitStmDecl(StmDecl* stmdecl)
{
    /* Code For StmDecl Goes Here*/

    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);

}

void FunctionLoader::visitStmAss(StmAss* stmass)
{
    /* Code For StmAss Goes Here*/

    visitIdent(stmass->ident_);
    stmass->expr_->accept(this);

}

void FunctionLoader::visitStmIncr(StmIncr* stmincr)
{
    /* Code For StmIncr Goes Here*/

    visitIdent(stmincr->ident_);

}

void FunctionLoader::visitStmDecr(StmDecr* stmdecr)
{
    /* Code For StmDecr Goes Here*/

    visitIdent(stmdecr->ident_);

}

void FunctionLoader::visitStmRet(StmRet* stmret)
{
    /* Code For StmRet Goes Here*/

    stmret->expr_->accept(this);

}

void FunctionLoader::visitStmVRet(StmVRet* stmvret)
{
    /* Code For StmVRet Goes Here*/


}

void FunctionLoader::visitStmCond(StmCond* stmcond)
{
    /* Code For StmCond Goes Here*/

    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);

}

void FunctionLoader::visitStmCondElse(StmCondElse* stmcondelse)
{
    /* Code For StmCondElse Goes Here*/

    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);

}

void FunctionLoader::visitStmWhile(StmWhile* stmwhile)
{
    /* Code For StmWhile Goes Here*/

    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);

}

void FunctionLoader::visitStmSExp(StmSExp* stmsexp)
{
    /* Code For StmSExp Goes Here*/

    stmsexp->expr_->accept(this);

}

void FunctionLoader::visitStmNoInit(StmNoInit* stmnoinit)
{
    /* Code For StmNoInit Goes Here*/

    visitIdent(stmnoinit->ident_);

}

void FunctionLoader::visitStmInit(StmInit* stminit)
{
    /* Code For StmInit Goes Here*/

    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);

}

void FunctionLoader::visitInt(Int* integer)
{
    /* Code For Int Goes Here*/


}

void FunctionLoader::visitStr(Str* str)
{
    /* Code For Str Goes Here*/


}

void FunctionLoader::visitBool(Bool* boolean)
{
    /* Code For Bool Goes Here*/


}

void FunctionLoader::visitVoid(Void* void_field)
{
    /* Code For Void Goes Here*/


}

void FunctionLoader::visitFun(Fun* fun)
{
    /* Code For Fun Goes Here*/

    fun->type_->accept(this);
    fun->listtype_->accept(this);

}

void FunctionLoader::visitEVar(EVar* evar)
{
    /* Code For EVar Goes Here*/

    visitIdent(evar->ident_);

}

void FunctionLoader::visitELitInt(ELitInt* elitint)
{
    /* Code For ELitInt Goes Here*/

    visitInteger(elitint->integer_);

}

void FunctionLoader::visitELitTrue(ELitTrue* elittrue)
{
    /* Code For ELitTrue Goes Here*/


}

void FunctionLoader::visitELitFalse(ELitFalse* elitfalse)
{
    /* Code For ELitFalse Goes Here*/


}

void FunctionLoader::visitEApp(EApp* eapp)
{
    /* Code For EApp Goes Here*/

    visitIdent(eapp->ident_);
    eapp->listexpr_->accept(this);

}

void FunctionLoader::visitEString(EString* estring)
{
    /* Code For EString Goes Here*/

    visitString(estring->string_);

}

void FunctionLoader::visitNeg(Neg* neg)
{
    /* Code For Neg Goes Here*/

    neg->expr_->accept(this);

}

void FunctionLoader::visitNot(Not* not_field)
{
    /* Code For Not Goes Here*/

    not_field->expr_->accept(this);

}

void FunctionLoader::visitEMul(EMul* emul)
{
    /* Code For EMul Goes Here*/

    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);

}

void FunctionLoader::visitEAdd(EAdd* eadd)
{
    /* Code For EAdd Goes Here*/

    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);

}

void FunctionLoader::visitERel(ERel* erel)
{
    /* Code For ERel Goes Here*/

    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);

}

void FunctionLoader::visitEAnd(EAnd* eand)
{
    /* Code For EAnd Goes Here*/

    eand->expr_1->accept(this);
    eand->expr_2->accept(this);

}

void FunctionLoader::visitEOr(EOr* eor)
{
    /* Code For EOr Goes Here*/

    eor->expr_1->accept(this);
    eor->expr_2->accept(this);

}

void FunctionLoader::visitPlus(Plus* plus)
{
    /* Code For Plus Goes Here*/


}

void FunctionLoader::visitMinus(Minus* minus)
{
    /* Code For Minus Goes Here*/


}

void FunctionLoader::visitTimes(Times* times)
{
    /* Code For Times Goes Here*/


}

void FunctionLoader::visitDiv(Div* div)
{
    /* Code For Div Goes Here*/


}

void FunctionLoader::visitMod(Mod* mod)
{
    /* Code For Mod Goes Here*/


}

void FunctionLoader::visitLTH(LTH* lth)
{
    /* Code For LTH Goes Here*/


}

void FunctionLoader::visitLE(LE* le)
{
    /* Code For LE Goes Here*/


}

void FunctionLoader::visitGTH(GTH* gth)
{
    /* Code For GTH Goes Here*/


}

void FunctionLoader::visitGE(GE* ge)
{
    /* Code For GE Goes Here*/


}

void FunctionLoader::visitEQU(EQU* equ)
{
    /* Code For EQU Goes Here*/


}

void FunctionLoader::visitNE(NE* ne)
{
    /* Code For NE Goes Here*/


}


void FunctionLoader::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListArg(ListArg* listarg)
{
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListStmt(ListStmt* liststmt)
{
    for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListItem(ListItem* listitem)
{
    for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListType(ListType* listtype)
{
    for (ListType::iterator i = listtype->begin() ; i != listtype->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void FunctionLoader::visitInteger(Integer x)
{
    /* Code for Integer Goes Here*/
}

void FunctionLoader::visitChar(Char x)
{
    /* Code for Char Goes Here*/
}

void FunctionLoader::visitDouble(Double x)
{
    /* Code for Double Goes Here*/
}

void FunctionLoader::visitString(String x)
{
    /* Code for String Goes Here*/
}

void FunctionLoader::visitIdent(Ident x)
{
}

} /* namespace frontend */
