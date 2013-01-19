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


const char* Creator_x86::self_name = "self";

std::string Creator_x86::method_ident(std::string& class_name,
            std::string& method_name)
{
    return "_" + class_name + "@@" + method_name;
}

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
    this->instruction_manager.function_prologue();
    this->env.prepare();
    this->env.new_fun();
    fndef->type_->accept(this);
    visitIdent(fndef->ident_);
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);
    this->env.back(); // ESP will be changed by EBP
}

void Creator_x86::visitClsDefNoInher(ClsDefNoInher *clsdefnoinher)
{
    this->last_class_name = clsdefnoinher->ident_;
    visitIdent(clsdefnoinher->ident_);
    clsdefnoinher->listclsdef_->accept(this);

    // TODO: Create vtable
}

void Creator_x86::visitClsDefInher(ClsDefInher *clsdefinher)
{
    this->last_class_name = clsdefinher->ident_1;
    visitIdent(clsdefinher->ident_1);
    visitIdent(clsdefinher->ident_2);
    clsdefinher->listclsdef_->accept(this);

    // TODO: Create vtable

}

void Creator_x86::visitArgument(Argument *argument)
{
    this->env.add_variable(argument->type_, argument->ident_);

    argument->type_->accept(this);
    visitIdent(argument->ident_);
}

void Creator_x86::visitMethodDef(MethodDef *methoddef)
{
    this->instruction_manager.new_block(Creator_x86::method_ident(
            this->last_class_name, methoddef->ident_));
    this->instruction_manager.function_prologue();
    this->env.prepare();
    this->env.new_fun();
    this->env.add_obj(Creator_x86::self_name,
            this->fr_env.get_class(this->last_class_name));
    // Then add arguments.
    methoddef->type_->accept(this);
    visitIdent(methoddef->ident_);
    methoddef->listarg_->accept(this);

    methoddef->blk_->accept(this);
    this->env.back(); // ESP will be changed by EBP
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
    int ESP_diff = this->env.back();
    this->instruction_manager.add_to_ESP(ESP_diff);
    // ESP will be changed by EBP
}

void Creator_x86::visitStmDecl(StmDecl *stmdecl)
{
    stmdecl->type_->accept(this);
    this->declaration_type = stmdecl->type_;
    stmdecl->listitem_->accept(this);
}

void Creator_x86::visitStmAss(StmAss *stmass)
{
    this->current_var_on_stack = true;
    this->current_var_is_addr = false;
    this->current_var_offset = 0;
    stmass->liststructuredident_->accept(this);

    stmass->expr_->accept(this);

    // TODO: boolean!

    if ((this->current_var_on_stack) and !(this->current_var_is_addr)) {
        // POP - write direct to stack
        this->instruction_manager.pop_top_to_var(this->current_var_offset);
    } else if (this->current_var_is_addr) {
        // POP - write to stack (variable)
        this->instruction_manager.pop_top_to_addr(this->current_var_offset);
    } else {
        // POP - addres in memmory where write next POP
        this->instruction_manager.pop_sec_top_to_addr_on_top();
    }
}

void Creator_x86::visitStmAssArr(StmAssArr *stmassarr)
{

    this->current_var_on_stack = true;
    this->current_var_is_addr = false;
    this->current_var_offset = 0;
    stmassarr->liststructuredident_->accept(this);

    stmassarr->type_->accept(this);
    stmassarr->expr_->accept(this);

    this->instruction_manager.alloc_array(stmassarr->type_); // with len on stack

    if ((this->current_var_on_stack) and !(this->current_var_is_addr)) {
        // POP - write direct to stack
        this->instruction_manager.pop_top_to_var(this->current_var_offset);
    } else if (this->current_var_is_addr) {
        // POP - write to stack (variable)
        this->instruction_manager.pop_top_to_addr(this->current_var_offset);
    } else {
        // POP - addres in memmory where write next POP
        this->instruction_manager.pop_sec_top_to_addr_on_top();
    }
}

void Creator_x86::visitStmAssObj(StmAssObj *stmassobj)
{
    this->current_var_on_stack = true;
    this->current_var_is_addr = false;
    this->current_var_offset = 0;

    stmassobj->liststructuredident_->accept(this);
    stmassobj->type_->accept(this);

    this->instruction_manager.alloc_object(stmassobj->type_);

    if ((this->current_var_on_stack) and !(this->current_var_is_addr)) {
        // POP - write direct to stack
        this->instruction_manager.pop_top_to_var(this->current_var_offset);
    } else if (this->current_var_is_addr) {
        // POP - write to stack (variable)
        this->instruction_manager.pop_top_to_addr(this->current_var_offset);
    } else {
        // POP - addres in memmory where write next POP
        this->instruction_manager.pop_sec_top_to_addr_on_top();
    }
}

void Creator_x86::visitStmIncr(StmIncr *stmincr)
{
    this->current_var_is_addr = false;
    this->current_var_on_stack = true;
    this->current_var_offset = 0;

    stmincr->liststructuredident_->accept(this);

    if ((this->current_var_on_stack) and !(this->current_var_is_addr)) {
        this->instruction_manager.increment_var_on_stack(this->current_var_offset, 1);
    } else if (this->current_var_is_addr) {
        this->instruction_manager.increment_var_in_addr(this->current_var_offset, 1);
    } else {
        this->instruction_manager.increment_var_addr_on_top(1);
    }
}

void Creator_x86::visitStmDecr(StmDecr *stmdecr)
{
    this->current_var_is_addr = false;
    this->current_var_on_stack = false;
    this->current_var_offset = 0;

    stmdecr->liststructuredident_->accept(this);

    if ((this->current_var_on_stack) and !(this->current_var_is_addr)) {
        this->instruction_manager.increment_var_on_stack(this->current_var_offset, -1);
    } else if (this->current_var_is_addr) {
        this->instruction_manager.increment_var_in_addr(this->current_var_offset, -1);
    } else {
        this->instruction_manager.increment_var_addr_on_top(-1);
    }
}

void Creator_x86::visitStmRet(StmRet *stmret)
{
    stmret->expr_->accept(this);

    // TODO: boolean

    this->instruction_manager.top_to_EAX();
    this->instruction_manager.function_epilogue();
}

void Creator_x86::visitStmVRet(StmVRet *stmvret)
{
    this->instruction_manager.function_epilogue();
}

void Creator_x86::visitStmCond(StmCond *stmcond)
{
    // TODO:
    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);
}

void Creator_x86::visitStmCondElse(StmCondElse *stmcondelse)
{
    // TODO:
    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);
}

void Creator_x86::visitStmWhile(StmWhile *stmwhile)
{
    // TODO:
    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);
}

void Creator_x86::visitStmForeach(StmForeach *stmforeach)
{
    // TODO:
    stmforeach->type_->accept(this);
    visitIdent(stmforeach->ident_);
    stmforeach->liststructuredident_->accept(this);
    stmforeach->stmt_->accept(this);
}

void Creator_x86::visitStmSExp(StmSExp *stmsexp)
{
    stmsexp->expr_->accept(this);
    // TODO: boolean
    this->instruction_manager.add_to_ESP(1);
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
    // TODO: boolean
}

void Creator_x86::visitStmInitArray(StmInitArray *stminitarray)
{
    visitIdent(stminitarray->ident_);
    stminitarray->type_->accept(this);
    this->env.add_array(stminitarray->type_, stminitarray->ident_);
    stminitarray->expr_->accept(this);
    this->instruction_manager.alloc_array(stminitarray->type_); // with len on stack
}

void Creator_x86::visitStmInitObj(StmInitObj *stminitobj)
{
    visitIdent(stminitobj->ident_);
    stminitobj->type_->accept(this);
    frontend::Environment::ClsInfoPtr cls =
            this->fr_env.get_class((dynamic_cast<Class*>(stminitobj->type_)->ident_));
    this->env.add_obj(stminitobj->ident_, cls);
}

void Creator_x86::visitSingleIdent(SingleIdent *singleident)
{
    visitIdent(singleident->ident_);

    if ((this->current_var_on_stack) and !(this->current_var_is_addr)) {
        this->current_var_offset = this->env.get_variable(singleident->ident_)->position;
    } else if (this->current_var_is_addr) {
        // Array of objects:
        // TODO:
    } else {
        // Object
        // TODO:
    }
}

void Creator_x86::visitTableVal(TableVal *tableval)
{
    visitIdent(tableval->ident_);
    tableval->listarrayindex_->accept(this);
}

void Creator_x86::visitSelfIdent(SelfIdent *selfident)
{
    /* self should be always first if not - do nothing with rest values */
    std::string self = Creator_x86::self_name;
    this->current_var_offset = this->env.get_variable(self)->position;
    this->current_var_is_addr = true;
    this->current_var_on_stack = true;
}

void Creator_x86::visitExprIndex(ExprIndex *exprindex)
{
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
    visitString(estring->string_);
    this->instruction_manager.add_const_string(estring->string_);
}

void Creator_x86::visitNeg(Neg *neg)
{
    neg->expr_->accept(this);
}

void Creator_x86::visitNot(Not *_not)
{
    _not->expr_->accept(this);
}

void Creator_x86::visitEDynamicCast(EDynamicCast *edynamiccast)
{
    visitIdent(edynamiccast->ident_);
    edynamiccast->expr_->accept(this);
}

void Creator_x86::visitEMul(EMul *emul)
{
    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);
}

void Creator_x86::visitEAdd(EAdd *eadd)
{
    eadd->expr_1->accept(this);
    eadd->expr_2->accept(this);
    eadd->addop_->accept(this);
}

void Creator_x86::visitERel(ERel *erel)
{
    erel->expr_1->accept(this);
    erel->expr_2->accept(this);
    erel->relop_->accept(this);
}

void Creator_x86::visitEAnd(EAnd *eand)
{
    eand->expr_1->accept(this);
    eand->expr_2->accept(this);
}

void Creator_x86::visitEOr(EOr *eor)
{
    eor->expr_1->accept(this);
    eor->expr_2->accept(this);

}

void Creator_x86::visitPlus(Plus *plus)
{
    this->instruction_manager.add_on_stack();
}

void Creator_x86::visitMinus(Minus *minus)
{
    this->instruction_manager.sub_on_stack();
}

void Creator_x86::visitTimes(Times *times)
{
    this->instruction_manager.mul_on_stack();
}

void Creator_x86::visitDiv(Div *div)
{
    this->instruction_manager.div_on_stack();
}

void Creator_x86::visitMod(Mod *mod)
{
    this->instruction_manager.mod_on_stack();
}

void Creator_x86::visitLTH(LTH *lth)
{
}

void Creator_x86::visitLE(LE *le)
{
}

void Creator_x86::visitGTH(GTH *gth)
{
}

void Creator_x86::visitGE(GE *ge)
{
}

void Creator_x86::visitEQU(EQU *equ)
{
}

void Creator_x86::visitNE(NE *ne)
{
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
    for (ListArrayIndex::iterator i = listarrayindex->begin() ; i != listarrayindex->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListStructuredIdent(ListStructuredIdent* liststructuredident)
{
    for (ListStructuredIdent::iterator i = liststructuredident->begin() ; i != liststructuredident->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void Creator_x86::visitInteger(Integer x)
{
}

void Creator_x86::visitChar(Char x)
{
}

void Creator_x86::visitDouble(Double x)
{
}

void Creator_x86::visitString(String x)
{
}

void Creator_x86::visitIdent(Ident x)
{
}


} /* namespace backend */

