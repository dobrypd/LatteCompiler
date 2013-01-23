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

std::string FunctionLoader::default_cls_name("def_cls_name");

FunctionLoader::FunctionLoader(ErrorHandler& error_handler,
        Environment& env)
    : error_handler(error_handler), env(env),
      loaded_classes_no_ext_and_fun(false),
      last_cls_name(FunctionLoader::default_cls_name)
{
}


void FunctionLoader::check(Visitable* v)
{
    v->accept(this);
    this->loaded_classes_no_ext_and_fun = true;
    v->accept(this);

    /*
     * check if has main
     */
    std::string main_idnt("main");
    Environment::FunInfoPtr fun_ptr = this->env.get_function(main_idnt);
    // Check if function exists.
    if ((!fun_ptr)
            || (!fun_ptr->arguments.empty())
            || (!check_is<Int*>(fun_ptr->ret_type)))
    {
        std::string msg = "function `";
        msg += main_idnt;
        msg += "` with 0 arguments and int return type does not exist.";
        this->error_handler.error(1, msg);
        return;
    }

    if (debug)
    {
        // Log what was added and checked.
        std::cout << "Function loader finished with environment:" << std::endl;
        std::cout << std::endl;
        std::cout << "Functions:" << std::endl;
        for (std::map<std::string, Environment::FunInfoPtr>::iterator it =
                this->env.get_env_f_begin(); it != this->env.get_env_f_end();
                it++) {
            std::cout << "\t" << it->first << "(";
            for(std::vector<Environment::VarInfoPtr>::iterator it_arg =
                    it->second->arguments.begin();
                    it_arg != it->second->arguments.end(); it_arg++){
                std::cout << type_pretty_print((*it_arg)->type) << ",";
            }

            std::cout << ")" << std::endl;
        }
        std::cout << "Classes:" << std::endl;
        for (std::map<std::string, Environment::ClsInfoPtr>::iterator it =
                this->env.get_env_cls_begin(); it != this->env.get_env_cls_end();
                it++) {
            std::cout << "\t" << it->first << " {" << std::endl;
            std::cout << "\t\tFields:" << std::endl;
            for(std::vector<std::pair<std::string, Environment::VarInfoPtr> >::iterator it_fields =
                    it->second->fields.begin();
                    it_fields != it->second->fields.end(); it_fields++){
                std::cout << "\t\t\t";
                std::cout << type_pretty_print((*it_fields).second->type);
                std::cout << " " << (*it_fields).first;
                std::cout << std::endl;
            }
            std::cout << "\t\tMethods:" << std::endl;
            for(std::vector<std::pair<std::string, Environment::FunInfoPtr> >::iterator it_method =
                    it->second->methods.begin();
                    it_method != it->second->methods.end(); it_method++){
                std::cout << "\t\t\t";
                std::cout << type_pretty_print((*it_method).second->ret_type);
                std::cout << " " << (*it_method).first << "(";
                for(std::vector<Environment::VarInfoPtr>::iterator it_arg =
                        (*it_method).second->arguments.begin();
                        it_arg != (*it_method).second->arguments.end(); it_arg++){
                    std::cout << type_pretty_print((*it_arg)->type) << ",";
                }
                std::cout << ")" << std::endl;
            }
            std::cout << "\t}" << std::endl;
        }
    }
}

void FunctionLoader::visitProg(Prog* t) {} //abstract class
void FunctionLoader::visitTopDef(TopDef* t) {} //abstract class
void FunctionLoader::visitArg(Arg* t) {} //abstract class
void FunctionLoader::visitClsDef(ClsDef* t) {} //abstract class
void FunctionLoader::visitBlk(Blk* t) {} //abstract class
void FunctionLoader::visitStmt(Stmt* t) {} //abstract class
void FunctionLoader::visitItem(Item* t) {} //abstract class
void FunctionLoader::visitStructuredIdent(StructuredIdent* t) {} //abstract class
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
    if (!this->loaded_classes_no_ext_and_fun) {
        if (this->env.can_add_function(fndef->ident_))
            this->env.add_function(fndef, false);
        else
        {
            std::string comunicate;
            comunicate += "function `";
            comunicate += fndef->ident_;
            comunicate += "` already declared.";
            this->error_handler.error(fndef->type_->line_number, comunicate);
        }
    }
}

void FunctionLoader::visitClsDefNoInher(ClsDefNoInher *clsdefnoinher)
{
    if (!this->loaded_classes_no_ext_and_fun) {
        if (this->env.can_add_class(clsdefnoinher->ident_)) {
            this->env.add_class(clsdefnoinher->ident_);
            this->last_cls_name = clsdefnoinher->ident_;
            clsdefnoinher->listclsdef_->accept(this);
        } else {
            std::string comunicate;
            comunicate += "class `";
            comunicate += clsdefnoinher->ident_;
            comunicate += "` already declared.";
            this->error_handler.error(clsdefnoinher->line_number, comunicate);
        }
    }
}

void FunctionLoader::visitClsDefInher(ClsDefInher *clsdefinher)
{
    if (this->loaded_classes_no_ext_and_fun) {
        if (this->env.can_add_class(clsdefinher->ident_1, clsdefinher->ident_2)) {
            this->env.add_class(clsdefinher->ident_1, clsdefinher->ident_2);
            this->last_cls_name = clsdefinher->ident_1;
            clsdefinher->listclsdef_->accept(this);
        } else {
            std::string comunicate;
            comunicate += "class `";
            comunicate += clsdefinher->ident_1;
            comunicate += "` already declared or inheriting from not existing class `";
            comunicate += clsdefinher->ident_2;
            comunicate += "`.";
            this->error_handler.error(clsdefinher->line_number, comunicate);
        }
    }
}

void FunctionLoader::visitArgument(Argument* argument) { }

void FunctionLoader::visitMethodDef(MethodDef *methoddef)
{
    Environment::ClsInfoPtr cls = this->env.get_class(this->last_cls_name);
    for (Environment::lat_class::methods_t::iterator it = cls->methods.begin();
            it != cls->methods.end(); it++) {
        if (it->first == methoddef->ident_) {
            std::string comunicate;
            comunicate += "function `";
            comunicate += methoddef->ident_;
            comunicate += "` already declared in class `";
            comunicate += this->last_cls_name;
            comunicate += "`.";
            this->error_handler.error(methoddef->type_->line_number, comunicate);
            return;
        }
    }

    this->env.add_method_to_cls(this->last_cls_name, methoddef);
}

void FunctionLoader::visitFieldDef(FieldDef *fielddef)
{
    Environment::ClsInfoPtr cls = this->env.get_class(this->last_cls_name);

    while(cls) {
        for (Environment::lat_class::fields_t::iterator it = cls->fields.begin();
                it != cls->fields.end(); it++) {
            if (it->first == fielddef->ident_) {
                std::string comunicate;
                comunicate += "function `";
                comunicate += fielddef->ident_;
                comunicate += "` already declared in class `";
                comunicate += this->last_cls_name;
                comunicate += "`.";
                this->error_handler.error(fielddef->type_->line_number, comunicate);
                return;
            }
        }
        cls = cls->lat_cls_parent;
    }

    this->env.add_field_to_cls(this->last_cls_name, fielddef->type_,
            fielddef->ident_);
}

void FunctionLoader::visitStmBlock(StmBlock* stmblock)
{}

void FunctionLoader::visitStmEmpty(StmEmpty* stmempty)
{}

void FunctionLoader::visitStmBStmt(StmBStmt* stmbstmt)
{}

void FunctionLoader::visitStmDecl(StmDecl* stmdecl)
{}

void FunctionLoader::visitStmAss(StmAss* stmass)
{}

void FunctionLoader::visitStmAssArr(StmAssArr *stmassarr)
{}

void FunctionLoader::visitStmAssObj(StmAssObj *stmassobj)
{}

void FunctionLoader::visitStmIncr(StmIncr *stmincr)
{}

void FunctionLoader::visitStmDecr(StmDecr *stmdecr)
{}

void FunctionLoader::visitStmRet(StmRet* stmret)
{}

void FunctionLoader::visitStmVRet(StmVRet* stmvret)
{}

void FunctionLoader::visitStmCond(StmCond* stmcond)
{}

void FunctionLoader::visitStmCondElse(StmCondElse* stmcondelse)
{}

void FunctionLoader::visitStmWhile(StmWhile* stmwhile)
{}

void FunctionLoader::visitStmForeach(StmForeach *stmforeach)
{}

void FunctionLoader::visitStmSExp(StmSExp* stmsexp)
{}

void FunctionLoader::visitStmNoInit(StmNoInit* stmnoinit)
{}

void FunctionLoader::visitStmInit(StmInit* stminit)
{}

void FunctionLoader::visitStmInitArray(StmInitArray *stminitarray)
{}

void FunctionLoader::visitStmInitObj(StmInitObj *stminitobj)
{}

void FunctionLoader::visitSingleIdent(SingleIdent *singleident)
{}

void FunctionLoader::visitObjectIdent(ObjectIdent *objectident)
{}

void FunctionLoader::visitTableVal(TableVal *tableval)
{}

void FunctionLoader::visitSelfIdent(SelfIdent *selfident)
{}

void FunctionLoader::visitClass(Class *_class)
{}

void FunctionLoader::visitInt(Int* integer)
{}

void FunctionLoader::visitStr(Str* str)
{}

void FunctionLoader::visitBool(Bool* boolean)
{}

void FunctionLoader::visitVoid(Void* void_field)
{}

void FunctionLoader::visitTType(TType *ttype)
{}

void FunctionLoader::visitEVar(EVar *evar)
{}

void FunctionLoader::visitELitInt(ELitInt* elitint)
{}

void FunctionLoader::visitELitTrue(ELitTrue* elittrue)
{}

void FunctionLoader::visitELitFalse(ELitFalse* elitfalse)
{}

void FunctionLoader::visitELitNull(ELitNull *elitnull)
{}

void FunctionLoader::visitEApp(EApp *eapp)
{}

void FunctionLoader::visitEMethodApp(EMethodApp *emethodapp)
{}

void FunctionLoader::visitEString(EString* estring)
{}

void FunctionLoader::visitNeg(Neg* neg)
{}

void FunctionLoader::visitNot(Not* not_field)
{}

void FunctionLoader::visitEDynamicCast(EDynamicCast *edynamiccast)
{}

void FunctionLoader::visitEMul(EMul* emul)
{}

void FunctionLoader::visitEAdd(EAdd* eadd)
{}

void FunctionLoader::visitERel(ERel* erel)
{}

void FunctionLoader::visitEAnd(EAnd* eand)
{}

void FunctionLoader::visitEOr(EOr* eor)
{}

void FunctionLoader::visitPlus(Plus* plus)
{}

void FunctionLoader::visitMinus(Minus* minus)
{}

void FunctionLoader::visitTimes(Times* times)
{}

void FunctionLoader::visitDiv(Div* div)
{}

void FunctionLoader::visitMod(Mod* mod)
{}

void FunctionLoader::visitLTH(LTH* lth)
{}

void FunctionLoader::visitLE(LE* le)
{}

void FunctionLoader::visitGTH(GTH* gth)
{}

void FunctionLoader::visitGE(GE* ge)
{}

void FunctionLoader::visitEQU(EQU* equ)
{}

void FunctionLoader::visitNE(NE* ne)
{}


void FunctionLoader::visitListTopDef(ListTopDef* listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListArg(ListArg* listarg)
{}

void FunctionLoader::visitListClsDef(ListClsDef* listclsdef)
{
    for (ListClsDef::iterator i = listclsdef->begin() ; i != listclsdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListStmt(ListStmt* liststmt)
{}

void FunctionLoader::visitListItem(ListItem* listitem)
{}

void FunctionLoader::visitListStructuredIdent(ListStructuredIdent* liststructuredident)
{}

void FunctionLoader::visitListExpr(ListExpr* listexpr)
{}


void FunctionLoader::visitInteger(Integer x)
{}

void FunctionLoader::visitChar(Char x)
{}

void FunctionLoader::visitDouble(Double x)
{}

void FunctionLoader::visitString(String x)
{}

void FunctionLoader::visitIdent(Ident x)
{}

} /* namespace frontend */
