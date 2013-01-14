/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "Creator_x86.H"


namespace backend
{

void Skeleton::visitProg(Prog* t) {} //abstract class
void Skeleton::visitTopDef(TopDef* t) {} //abstract class
void Skeleton::visitArg(Arg* t) {} //abstract class
void Skeleton::visitClsDef(ClsDef* t) {} //abstract class
void Skeleton::visitBlk(Blk* t) {} //abstract class
void Skeleton::visitStmt(Stmt* t) {} //abstract class
void Skeleton::visitItem(Item* t) {} //abstract class
void Skeleton::visitStructuredIdent(StructuredIdent* t) {} //abstract class
void Skeleton::visitArrayIndex(ArrayIndex* t) {} //abstract class
void Skeleton::visitType(Type* t) {} //abstract class
void Skeleton::visitExpr(Expr* t) {} //abstract class
void Skeleton::visitAddOp(AddOp* t) {} //abstract class
void Skeleton::visitMulOp(MulOp* t) {} //abstract class
void Skeleton::visitRelOp(RelOp* t) {} //abstract class

void Skeleton::visitProgram(Program *program)
{
    /* Code For Program Goes Here */

    program->listtopdef_->accept(this);

}

void Skeleton::visitFnDef(FnDef *fndef)
{
    /* Code For FnDef Goes Here */

    fndef->type_->accept(this);
    visitIdent(fndef->ident_);
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);

}

void Skeleton::visitClsDefNoInher(ClsDefNoInher *clsdefnoinher)
{
    /* Code For ClsDefNoInher Goes Here */
    /* Latte++ */

    visitIdent(clsdefnoinher->ident_);
    clsdefnoinher->listclsdef_->accept(this);

}

void Skeleton::visitClsDefInher(ClsDefInher *clsdefinher)
{
    /* Code For ClsDefInher Goes Here */
    /* Latte++ */

    visitIdent(clsdefinher->ident_1);
    visitIdent(clsdefinher->ident_2);
    clsdefinher->listclsdef_->accept(this);

}

void Skeleton::visitArgument(Argument *argument)
{
    /* Code For Argument Goes Here */

    argument->type_->accept(this);
    visitIdent(argument->ident_);

}

void Skeleton::visitMethodDef(MethodDef *methoddef)
{
    /* Code For MethodDef Goes Here */
    /* Latte++ */

    methoddef->type_->accept(this);
    visitIdent(methoddef->ident_);
    methoddef->listarg_->accept(this);
    methoddef->blk_->accept(this);

}

void Skeleton::visitFieldDef(FieldDef *fielddef)
{
    /* Code For FieldDef Goes Here */
    /* Latte++ */

    fielddef->type_->accept(this);
    visitIdent(fielddef->ident_);

}

void Skeleton::visitStmBlock(StmBlock *stmblock)
{
    /* Code For StmBlock Goes Here */

    stmblock->liststmt_->accept(this);

}

void Skeleton::visitStmEmpty(StmEmpty *stmempty)
{
    /* Code For StmEmpty Goes Here */


}

void Skeleton::visitStmBStmt(StmBStmt *stmbstmt)
{
    /* Code For StmBStmt Goes Here */

    stmbstmt->blk_->accept(this);

}

void Skeleton::visitStmDecl(StmDecl *stmdecl)
{
    /* Code For StmDecl Goes Here */

    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);

}

void Skeleton::visitStmAss(StmAss *stmass)
{
    /* Code For StmAss Goes Here */
    /* Latte++ */

    stmass->liststructuredident_->accept(this);
    stmass->expr_->accept(this);

}

void Skeleton::visitStmAssArr(StmAssArr *stmassarr)
{
    /* Code For StmAssArr Goes Here */
    /* Latte++ */

    stmassarr->liststructuredident_->accept(this);
    stmassarr->type_->accept(this);
    stmassarr->expr_->accept(this);

}

void Skeleton::visitStmAssObj(StmAssObj *stmassobj)
{
    /* Code For StmAssObj Goes Here */
    /* Latte++ */

    stmassobj->liststructuredident_->accept(this);
    stmassobj->type_->accept(this);

}

void Skeleton::visitStmIncr(StmIncr *stmincr)
{
    /* Code For StmIncr Goes Here */
    /* Latte++ */

    stmincr->liststructuredident_->accept(this);

}

void Skeleton::visitStmDecr(StmDecr *stmdecr)
{
    /* Code For StmDecr Goes Here */
    /* Latte++ */

    stmdecr->liststructuredident_->accept(this);

}

void Skeleton::visitStmRet(StmRet *stmret)
{
    /* Code For StmRet Goes Here */

    stmret->expr_->accept(this);

}

void Skeleton::visitStmVRet(StmVRet *stmvret)
{
    /* Code For StmVRet Goes Here */


}

void Skeleton::visitStmCond(StmCond *stmcond)
{
    /* Code For StmCond Goes Here */

    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);

}

void Skeleton::visitStmCondElse(StmCondElse *stmcondelse)
{
    /* Code For StmCondElse Goes Here */

    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);

}

void Skeleton::visitStmWhile(StmWhile *stmwhile)
{
    /* Code For StmWhile Goes Here */

    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);

}

void Skeleton::visitStmForeach(StmForeach *stmforeach)
{
    /* Code For StmForeach Goes Here */
    /* Latte++ */

    stmforeach->type_->accept(this);
    visitIdent(stmforeach->ident_);
    stmforeach->liststructuredident_->accept(this);
    stmforeach->stmt_->accept(this);

}

void Skeleton::visitStmSExp(StmSExp *stmsexp)
{
    /* Code For StmSExp Goes Here */

    stmsexp->expr_->accept(this);

}

void Skeleton::visitStmNoInit(StmNoInit *stmnoinit)
{
    /* Code For StmNoInit Goes Here */

    visitIdent(stmnoinit->ident_);

}

void Skeleton::visitStmInit(StmInit *stminit)
{
    /* Code For StmInit Goes Here */

    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);

}

void Skeleton::visitStmInitArray(StmInitArray *stminitarray)
{
    /* Code For StmInitArray Goes Here */
    /* Latte++ */

    visitIdent(stminitarray->ident_);
    stminitarray->type_->accept(this);
    stminitarray->expr_->accept(this);

}

void Skeleton::visitStmInitObj(StmInitObj *stminitobj)
{
    /* Code For StmInitObj Goes Here */
    /* Latte++ */

    visitIdent(stminitobj->ident_);
    stminitobj->type_->accept(this);

}

void Skeleton::visitSingleIdent(SingleIdent *singleident)
{
    /* Code For SingleIdent Goes Here */
    /* Latte++ */

    visitIdent(singleident->ident_);

}

void Skeleton::visitTableVal(TableVal *tableval)
{
    /* Code For TableVal Goes Here */
    /* Latte++ */

    visitIdent(tableval->ident_);
    tableval->listarrayindex_->accept(this);

}

void Skeleton::visitExprIndex(ExprIndex *exprindex)
{
    /* Code For ExprIndex Goes Here */
    /* Latte++ */

    exprindex->expr_->accept(this);

}

void Skeleton::visitClass(Class *_class)
{
    /* Code For Class Goes Here */
    /* Latte++ */

    visitIdent(_class->ident_);

}

void Skeleton::visitInt(Int *_int)
{
    /* Code For Int Goes Here */


}

void Skeleton::visitStr(Str *str)
{
    /* Code For Str Goes Here */


}

void Skeleton::visitBool(Bool *_bool)
{
    /* Code For Bool Goes Here */


}

void Skeleton::visitVoid(Void *_void)
{
    /* Code For Void Goes Here */


}

void Skeleton::visitTType(TType *ttype)
{
    /* Code For TType Goes Here */
    /* Latte++ */

    ttype->type_->accept(this);

}

void Skeleton::visitEVar(EVar *evar)
{
    /* Code For EVar Goes Here */
    /* Latte++ */

    evar->liststructuredident_->accept(this);

}

void Skeleton::visitELitInt(ELitInt *elitint)
{
    /* Code For ELitInt Goes Here */

    visitInteger(elitint->integer_);

}

void Skeleton::visitELitTrue(ELitTrue *elittrue)
{
    /* Code For ELitTrue Goes Here */


}

void Skeleton::visitELitFalse(ELitFalse *elitfalse)
{
    /* Code For ELitFalse Goes Here */


}

void Skeleton::visitEApp(EApp *eapp)
{
    /* Code For EApp Goes Here */
    /* Latte++ */

    eapp->liststructuredident_->accept(this);
    eapp->listexpr_->accept(this);

}

void Skeleton::visitEString(EString *estring)
{
    /* Code For EString Goes Here */

    visitString(estring->string_);

}

void Skeleton::visitNeg(Neg *neg)
{
    /* Code For Neg Goes Here */

    neg->expr_->accept(this);

}

void Skeleton::visitNot(Not *_not)
{
    /* Code For Not Goes Here */

    not->expr_->accept(this);

}

void Skeleton::visitEDynamicCast(EDynamicCast *edynamiccast)
{
    /* Code For EDynamicCast Goes Here */
    /* Latte++ */

    visitIdent(edynamiccast->ident_);
    edynamiccast->expr_->accept(this);

}

void Skeleton::visitEMul(EMul *emul)
{
    /* Code For EMul Goes Here */

    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);

}

void Skeleton::visitEAdd(EAdd *eadd)
{
    /* Code For EAdd Goes Here */

    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);

}

void Skeleton::visitERel(ERel *erel)
{
    /* Code For ERel Goes Here */

    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);

}

void Skeleton::visitEAnd(EAnd *eand)
{
    /* Code For EAnd Goes Here */

    eand->expr_1->accept(this);
    eand->expr_2->accept(this);

}

void Skeleton::visitEOr(EOr *eor)
{
    /* Code For EOr Goes Here */

    eor->expr_1->accept(this);
    eor->expr_2->accept(this);

}

void Skeleton::visitPlus(Plus *plus)
{
    /* Code For Plus Goes Here */


}

void Skeleton::visitMinus(Minus *minus)
{
    /* Code For Minus Goes Here */


}

void Skeleton::visitTimes(Times *times)
{
    /* Code For Times Goes Here */


}

void Skeleton::visitDiv(Div *div)
{
    /* Code For Div Goes Here */


}

void Skeleton::visitMod(Mod *mod)
{
    /* Code For Mod Goes Here */


}

void Skeleton::visitLTH(LTH *lth)
{
    /* Code For LTH Goes Here */


}

void Skeleton::visitLE(LE *le)
{
    /* Code For LE Goes Here */


}

void Skeleton::visitGTH(GTH *gth)
{
    /* Code For GTH Goes Here */


}

void Skeleton::visitGE(GE *ge)
{
    /* Code For GE Goes Here */


}

void Skeleton::visitEQU(EQU *equ)
{
    /* Code For EQU Goes Here */


}

void Skeleton::visitNE(NE *ne)
{
    /* Code For NE Goes Here */


}


void Skeleton::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Skeleton::visitListArg(ListArg* listarg)
{
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Skeleton::visitListClsDef(ListClsDef* listclsdef)
{
    /* Latte++ */
    for (ListClsDef::iterator i = listclsdef->begin() ; i != listclsdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Skeleton::visitListStmt(ListStmt* liststmt)
{
    for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Skeleton::visitListItem(ListItem* listitem)
{
    for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Skeleton::visitListArrayIndex(ListArrayIndex* listarrayindex)
{
    /* Latte++ */
    for (ListArrayIndex::iterator i = listarrayindex->begin() ; i != listarrayindex->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Skeleton::visitListStructuredIdent(ListStructuredIdent* liststructuredident)
{
    /* Latte++ */
    for (ListStructuredIdent::iterator i = liststructuredident->begin() ; i != liststructuredident->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Skeleton::visitListExpr(ListExpr* listexpr)
{
    /* Latte++ */
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void Skeleton::visitInteger(Integer x)
{
    /* Code for Integer Goes Here */
}

void Skeleton::visitChar(Char x)
{
    /* Code for Char Goes Here */
}

void Skeleton::visitDouble(Double x)
{
    /* Code for Double Goes Here */
}

void Skeleton::visitString(String x)
{
    /* Code for String Goes Here */
}

void Skeleton::visitIdent(Ident x)
{
    /* Code for Ident Goes Here */
}


} /* namespace backend */

