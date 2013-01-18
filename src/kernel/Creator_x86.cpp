/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "Environment.h"
#include "Instruction.h"
#include "CompilerEnvironment.h"
#include "global.h"
#include "Creator_x86.h"


namespace backend
{

Creator_x86::Creator_x86(InstructionManager& instruction_manager,
        frontend::Environment& frontend_environment) :
        instruction_manager(instruction_manager),
        fr_env(frontend_environment)
{

}


void Creator_x86::visitProg(Prog* t) {} //abstract class
void Creator_x86::visitTopDef(TopDef* t) {} //abstract class
void Creator_x86::visitArg(Arg* t) {} //abstract class
void Creator_x86::visitClsDef(ClsDef* t) {} //abstract class
void Creator_x86::visitBlk(Blk* t) {} //abstract class
void Creator_x86::visitStmt(Stmt* t) {} //abstract class
void Creator_x86::visitItem(Item* t) {} //abstract class
void Creator_x86::visitStructuredIdent(StructuredIdent* t) {} //abstract class
void Creator_x86::visitArrayIndex(ArrayIndex* t) {} //abstract class
void Creator_x86::visitType(Type* t) {} //abstract class
void Creator_x86::visitExpr(Expr* t) {} //abstract class
void Creator_x86::visitAddOp(AddOp* t) {} //abstract class
void Creator_x86::visitMulOp(MulOp* t) {} //abstract class
void Creator_x86::visitRelOp(RelOp* t) {} //abstract class

void Creator_x86::visitProgram(Program *program)
{
    program->listtopdef_->accept(this);
}

void Creator_x86::visitFnDef(FnDef *fndef)
{
    this->instruction_manager.new_block(fndef->ident_);
    this->env.prepare();

    fndef->type_->accept(this);
    visitIdent(fndef->ident_);
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);

    this->env.back();
}

void Creator_x86::visitClsDefNoInher(ClsDefNoInher *clsdefnoinher)
{
    visitIdent(clsdefnoinher->ident_);
    clsdefnoinher->listclsdef_->accept(this);
}

void Creator_x86::visitClsDefInher(ClsDefInher *clsdefinher)
{

    visitIdent(clsdefinher->ident_1);
    visitIdent(clsdefinher->ident_2);
    clsdefinher->listclsdef_->accept(this);

}

void Creator_x86::visitArgument(Argument *argument)
{
    /* Code For Argument Goes Here */

    argument->type_->accept(this);
    visitIdent(argument->ident_);

}

void Creator_x86::visitMethodDef(MethodDef *methoddef)
{

    methoddef->type_->accept(this);
    visitIdent(methoddef->ident_);
    methoddef->listarg_->accept(this);
    methoddef->blk_->accept(this);

}

void Creator_x86::visitFieldDef(FieldDef *fielddef)
{
    fielddef->type_->accept(this);
    visitIdent(fielddef->ident_);
}

void Creator_x86::visitStmBlock(StmBlock *stmblock)
{
    stmblock->liststmt_->accept(this);
}

void Creator_x86::visitStmEmpty(StmEmpty *stmempty)
{
}

void Creator_x86::visitStmBStmt(StmBStmt *stmbstmt)
{
    this->env.prepare();
    stmbstmt->blk_->accept(this);
    this->env.back();
}

void Creator_x86::visitStmDecl(StmDecl *stmdecl)
{
    stmdecl->type_->accept(this);
    this->declaration_type = stmdecl->type_;
    stmdecl->listitem_->accept(this);
}

void Creator_x86::visitStmAss(StmAss *stmass)
{
    stmass->liststructuredident_->accept(this);

//    CompilerEnvironment::VarInfoPtr var_ptr =
//            this->env.get_variable(stmass->liststructuredident_);
//    int stack_offset = this->env.stack_all_offset() - var_ptr->position;

    stmass->expr_->accept(this);

//    if (var_ptr->on_stack) {
//        this->instruction_manager.pop_deeper_on_stack(stack_offset);
//    } else {
//        this->instruction_manager.get_addr_to_EDI(stmass->liststructuredident_);
//        // Save value from top of the stack to address in EDX
//        this->instruction_manager.pop_to_addr_from_EDI();
//    }
}

void Creator_x86::visitStmAssArr(StmAssArr *stmassarr)
{
    stmassarr->liststructuredident_->accept(this);

//    CompilerEnvironment::VarInfoPtr var_ptr =
//            this->env.get_variable(stmassarr->liststructuredident_);
//    int stack_offset = this->env.stack_all_offset() - var_ptr->position;

//    this->instruction_manager.get_addr_to_EDI(stmassarr->liststructuredident_);
//    // Save value from top of the stack to address in EDX
//    this->instruction_manager.pop_to_addr_from_EDI();

    stmassarr->type_->accept(this);
    stmassarr->expr_->accept(this);
}

void Creator_x86::visitStmAssObj(StmAssObj *stmassobj)
{
    /* Code For StmAssObj Goes Here */
    /* Latte++ */

    stmassobj->liststructuredident_->accept(this);
    stmassobj->type_->accept(this);

}

void Creator_x86::visitStmIncr(StmIncr *stmincr)
{
    stmincr->liststructuredident_->accept(this);
}

void Creator_x86::visitStmDecr(StmDecr *stmdecr)
{
    /* Code For StmDecr Goes Here */
    /* Latte++ */

    stmdecr->liststructuredident_->accept(this);

}

void Creator_x86::visitStmRet(StmRet *stmret)
{
    /* Code For StmRet Goes Here */

    stmret->expr_->accept(this);

}

void Creator_x86::visitStmVRet(StmVRet *stmvret)
{
    /* Code For StmVRet Goes Here */


}

void Creator_x86::visitStmCond(StmCond *stmcond)
{
    /* Code For StmCond Goes Here */

    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);

}

void Creator_x86::visitStmCondElse(StmCondElse *stmcondelse)
{
    /* Code For StmCondElse Goes Here */

    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);

}

void Creator_x86::visitStmWhile(StmWhile *stmwhile)
{
    /* Code For StmWhile Goes Here */

    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);

}

void Creator_x86::visitStmForeach(StmForeach *stmforeach)
{
    /* Code For StmForeach Goes Here */
    /* Latte++ */

    stmforeach->type_->accept(this);
    visitIdent(stmforeach->ident_);
    stmforeach->liststructuredident_->accept(this);
    stmforeach->stmt_->accept(this);

}

void Creator_x86::visitStmSExp(StmSExp *stmsexp)
{
    /* Code For StmSExp Goes Here */

    stmsexp->expr_->accept(this);

}

void Creator_x86::visitStmNoInit(StmNoInit *stmnoinit)
{
    visitIdent(stmnoinit->ident_);
    this->env.add_variable(this->declaration_type, stmnoinit->ident_);
    this->instruction_manager.alloc_default(this->declaration_type);
}

void Creator_x86::visitStmInit(StmInit *stminit)
{
    visitIdent(stminit->ident_);
    this->env.add_variable(this->declaration_type, stminit->ident_);
    stminit->expr_->accept(this);
    // Default value should be on stack. Do not pop it.
}

void Creator_x86::visitStmInitArray(StmInitArray *stminitarray)
{
    visitIdent(stminitarray->ident_);
    stminitarray->type_->accept(this);
    stminitarray->expr_->accept(this);

    this->instruction_manager.alloc_array(
            CompilerEnvironment::type_sizeof(stminitarray->type_));
}

void Creator_x86::visitStmInitObj(StmInitObj *stminitobj)
{
    /* Code For StmInitObj Goes Here */
    /* Latte++ */

    visitIdent(stminitobj->ident_);
    stminitobj->type_->accept(this);

}

void Creator_x86::visitSingleIdent(SingleIdent *singleident)
{
    /* Code For SingleIdent Goes Here */
    /* Latte++ */

    visitIdent(singleident->ident_);

}

void Creator_x86::visitTableVal(TableVal *tableval)
{
    /* Code For TableVal Goes Here */
    /* Latte++ */

    visitIdent(tableval->ident_);
    tableval->listarrayindex_->accept(this);

}

void Creator_x86::visitSelfIdent(SelfIdent *selfident)
{
  /* Code For SelfIdent Goes Here */


}

void Creator_x86::visitExprIndex(ExprIndex *exprindex)
{
    /* Code For ExprIndex Goes Here */
    /* Latte++ */

    exprindex->expr_->accept(this);

}

void Creator_x86::visitClass(Class *_class)
{
    /* Code For Class Goes Here */
    /* Latte++ */

    visitIdent(_class->ident_);

}

void Creator_x86::visitInt(Int *_int)
{
    /* Code For Int Goes Here */


}

void Creator_x86::visitStr(Str *str)
{
    /* Code For Str Goes Here */


}

void Creator_x86::visitBool(Bool *_bool)
{

}

void Creator_x86::visitVoid(Void *_void)
{

}

void Creator_x86::visitTType(TType *ttype)
{
    /* Code For TType Goes Here */
    /* Latte++ */

    ttype->type_->accept(this);

}

void Creator_x86::visitEVar(EVar *evar)
{
    /* Code For EVar Goes Here */
    /* Latte++ */

    evar->liststructuredident_->accept(this);

}

void Creator_x86::visitELitInt(ELitInt *elitint)
{
    visitInteger(elitint->integer_);
    //this->instruction_manager.add(push)
}

void Creator_x86::visitELitTrue(ELitTrue *elittrue)
{
}

void Creator_x86::visitELitFalse(ELitFalse *elitfalse)
{
}

void Creator_x86::visitELitNull(ELitNull *elitnull)
{
}

void Creator_x86::visitEApp(EApp *eapp)
{

    eapp->liststructuredident_->accept(this);
    eapp->listexpr_->accept(this);

    //Block::instr_ptr_t call(new x86_Call(get_from_vtable(eapp->liststructuredident_)));
    //this->instruction_manager.add(call);
}

void Creator_x86::visitEString(EString *estring)
{
    /* Code For EString Goes Here */

    visitString(estring->string_);

}

void Creator_x86::visitNeg(Neg *neg)
{
    /* Code For Neg Goes Here */

    neg->expr_->accept(this);

}

void Creator_x86::visitNot(Not *_not)
{
    /* Code For Not Goes Here */

    _not->expr_->accept(this);

}

void Creator_x86::visitEDynamicCast(EDynamicCast *edynamiccast)
{
    /* Code For EDynamicCast Goes Here */
    /* Latte++ */

    visitIdent(edynamiccast->ident_);
    edynamiccast->expr_->accept(this);

}

void Creator_x86::visitEMul(EMul *emul)
{
    /* Code For EMul Goes Here */

    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);

}

void Creator_x86::visitEAdd(EAdd *eadd)
{
    /* Code For EAdd Goes Here */

    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);

}

void Creator_x86::visitERel(ERel *erel)
{
    /* Code For ERel Goes Here */

    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);

}

void Creator_x86::visitEAnd(EAnd *eand)
{
    /* Code For EAnd Goes Here */

    eand->expr_1->accept(this);
    eand->expr_2->accept(this);

}

void Creator_x86::visitEOr(EOr *eor)
{
    /* Code For EOr Goes Here */

    eor->expr_1->accept(this);
    eor->expr_2->accept(this);

}

void Creator_x86::visitPlus(Plus *plus)
{
    /* Code For Plus Goes Here */


}

void Creator_x86::visitMinus(Minus *minus)
{
    /* Code For Minus Goes Here */


}

void Creator_x86::visitTimes(Times *times)
{
    /* Code For Times Goes Here */


}

void Creator_x86::visitDiv(Div *div)
{
    /* Code For Div Goes Here */


}

void Creator_x86::visitMod(Mod *mod)
{
    /* Code For Mod Goes Here */


}

void Creator_x86::visitLTH(LTH *lth)
{
    /* Code For LTH Goes Here */


}

void Creator_x86::visitLE(LE *le)
{
    /* Code For LE Goes Here */


}

void Creator_x86::visitGTH(GTH *gth)
{
    /* Code For GTH Goes Here */


}

void Creator_x86::visitGE(GE *ge)
{
    /* Code For GE Goes Here */


}

void Creator_x86::visitEQU(EQU *equ)
{
    /* Code For EQU Goes Here */


}

void Creator_x86::visitNE(NE *ne)
{
    /* Code For NE Goes Here */


}


void Creator_x86::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListArg(ListArg* listarg)
{
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListClsDef(ListClsDef* listclsdef)
{
    /* Latte++ */
    for (ListClsDef::iterator i = listclsdef->begin() ; i != listclsdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListStmt(ListStmt* liststmt)
{
    for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListItem(ListItem* listitem)
{
    for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListArrayIndex(ListArrayIndex* listarrayindex)
{
    /* Latte++ */
    for (ListArrayIndex::iterator i = listarrayindex->begin() ; i != listarrayindex->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListStructuredIdent(ListStructuredIdent* liststructuredident)
{
    /* Latte++ */
    for (ListStructuredIdent::iterator i = liststructuredident->begin() ; i != liststructuredident->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListExpr(ListExpr* listexpr)
{
    /* Latte++ */
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void Creator_x86::visitInteger(Integer x)
{
    /* Code for Integer Goes Here */
}

void Creator_x86::visitChar(Char x)
{
    /* Code for Char Goes Here */
}

void Creator_x86::visitDouble(Double x)
{
    /* Code for Double Goes Here */
}

void Creator_x86::visitString(String x)
{
    /* Code for String Goes Here */
}

void Creator_x86::visitIdent(Ident x)
{
    /* Code for Ident Goes Here */
}


} /* namespace backend */

