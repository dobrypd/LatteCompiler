/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include "Environment.h"
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

const int Creator_x86::words_per_var = 4; // 32 = word * 4
const int Creator_x86::object_fields_offset = 1;  // for v_table
const char* Creator_x86::self_name = "self";
const char* Creator_x86::v_table_name = "__vtable_ptr";
const char* Creator_x86::named_temp_on_stack_prefix = "#_TEMP__ON__STACK_#";

std::string Creator_x86::method_ident(std::string& class_name,
            std::string& method_name)
{
    return "_" + class_name + "." + method_name;
}

std::string Creator_x86::v_table_ident(std::string& class_name)
{
    return "_VTABLE." + class_name;
}

Creator_x86::Creator_x86(InstructionManager& instruction_manager,
        frontend::Environment& frontend_environment) :
        instruction_manager(instruction_manager),
        fr_env(frontend_environment),
        last_class_type("null"), next_label(1)
{
}


void Creator_x86::visitProg(Prog* t) {} //abstract class
void Creator_x86::visitTopDef(TopDef* t) {} //abstract class
void Creator_x86::visitArg(Arg* t) {} //abstract class
void Creator_x86::visitClsDef(ClsDef* t) {} //abstract class
void Creator_x86::visitBlk(Blk* t) {} //abstract class
void Creator_x86::visitStmt(Stmt* t) {} //abstract class
void Creator_x86::visitItem(Item* t) {} //abstract class
void Creator_x86::visitStructuredIdent(StructuredIdent* t) {} //abs
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
    this->last_class = ClsInfoPtr();
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
    this->last_class = this->fr_env.get_class(clsdefnoinher->ident_);
    this->last_class_type.ident_ = clsdefnoinher->ident_;
    assert(this->last_class);

    visitIdent(clsdefnoinher->ident_);

    this->instruction_manager.new_vtable(
            this->v_table_ident(clsdefnoinher->ident_),
            this->fr_env.get_class_methods_list(clsdefnoinher->ident_));

    clsdefnoinher->listclsdef_->accept(this);
}

void Creator_x86::visitClsDefInher(ClsDefInher *clsdefinher)
{
    this->last_class = this->fr_env.get_class(clsdefinher->ident_1);
    this->last_class_type.ident_ = clsdefinher->ident_1;
    assert(this->last_class);

    visitIdent(clsdefinher->ident_1);
    visitIdent(clsdefinher->ident_2);

    this->instruction_manager.new_vtable(
            this->v_table_ident(clsdefinher->ident_1),
            this->fr_env.get_class_methods_list(clsdefinher->ident_1));

    clsdefinher->listclsdef_->accept(this);

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
            this->last_class->ident, methoddef->ident_));
    this->instruction_manager.function_prologue();
    this->env.prepare();
    this->env.new_fun();
    // pointer to vtable'll be in in object - (get it by self!).
    this->env.add_variable(&(this->last_class_type), Creator_x86::self_name);

    methoddef->type_->accept(this);
    visitIdent(methoddef->ident_);
    // Add arguments.
    methoddef->listarg_->accept(this);

    // Code block.
    methoddef->blk_->accept(this);

    if (check_is<Void*>(methoddef->type_))
            this->instruction_manager.function_epilogue();

    this->env.back();
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
    if (ESP_diff > 0) this->instruction_manager.add_to_ESP(ESP_diff);
    // ESP will be changed by EBP in case of returning from function
}

void Creator_x86::visitStmDecl(StmDecl *stmdecl)
{
    stmdecl->type_->accept(this);
    this->declaration_type = stmdecl->type_;
    stmdecl->listitem_->accept(this);
}

void Creator_x86::visitStmAss(StmAss *stmass)
{
    int label_t = this->last_true_label = this->next_label++;
    int label_f = this->last_false_label = this->next_label++;

    stmass->expr_->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);

    stmass->liststructuredident_->accept(this);
    this->instruction_manager.pop_to_addr_from_ESI();
}

void Creator_x86::visitStmAssArr(StmAssArr *stmassarr)
{
    stmassarr->type_->accept(this);
    stmassarr->expr_->accept(this);

    this->instruction_manager.alloc_array(); // with len on stack

    stmassarr->liststructuredident_->accept(this);
    this->instruction_manager.pop_to_addr_from_ESI();
}

void Creator_x86::visitStmAssObj(StmAssObj* stmassobj)
{
    stmassobj->liststructuredident_->accept(this);
    stmassobj->type_->accept(this);

    std::string& cls_ident = (dynamic_cast<Class*>(stmassobj->type_))->ident_;
    this->instruction_manager.alloc_object(this->v_table_ident(cls_ident),
            this->fr_env.get_class_size(cls_ident));

    this->instruction_manager.pop_to_addr_from_ESI();
}

void Creator_x86::visitStmIncr(StmIncr *stmincr)
{
    stmincr->liststructuredident_->accept(this);
    this->instruction_manager.increment_in_ESI(1);
}

void Creator_x86::visitStmDecr(StmDecr *stmdecr)
{
    stmdecr->liststructuredident_->accept(this);
    this->instruction_manager.increment_in_ESI(-1);
}

void Creator_x86::visitStmRet(StmRet *stmret)
{
    int label_t = this->last_true_label = this->next_label++;
    int label_f = this->last_false_label = this->next_label++;
    stmret->expr_->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    this->instruction_manager.pop_to_EAX();  // Function calling conventions
    this->instruction_manager.function_epilogue();
}

void Creator_x86::visitStmVRet(StmVRet *stmvret)
{
    this->instruction_manager.function_epilogue();
}

void Creator_x86::visitStmCond(StmCond *stmcond)
{
    int label_t = this->last_true_label = this->next_label++;
    int label_f = this->last_false_label = this->next_label++;
    stmcond->expr_->accept(this);  // TODO: cond jumps optimization

    this->instruction_manager.new_block(label_t);
    stmcond->stmt_->accept(this);
    this->instruction_manager.new_block(label_f);
}

void Creator_x86::visitStmCondElse(StmCondElse *stmcondelse)
{
    int label_t = this->last_true_label = this->next_label++;
    int label_f = this->last_false_label =this->next_label++;
    int label_end = this->next_label++;
    stmcondelse->expr_->accept(this);  // TODO: cond jumps optimization

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
    int after_lbl = this->last_false_label = this->next_label++;
    int block_start = this->last_true_label = this->next_label++;

    this->instruction_manager.new_block(before_lbl);
    stmwhile->expr_->accept(this);
    this->instruction_manager.new_block(block_start);
    stmwhile->stmt_->accept(this);
    this->instruction_manager.jump(before_lbl);
    this->instruction_manager.new_block(after_lbl);
}

void Creator_x86::visitStmForeach(StmForeach *stmforeach)
{
    int start = this->next_label++;
    stmforeach->type_->accept(this);
    visitIdent(stmforeach->ident_);

    this->env.prepare();
    std::string ecx_var_name(Creator_x86::named_temp_on_stack_prefix);
    ecx_var_name += "foreach ECX";  // save ECX in case of neasted use
    std::string esi_var_name(Creator_x86::named_temp_on_stack_prefix);
    esi_var_name += "foreach ESI"; // IDENT reference

    // loop cond
    this->instruction_manager.push_ECX();
    this->env.add_variable(this->fr_env.global_int_type, ecx_var_name);

    // ptr to array
    stmforeach->liststructuredident_->accept(this);
    this->instruction_manager.push_ESI();
    this->env.add_variable(this->fr_env.global_int_type, esi_var_name);

    // size of array - 1 (iterations)
    this->instruction_manager.dereference_from_ESI_to_ECX_minus_1();

    // default value as variable from array (make place holder on stack)
    this->instruction_manager.push_literal(0);
    this->env.add_variable(stmforeach->type_, stmforeach->ident_);
    CompilerEnvironment::VarInfoPtr foreach_var_info = this->env.get_variable(
                        stmforeach->ident_);

    this->instruction_manager.new_block(start);
    CompilerEnvironment::VarInfoPtr var_info = this->env.get_variable(
            esi_var_name);
    // in first iteration will omit size of array
    this->instruction_manager.add_to_var(var_info->position, 1);
    this->instruction_manager.dereference_var_to_var(var_info->position,
            foreach_var_info->position);
    stmforeach->stmt_->accept(this);
    this->instruction_manager.loop(start);

    this->instruction_manager.var_to_ECX(
            this->env.get_variable(ecx_var_name)->position);

    this->env.back();

}

void Creator_x86::visitStmSExp(StmSExp *stmsexp)
{
    stmsexp->expr_->accept(this);

    if ((!this->e_was_rel) && (!check_is<Void*>(this->last_type)))
        // Remove from top of the stack
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
    int label_t = this->last_true_label = this->next_label++;
    int label_f = this->last_false_label = this->next_label++;
    stminit->expr_->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
}

void Creator_x86::visitStmInitArray(StmInitArray *stminitarray)
{
    visitIdent(stminitarray->ident_);
    stminitarray->type_->accept(this);
    stminitarray->expr_->accept(this);
    this->env.add_variable(this->declaration_type, stminitarray->ident_);
    this->instruction_manager.alloc_array();
}

void Creator_x86::visitStmInitObj(StmInitObj *stminitobj)
{
    visitIdent(stminitobj->ident_);
    stminitobj->type_->accept(this);
    this->env.add_variable(this->declaration_type, stminitobj->ident_);
    std::string& cls_ident = (dynamic_cast<Class*>(stminitobj->type_))->ident_;
    this->instruction_manager.alloc_object(this->v_table_ident(cls_ident),
            this->fr_env.get_class_size(cls_ident));
}


void Creator_x86::visit_ident(std::string& ident)
{
    if (this->ident_type == 0){
        CompilerEnvironment::VarInfoPtr var = this->env.get_variable(ident);
        if (!var) {
            // self object field
            std::string self = Creator_x86::self_name;
            CompilerEnvironment::VarInfoPtr self_var = this->env.get_variable(self);
            this->instruction_manager.add_to_ESI_val_address(self_var->position);
            this->ident_type = &this->last_class_type;
            var = this->fr_env.get_field(ident, this->last_class->ident);
            this->instruction_manager.dereference_ESI();
            this->instruction_manager.add_to_ESI(
                    (Creator_x86::object_fields_offset + var->position)
                    * Creator_x86::words_per_var);
            this->ident_type = var->type;
        } else {
            // this block variable
            this->ident_type = var->type;
            this->instruction_manager.add_to_ESI_val_address(var->position);
        }
    } else if (check_is<TType*>(this->ident_type)) {
        this->instruction_manager.dereference_ESI();
        this->ident_type = &(this->literal_int);
    } else {
        this->instruction_manager.dereference_ESI();
        Class* cls = dynamic_cast<Class*>(this->ident_type);
        frontend::Environment::VarInfoPtr var = this->fr_env.get_field(
                ident, cls->ident_);
        this->instruction_manager.add_to_ESI(
                (Creator_x86::object_fields_offset + var->position)
                * Creator_x86::words_per_var);
        this->ident_type = var->type;
    }
}

void Creator_x86::visitSingleIdent(SingleIdent* singleident)
{
    visitIdent(singleident->ident_);
    this->visit_ident(singleident->ident_);
}

void Creator_x86::visitArrayIdent(ArrayIdent * tableval)
{
    this->instruction_manager.push_ESI();

    tableval->expr_->accept(this);

    this->visit_ident(tableval->ident_);

    // First element is size, add it:
    this->instruction_manager.add_to_ESI(Creator_x86::words_per_var);
    this->instruction_manager.pop_add_to_ESI();
    this->instruction_manager.pop_ESI();
    this->ident_type = dynamic_cast<TType*>(this->ident_type)->type_;
}

void Creator_x86::visitSelfIdent(SelfIdent *selfident)
{
    std::string self = Creator_x86::self_name;
    CompilerEnvironment::VarInfoPtr v =
        this->env.get_variable(self);
    this->instruction_manager.add_to_ESI_val_address(v->position);

    this->ident_type = &this->last_class_type;
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
    this->last_type = this->ident_type;
    this->instruction_manager.dereference_ESI_to_stack();
    if (check_is<Bool*>(this->last_type)) {
        //this->instruction_manager.cmp_stack();
        this->instruction_manager.jump_if_0(this->last_false_label);
        this->instruction_manager.jump(this->last_true_label);
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

void Creator_x86::method_call(std::string& cls_ident, std::string method_ident)
{
    // GOT object address in ESI
    frontend::Environment::FunInfoPtr fun =
            this->fr_env.get_method(method_ident, cls_ident);
    // get vtable from object addresed in (ESI + vtable position).
    // call $(method_ident @ cls_ident) + fun->position
    // Arres to object is in ESI
    this->instruction_manager.method_call(fun->position);

}

void Creator_x86::function_call(std::string& ident,
        frontend::Environment::FunInfoPtr fun, ListExpr * arguments)
{
    if (fun->position < 0) {
        arguments->accept(this);
        this->instruction_manager.function_call(ident);

        // If extern remove arguments from stack
        if (fun->is_extern) {
            this->instruction_manager.add_to_ESP(fun->arguments.size());
        }
    }
    else
    {
        std::string self = Creator_x86::self_name;
        CompilerEnvironment::VarInfoPtr v = this->env.get_variable(self);
        this->instruction_manager.add_to_ESI_val_address(v->position);
        this->instruction_manager.push_ESI();  //SELF
        arguments->accept(this);
        this->method_call(this->last_class_type.ident_, ident);
    }

}

void Creator_x86::visitEApp(EApp *eapp)
{
    visitIdent(eapp->ident_);
    frontend::Environment::FunInfoPtr fun;
    if (this->last_class)
        fun = this->fr_env.get_method(eapp->ident_, this->last_class->ident);
    if (!fun) fun = this->fr_env.get_function(eapp->ident_);

    this->function_call(eapp->ident_, fun, eapp->listexpr_);

    this->last_type = fun->ret_type;
    this->e_was_rel = false;
    this->instruction_manager.push_EAX();
}

void Creator_x86::visitEMethodApp(EMethodApp *emethodapp)
{
    this->ident_type = 0;
    for (ListStructuredIdent::iterator
            i = emethodapp->liststructuredident_->begin();
            i != emethodapp->liststructuredident_->end() - 1; ++i)
    {
        (*i)->accept(this);
    }

    // Do not accept method name.
    SingleIdent* sid = dynamic_cast<SingleIdent*>(
            emethodapp->liststructuredident_->back());

    Class* cls_type = dynamic_cast<Class*>(this->ident_type);
    frontend::Environment::FunInfoPtr fun = this->fr_env.get_method(sid->ident_,
                cls_type->ident_);

    this->instruction_manager.push_ESI();  // object
    emethodapp->listexpr_->accept(this);  // arguments
    this->method_call(cls_type->ident_, sid->ident_);

    this->last_type = fun->ret_type;
    this->e_was_rel = false;
    this->instruction_manager.push_EAX();
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
    int label_t = this->last_true_label = this->next_label++;
    int label_f = this->last_false_label = this->next_label++;
    erel->expr_1->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);

    label_t = this->last_true_label = this->next_label++;
    label_f = this->last_false_label = this->next_label++;
    erel->expr_2->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);

    this->instruction_manager.cmp_stack();
    erel->relop_->accept(this);
    // TODO: Jumps optimization;
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

    int label_t = this->last_true_label = this->next_label++;
    int label_f = this->last_false_label = this->next_label++;
    eand->expr_1->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    this->instruction_manager.jump_if_0(this_false_label);

    label_t = this->last_true_label = this->next_label++;
    label_f = this->last_false_label = this->next_label++;
    eand->expr_2->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    // TODO: Jumps optimization;
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

    int label_t = this->last_true_label = this->next_label++;
    int label_f = this->last_false_label = this->next_label++;
    eor->expr_1->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    this->instruction_manager.jump_if_not0(this_true_label);

    label_t = this->last_true_label = this->next_label++;
    label_f = this->last_false_label = this->next_label++;
    eor->expr_2->accept(this);
    if (this->e_was_rel) this->bool_expr_to_stack(label_t, label_f);
    // TODO: Jumps optimization;
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

void Creator_x86::visitListStructuredIdent(ListStructuredIdent* liststructuredident)
{
    this->ident_type = 0;
    for (ListStructuredIdent::iterator i = liststructuredident->begin() ; i != liststructuredident->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void Creator_x86::visitListExpr(ListExpr* listexpr)
{
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        int label_t = this->last_true_label = this->next_label++;
        int label_f = this->last_false_label = this->next_label++;
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

