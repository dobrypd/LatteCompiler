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


void Creator_x86::bool_expr_to_stack(int label_t, int label_f)
{
    int label_end = this->next_label++;
    this->instruction_manager.new_block(label_t);
    this->instruction_manager.push_literal(1);
    this->instruction_manager.jump(label_end);
    this->instruction_manager.new_block(label_f);
    this->instruction_manager.push_literal(0);
    this->instruction_manager.new_block(label_end);
}

const char* Creator_x86::self_name = "self";

std::string Creator_x86::method_ident(std::string& class_name,
            std::string& method_name)
{
    return "_" + class_name + "@@" + method_name;
}

Creator_x86::Creator_x86(InstructionManager& instruction_manager,
        frontend::Environment& frontend_environment) :
        instruction_manager(instruction_manager),
        fr_env(frontend_environment),
        next_label(1)
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
    this->e_was_rel = false;
    program->listtopdef_->accept(this);
}

void Creator_x86::visitFnDef(FnDef *fndef)
{
    this->instruction_manager.new_function_block(fndef->ident_);
    this->instruction_manager.function_prologue();
    this->env.prepare();
    this->env.new_fun();
    fndef->type_->accept(this);
    visitIdent(fndef->ident_);
    fndef->listarg_->accept(this);
    fndef->blk_->accept(this);
    if (check_is<Void*>(fndef->type_))
        this->instruction_manager.function_epilogue(); // TODO: want it?
    this->env.back(); // ESP will be changed by EBP
}

void Creator_x86::visitClsDefNoInher(ClsDefNoInher *clsdefnoinher)
{
    this->last_class_name = clsdefnoinher->ident_;
    visitIdent(clsdefnoinher->ident_);
    clsdefnoinher->listclsdef_->accept(this);

    // TODO: Create vtable and constructor
}

void Creator_x86::visitClsDefInher(ClsDefInher *clsdefinher)
{
    this->last_class_name = clsdefinher->ident_1;
    visitIdent(clsdefinher->ident_1);
    visitIdent(clsdefinher->ident_2);
    clsdefinher->listclsdef_->accept(this);

    // TODO: Create vtable and constructor

}

void Creator_x86::visitArgument(Argument *argument)
{
    this->env.add_variable(argument->type_, argument->ident_);

    argument->type_->accept(this);
    visitIdent(argument->ident_);
}

void Creator_x86::visitMethodDef(MethodDef *methoddef)
{
    this->instruction_manager.new_function_block(Creator_x86::method_ident(
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
    if (check_is<Void*>(methoddef->type_))
            this->instruction_manager.function_epilogue(); // TODO: want it?
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
    int label_t = this->next_label++;
    int label_f = this->next_label++;

    stmass->liststructuredident_->accept(this);

    this->last_true_label = label_t;
    this->last_false_label = label_f;
    stmass->expr_->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    this->instruction_manager.pop_to_addr_from_ESI();
}

void Creator_x86::visitStmAssArr(StmAssArr *stmassarr)
{
    this->current_var_on_stack = true;
    stmassarr->liststructuredident_->accept(this);

    stmassarr->type_->accept(this);
    stmassarr->expr_->accept(this);

    this->instruction_manager.alloc_array(stmassarr->type_); // with len on stack
    this->instruction_manager.pop_to_addr_from_ESI();
}

void Creator_x86::visitStmAssObj(StmAssObj* stmassobj)
{
    this->current_var_on_stack = true;

    stmassobj->liststructuredident_->accept(this);
    stmassobj->type_->accept(this);

    this->instruction_manager.alloc_object(stmassobj->type_);
    this->instruction_manager.pop_to_addr_from_ESI();
}

void Creator_x86::visitStmIncr(StmIncr *stmincr)
{
    this->current_var_on_stack = true;
    stmincr->liststructuredident_->accept(this);
    this->instruction_manager.increment_in_ESI(1);
}

void Creator_x86::visitStmDecr(StmDecr *stmdecr)
{
    this->current_var_on_stack = true;
    stmdecr->liststructuredident_->accept(this);
    this->instruction_manager.increment_in_ESI(-1);
}

void Creator_x86::visitStmRet(StmRet *stmret)
{
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    stmret->expr_->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    this->instruction_manager.top_to_EAX();
    this->instruction_manager.function_epilogue();
}

void Creator_x86::visitStmVRet(StmVRet *stmvret)
{
    this->instruction_manager.function_epilogue();
}

void Creator_x86::visitStmCond(StmCond *stmcond)
{
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    stmcond->expr_->accept(this);
    this->instruction_manager.new_block(label_t);
    stmcond->stmt_->accept(this);
    this->instruction_manager.new_block(label_f);
}

void Creator_x86::visitStmCondElse(StmCondElse *stmcondelse)
{
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    int label_end = this->next_label++;
    this->last_false_label = label_f;
    this->last_true_label = label_t;
    stmcondelse->expr_->accept(this);
    this->instruction_manager.new_block(label_t);
    stmcondelse->stmt_1->accept(this);
    this->instruction_manager.jump(label_end);
    this->instruction_manager.new_block(label_f);
    stmcondelse->stmt_2->accept(this);
    this->instruction_manager.new_block(label_end);
}

void Creator_x86::visitStmWhile(StmWhile *stmwhile)
{
    int before_lbl = this->next_label++;
    int after_lbl = this->next_label++;
    int block_start = this->next_label++;
    this->instruction_manager.new_block(before_lbl);
    this->last_false_label = after_lbl;
    this->last_true_label = block_start;
    stmwhile->expr_->accept(this);
    this->instruction_manager.new_block(block_start);
    stmwhile->stmt_->accept(this);
    this->instruction_manager.jump(before_lbl);
    this->instruction_manager.new_block(after_lbl);
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

    if ((!this->e_was_rel) && (!check_is<Void*>(this->last_type)))
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
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    stminit->expr_->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
}

void Creator_x86::visitStmInitArray(StmInitArray *stminitarray)
{
    visitIdent(stminitarray->ident_);
    stminitarray->type_->accept(this);
    this->env.add_array(stminitarray->type_, stminitarray->ident_);
    stminitarray->expr_->accept(this);
    this->instruction_manager.alloc_array(stminitarray->type_);
    // with len on stack
}

void Creator_x86::visitStmInitObj(StmInitObj *stminitobj)
{
    visitIdent(stminitobj->ident_);
    stminitobj->type_->accept(this);
    frontend::Environment::ClsInfoPtr cls =
            this->fr_env.get_class(
                    (dynamic_cast<Class*>(stminitobj->type_)->ident_));
    this->env.add_obj(stminitobj->ident_, cls);
    this->instruction_manager.alloc_object(stminitobj->type_);
}

void Creator_x86::visitSingleIdent(SingleIdent* singleident)
{
    visitIdent(singleident->ident_);

    if (this->current_var_on_stack) {
        CompilerEnvironment::VarInfoPtr v =
                this->env.get_variable(singleident->ident_);
        this->instruction_manager.add_to_ESI_val_address(v->position);
        this->current_var_type = v->type;
        this->current_var_on_stack = false;
    } else {
        this->instruction_manager.dereference_ESI();
        if (check_is<Class*>(this->current_var_type)) {
            int field_pos = this->fr_env.get_field_position(singleident->ident_,
                    (dynamic_cast<Class*>(this->current_var_type))->ident_);
            frontend::Environment::VarInfoPtr field_info =
                    this->fr_env.find_field(singleident->ident_,
                    (dynamic_cast<Class*>(this->current_var_type))->ident_);
            this->instruction_manager.add_to_ESI(field_pos * 4);
        } else if (check_is<TType*>(this->current_var_type)){
            this->current_var_type = this->fr_env.global_int_type;
        } else {
            if (debug) std::cerr << "Wrong ident list!" << std::endl;
        }
        this->current_var_on_stack = false;
    }
}

void Creator_x86::visitTableVal(TableVal *tableval)
{
    visitIdent(tableval->ident_);
    // In current version only one dimension arrays
    tableval->listarrayindex_->accept(this);
    // because in first value of array is length
    this->instruction_manager.increment_ESI(1);
    this->instruction_manager.pop_add_to_ESI();
}

void Creator_x86::visitSelfIdent(SelfIdent *selfident)
{
    /* self should be always first if not - do nothing with rest values */
    std::string self = Creator_x86::self_name;
    CompilerEnvironment::VarInfoPtr v =
        this->env.get_variable(self);
    this->instruction_manager.add_to_ESI_val_address(v->position);
    this->current_var_type = v->type;
    this->current_var_on_stack = false;
}

void Creator_x86::visitExprIndex(ExprIndex *exprindex)
{
    exprindex->expr_->accept(this);
}

void Creator_x86::visitClass(Class *_class)
{
    visitIdent(_class->ident_);
}

void Creator_x86::visitInt(Int *_int)
{
}

void Creator_x86::visitStr(Str *str)
{
}

void Creator_x86::visitBool(Bool *_bool)
{
}

void Creator_x86::visitVoid(Void *_void)
{
}

void Creator_x86::visitTType(TType *ttype)
{
    ttype->type_->accept(this);
}

void Creator_x86::visitEVar(EVar *evar)
{
    evar->liststructuredident_->accept(this);
    this->instruction_manager.dereference_ESI_to_stack();
    if (check_is<Bool*>(this->last_type)) {
        this->instruction_manager.cmp_stack();
        this->instruction_manager.jump_if(InstructionManager::NE, this->last_true_label);
        this->instruction_manager.jump(this->last_false_label);
        this->e_was_rel = true;
    } else {
        this->e_was_rel = false;
    }
}

void Creator_x86::visitELitInt(ELitInt *elitint)
{
    visitInteger(elitint->integer_);
    this->instruction_manager.push_literal(elitint->integer_);
    this->last_type = &(this->literal_int);
    this->e_was_rel = false;
}

void Creator_x86::visitELitTrue(ELitTrue *elittrue)
{
    this->instruction_manager.jump(this->last_true_label);
    this->e_was_rel = true;
    this->last_type = &(this->literal_bool);
}

void Creator_x86::visitELitFalse(ELitFalse *elitfalse)
{
    this->instruction_manager.jump(this->last_false_label);
    this->e_was_rel = true;
    this->last_type = &(this->literal_bool);
}

void Creator_x86::visitELitNull(ELitNull *elitnull)
{
    this->instruction_manager.push_literal(0);
    this->last_type = &(this->literal_int);
    this->e_was_rel = false;
}

void Creator_x86::visitEApp(EApp *eapp)
{
    // arguments
    // TODO: vtable calls
    // TODO: last in this list is function application.
    eapp->liststructuredident_->accept(this); // TODO: function should be last
    visitIdent(eapp->ident_);
    frontend::Environment::ClsInfoPtr cls =
            this->fr_env.get_class((dynamic_cast<Class*>(this->last_type))
                    ->ident_);
    eapp->listexpr_->accept(this);

    //Block::instr_ptr_t call(new x86_Call(get_from_vtable(eapp->liststructuredident_)));
    //this->instruction_manager.add(call);
}

void Creator_x86::visitEString(EString *estring)
{
    visitString(estring->string_);
    this->instruction_manager.add_const_string(estring->string_);
    this->last_type = &(this->literal_string);
    this->e_was_rel = false;
}

void Creator_x86::visitNeg(Neg *neg)
{
    neg->expr_->accept(this);
    this->instruction_manager.neg_on_top();
}

void Creator_x86::visitNot(Not *_not)
{
    int tmp = this->last_true_label;
    this->last_true_label = this->last_false_label;
    this->last_false_label = tmp;
    _not->expr_->accept(this);
    this->e_was_rel = true;
}

void Creator_x86::visitEDynamicCast(EDynamicCast *edynamiccast)
{
    visitIdent(edynamiccast->ident_);
    edynamiccast->expr_->accept(this);
    this->fr_env.global_cls_type->ident_ = edynamiccast->ident_;
    this->last_type = this->fr_env.global_cls_type;
}

void Creator_x86::visitEMul(EMul *emul)
{
    emul->expr_1->accept(this);
    emul->expr_2->accept(this);
    emul->mulop_->accept(this);
}

void Creator_x86::visitEAdd(EAdd *eadd)
{
    eadd->expr_1->accept(this);
    eadd->expr_2->accept(this);
    if (check_is<Str *>(this->last_type))
        this->instruction_manager.concat_str_on_stack();
    else
        eadd->addop_->accept(this);
}

void Creator_x86::visitERel(ERel *erel)
{
    int this_last_f_label = this->last_false_label;
    int this_last_t_label = this->last_true_label;
    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    erel->expr_1->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    label_t = this->next_label++;
    label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    erel->expr_2->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);

    if (check_is<Str*>(this->last_type)){
        this->instruction_manager.compare_strings_on_stack();
        // PUT 2 VALUES ON STACK IF EQUAL 0, 0, otherise 0, 1
    }

    this->instruction_manager.cmp_stack();
    erel->relop_->accept(this);
    // TODO: Optimize it!
    this->instruction_manager.jump_if(this->last_rel, this_last_t_label);
    this->instruction_manager.jump(this_last_f_label);

    this->last_type = &(this->literal_bool);
    this->e_was_rel = true;
    this->last_false_label = this_last_f_label;
    this->last_true_label = this_last_t_label;
}

void Creator_x86::visitEAnd(EAnd *eand)
{
    int this_true_label = this->last_true_label;
    int this_false_label = this->last_false_label;

    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    eand->expr_1->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    this->instruction_manager.jump_if_0(this_false_label);

    label_t = this->next_label++;
    label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    eand->expr_2->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);

    this->instruction_manager.jump_if_0(this_false_label);
    this->instruction_manager.jump(this_true_label);

    this->last_true_label = this_true_label;
    this->last_false_label = this_false_label;
    this->e_was_rel = true;
}

void Creator_x86::visitEOr(EOr *eor)
{
    int this_true_label = this->last_true_label;
    int this_false_label = this->last_false_label;

    int label_t = this->next_label++;
    int label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    eor->expr_1->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    this->instruction_manager.jump_if_not0(this_true_label);

    label_t = this->next_label++;
    label_f = this->next_label++;
    this->last_true_label = label_t;
    this->last_false_label = label_f;
    eor->expr_2->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);

    this->instruction_manager.jump_if_not0(this_false_label);
    this->instruction_manager.jump(this_true_label);

    this->last_true_label = this_true_label;
    this->last_false_label = this_false_label;
    this->e_was_rel = true;

}

void Creator_x86::visitPlus(Plus *plus)
{
    if (!check_is<Str *>(this->last_type)){
        this->instruction_manager.add_on_stack();
    }
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
    this->last_rel = InstructionManager::LTH;
}

void Creator_x86::visitLE(LE *le)
{
    this->last_rel = InstructionManager::LE;
}

void Creator_x86::visitGTH(GTH *gth)
{
    this->last_rel = InstructionManager::GTH;
}

void Creator_x86::visitGE(GE *ge)
{
    this->last_rel = InstructionManager::GE;
}

void Creator_x86::visitEQU(EQU *equ)
{
    this->last_rel = InstructionManager::EQU;
}

void Creator_x86::visitNE(NE *ne)
{
    this->last_rel = InstructionManager::NE;
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
        int label_t = this->next_label++;
        int label_f = this->next_label++;
        this->last_true_label = label_t;
        this->last_false_label = label_f;
        (*i)->accept(this);
        if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
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

