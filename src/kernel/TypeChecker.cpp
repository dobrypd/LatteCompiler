/*
*  Author: Piotr Dobrowolski
*  pd291528@students.mimuw.edu.pl
*
*/

#include <iostream>
#include "TypeChecker.h"

namespace frontend
{

void TypeChecker::visitProg(Prog* t) {} //abstract class
void TypeChecker::visitTopDef(TopDef* t) {} //abstract class
void TypeChecker::visitArg(Arg* t) {} //abstract class
void TypeChecker::visitBlk(Blk* t) {} //abstract class
void TypeChecker::visitStmt(Stmt* t) {} //abstract class
void TypeChecker::visitItem(Item* t) {} //abstract class
void TypeChecker::visitType(Type* t) {} //abstract class
void TypeChecker::visitExpr(Expr* t) {} //abstract class
void TypeChecker::visitAddOp(AddOp* t) {} //abstract class
void TypeChecker::visitMulOp(MulOp* t) {} //abstract class
void TypeChecker::visitRelOp(RelOp* t) {} //abstract class

void TypeChecker::visitProgram(Program* program)
{
  /* Code For Program Goes Here*/

  program->listtopdef_->accept(this);

}

void TypeChecker::visitFnDef(FnDef* fndef)
{
  /* Code For FnDef Goes Here*/

  fndef->type_->accept(this);
  visitIdent(fndef->ident_);
  fndef->listarg_->accept(this);
  fndef->blk_->accept(this);

}

void TypeChecker::visitArgument(Argument* argument)
{
  /* Code For Argument Goes Here*/

  argument->type_->accept(this);
  visitIdent(argument->ident_);

}

void TypeChecker::visitStmBlock(StmBlock* stmblock)
{
  /* Code For StmBlock Goes Here*/

  stmblock->liststmt_->accept(this);

}

void TypeChecker::visitStmEmpty(StmEmpty* stmempty)
{
  /* Code For StmEmpty Goes Here*/


}

void TypeChecker::visitStmBStmt(StmBStmt* stmbstmt)
{
  /* Code For StmBStmt Goes Here*/

  stmbstmt->blk_->accept(this);

}

void TypeChecker::visitStmDecl(StmDecl* stmdecl)
{
  /* Code For StmDecl Goes Here*/

  stmdecl->type_->accept(this);
  stmdecl->listitem_->accept(this);

}

void TypeChecker::visitStmAss(StmAss* stmass)
{
  /* Code For StmAss Goes Here*/

  visitIdent(stmass->ident_);
  stmass->expr_->accept(this);

}

void TypeChecker::visitStmIncr(StmIncr* stmincr)
{
  /* Code For StmIncr Goes Here*/

  visitIdent(stmincr->ident_);

}

void TypeChecker::visitStmDecr(StmDecr* stmdecr)
{
  /* Code For StmDecr Goes Here*/

  visitIdent(stmdecr->ident_);

}

void TypeChecker::visitStmRet(StmRet* stmret)
{
  /* Code For StmRet Goes Here*/

  stmret->expr_->accept(this);

}

void TypeChecker::visitStmVRet(StmVRet* stmvret)
{
  /* Code For StmVRet Goes Here*/


}

void TypeChecker::visitStmCond(StmCond* stmcond)
{
  /* Code For StmCond Goes Here*/

  stmcond->expr_->accept(this);
  stmcond->stmt_->accept(this);

}

void TypeChecker::visitStmCondElse(StmCondElse* stmcondelse)
{
  /* Code For StmCondElse Goes Here*/

  stmcondelse->expr_->accept(this);
  stmcondelse->stmt_1->accept(this);
  stmcondelse->stmt_2->accept(this);

}

void TypeChecker::visitStmWhile(StmWhile* stmwhile)
{
  /* Code For StmWhile Goes Here*/

  stmwhile->expr_->accept(this);
  stmwhile->stmt_->accept(this);

}

void TypeChecker::visitStmSExp(StmSExp* stmsexp)
{
  /* Code For StmSExp Goes Here*/

  stmsexp->expr_->accept(this);

}

void TypeChecker::visitStmNoInit(StmNoInit* stmnoinit)
{
  /* Code For StmNoInit Goes Here*/

  visitIdent(stmnoinit->ident_);

}

void TypeChecker::visitStmInit(StmInit* stminit)
{
  /* Code For StmInit Goes Here*/

  visitIdent(stminit->ident_);
  stminit->expr_->accept(this);

}

void TypeChecker::visitInt(Int* integer)
{
  /* Code For Int Goes Here*/


}

void TypeChecker::visitStr(Str* str)
{
  /* Code For Str Goes Here*/


}

void TypeChecker::visitBool(Bool* boolean)
{
  /* Code For Bool Goes Here*/


}

void TypeChecker::visitVoid(Void* void_field)
{
  /* Code For Void Goes Here*/


}

void TypeChecker::visitFun(Fun* fun)
{
  /* Code For Fun Goes Here*/

  fun->type_->accept(this);
  fun->listtype_->accept(this);

}

void TypeChecker::visitEVar(EVar* evar)
{
  /* Code For EVar Goes Here*/

  visitIdent(evar->ident_);

}

void TypeChecker::visitELitInt(ELitInt* elitint)
{
  /* Code For ELitInt Goes Here*/

  visitInteger(elitint->integer_);

}

void TypeChecker::visitELitTrue(ELitTrue* elittrue)
{
  /* Code For ELitTrue Goes Here*/


}

void TypeChecker::visitELitFalse(ELitFalse* elitfalse)
{
  /* Code For ELitFalse Goes Here*/


}

void TypeChecker::visitEApp(EApp* eapp)
{
  /* Code For EApp Goes Here*/

  visitIdent(eapp->ident_);
  eapp->listexpr_->accept(this);

}

void TypeChecker::visitEString(EString* estring)
{
  /* Code For EString Goes Here*/

  visitString(estring->string_);

}

void TypeChecker::visitNeg(Neg* neg)
{
  /* Code For Neg Goes Here*/

  neg->expr_->accept(this);

}

void TypeChecker::visitNot(Not* not_field)
{
  /* Code For Not Goes Here*/

  not_field->expr_->accept(this);

}

void TypeChecker::visitEMul(EMul* emul)
{
  /* Code For EMul Goes Here*/

  emul->expr_1->accept(this);
  emul->mulop_->accept(this);
  emul->expr_2->accept(this);

}

void TypeChecker::visitEAdd(EAdd* eadd)
{
  /* Code For EAdd Goes Here*/

  eadd->expr_1->accept(this);
  eadd->addop_->accept(this);
  eadd->expr_2->accept(this);

}

void TypeChecker::visitERel(ERel* erel)
{
  /* Code For ERel Goes Here*/

  erel->expr_1->accept(this);
  erel->relop_->accept(this);
  erel->expr_2->accept(this);

}

void TypeChecker::visitEAnd(EAnd* eand)
{
  /* Code For EAnd Goes Here*/

  eand->expr_1->accept(this);
  eand->expr_2->accept(this);

}

void TypeChecker::visitEOr(EOr* eor)
{
  /* Code For EOr Goes Here*/

  eor->expr_1->accept(this);
  eor->expr_2->accept(this);

}

void TypeChecker::visitPlus(Plus* plus)
{
  /* Code For Plus Goes Here*/


}

void TypeChecker::visitMinus(Minus* minus)
{
  /* Code For Minus Goes Here*/


}

void TypeChecker::visitTimes(Times* times)
{
  /* Code For Times Goes Here*/


}

void TypeChecker::visitDiv(Div* div)
{
  /* Code For Div Goes Here*/


}

void TypeChecker::visitMod(Mod* mod)
{
  /* Code For Mod Goes Here*/


}

void TypeChecker::visitLTH(LTH* lth)
{
  /* Code For LTH Goes Here*/


}

void TypeChecker::visitLE(LE* le)
{
  /* Code For LE Goes Here*/


}

void TypeChecker::visitGTH(GTH* gth)
{
  /* Code For GTH Goes Here*/


}

void TypeChecker::visitGE(GE* ge)
{
  /* Code For GE Goes Here*/


}

void TypeChecker::visitEQU(EQU* equ)
{
  /* Code For EQU Goes Here*/


}

void TypeChecker::visitNE(NE* ne)
{
  /* Code For NE Goes Here*/


}


void TypeChecker::visitListTopDef(ListTopDef* listtopdef)
{
  for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void TypeChecker::visitListArg(ListArg* listarg)
{
  for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void TypeChecker::visitListStmt(ListStmt* liststmt)
{
  for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void TypeChecker::visitListItem(ListItem* listitem)
{
  for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void TypeChecker::visitListType(ListType* listtype)
{
  for (ListType::iterator i = listtype->begin() ; i != listtype->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void TypeChecker::visitListExpr(ListExpr* listexpr)
{
  for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void TypeChecker::visitInteger(Integer x)
{
  /* Code for Integer Goes Here*/
}

void TypeChecker::visitChar(Char x)
{
  /* Code for Char Goes Here*/
}

void TypeChecker::visitDouble(Double x)
{
  /* Code for Double Goes Here*/
}

void TypeChecker::visitString(String x)
{
  /* Code for String Goes Here*/
}

void TypeChecker::visitIdent(Ident x)
{
  /* Code for Ident Goes Here*/
    std::cout << x << std::endl;
}

} /* namespace frontend*/
