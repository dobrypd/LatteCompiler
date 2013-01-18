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

FunctionLoader::FunctionLoader(ErrorHandler& error_handler,
        Environment& env)
    : error_handler(error_handler), env(env)
{
}


void FunctionLoader::check(Visitable* v)
{
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
        std::cout << "Variables: (shouldn't be any):" << std::endl;
        for (std::vector<Environment::MapPtr>::iterator m_it =
                this->env.get_env_v_it_begin();
                m_it != this->env.get_env_v_it_end();
                m_it++) {
            for (Environment::MapType::iterator it = (*m_it)->begin();
                    it != (*m_it)->end(); it++) {
                std::cout << type_pretty_print(it->second->type) << " "
                        << it->first << std::endl;
            }

        }
        std::cout << "Functions:" << std::endl;
        for (std::map<std::string, Environment::FunInfoPtr>::iterator it =
                this->env.get_env_f_begin(); it != this->env.get_env_f_end();
                it++) {
            std::cout << it->first << "(";
            for(std::vector<Environment::VarInfoPtr>::iterator it_arg =
                    it->second->arguments.begin();
                    it_arg != it->second->arguments.end(); it_arg++){
                std::cout << type_pretty_print((*it_arg)->type);
            }

            std::cout << ")" << std::endl;
        }
        std::cout << "Classes:" << std::endl;
        for (std::map<std::string, Environment::ClsInfoPtr>::iterator it =
                this->env.get_env_cls_begin(); it != this->env.get_env_cls_end();
                it++) {
            std::cout << it->first << "{" << std::endl;
            std::cout << "\tFields:" << std::endl;
            for(std::vector<std::pair<std::string, Environment::VarInfoPtr> >::iterator it_fields =
                    it->second->fields.begin();
                    it_fields != it->second->fields.end(); it_fields++){
                std::cout << "\t";
                std::cout << type_pretty_print((*it_fields).second->type);
                std::cout << " " << (*it_fields).first;
                std::cout << std::endl;
            }

            std::cout << "}" << std::endl;
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
void FunctionLoader::visitArrayIndex(ArrayIndex* t) {} //abstract class
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

void FunctionLoader::visitClsDefNoInher(ClsDefNoInher *clsdefnoinher)
{
    /* Code For ClsDefNoInher Goes Here */
    /* Latte++ */

    visitIdent(clsdefnoinher->ident_);
    clsdefnoinher->listclsdef_->accept(this);

}

void FunctionLoader::visitClsDefInher(ClsDefInher *clsdefinher)
{
    /* Code For ClsDefInher Goes Here */
    /* Latte++ */

    visitIdent(clsdefinher->ident_1);
    visitIdent(clsdefinher->ident_2);
    clsdefinher->listclsdef_->accept(this);

}

void FunctionLoader::visitArgument(Argument* argument)
{
    argument->type_->accept(this);
    visitIdent(argument->ident_);

}

void FunctionLoader::visitMethodDef(MethodDef *methoddef)
{
    /* Code For MethodDef Goes Here */
    /* Latte++ */

    methoddef->type_->accept(this);
    visitIdent(methoddef->ident_);
    methoddef->listarg_->accept(this);
    methoddef->blk_->accept(this);

}

void FunctionLoader::visitFieldDef(FieldDef *fielddef)
{
    /* Code For FieldDef Goes Here */
    /* Latte++ */

    fielddef->type_->accept(this);
    visitIdent(fielddef->ident_);

}

void FunctionLoader::visitStmBlock(StmBlock* stmblock)
{
    stmblock->liststmt_->accept(this);
}

void FunctionLoader::visitStmEmpty(StmEmpty* stmempty)
{
}

void FunctionLoader::visitStmBStmt(StmBStmt* stmbstmt)
{
    stmbstmt->blk_->accept(this);
}

void FunctionLoader::visitStmDecl(StmDecl* stmdecl)
{
    stmdecl->type_->accept(this);
    stmdecl->listitem_->accept(this);
}

void FunctionLoader::visitStmAss(StmAss* stmass)
{
    stmass->liststructuredident_->accept(this);
    stmass->expr_->accept(this);
}

void FunctionLoader::visitStmAssArr(StmAssArr *stmassarr)
{
    /* Code For StmAssArr Goes Here */
    /* Latte++ */

    stmassarr->liststructuredident_->accept(this);
    stmassarr->type_->accept(this);
    stmassarr->expr_->accept(this);

}

void FunctionLoader::visitStmAssObj(StmAssObj *stmassobj)
{
    /* Code For StmAssObj Goes Here */
    /* Latte++ */

    stmassobj->liststructuredident_->accept(this);
    stmassobj->type_->accept(this);

}

void FunctionLoader::visitStmIncr(StmIncr *stmincr)
{
    /* Code For StmIncr Goes Here */
    /* Latte++ */

    stmincr->liststructuredident_->accept(this);

}

void FunctionLoader::visitStmDecr(StmDecr *stmdecr)
{
    /* Code For StmDecr Goes Here */
    /* Latte++ */

    stmdecr->liststructuredident_->accept(this);

}

void FunctionLoader::visitStmRet(StmRet* stmret)
{
    stmret->expr_->accept(this);
}

void FunctionLoader::visitStmVRet(StmVRet* stmvret)
{
}

void FunctionLoader::visitStmCond(StmCond* stmcond)
{
    stmcond->expr_->accept(this);
    stmcond->stmt_->accept(this);
}

void FunctionLoader::visitStmCondElse(StmCondElse* stmcondelse)
{
    stmcondelse->expr_->accept(this);
    stmcondelse->stmt_1->accept(this);
    stmcondelse->stmt_2->accept(this);
}

void FunctionLoader::visitStmWhile(StmWhile* stmwhile)
{
    stmwhile->expr_->accept(this);
    stmwhile->stmt_->accept(this);
}

void FunctionLoader::visitStmForeach(StmForeach *stmforeach)
{
    /* Code For StmForeach Goes Here */
    /* Latte++ */

    stmforeach->type_->accept(this);
    visitIdent(stmforeach->ident_);
    stmforeach->liststructuredident_->accept(this);
    stmforeach->stmt_->accept(this);

}

void FunctionLoader::visitStmSExp(StmSExp* stmsexp)
{
    stmsexp->expr_->accept(this);
}

void FunctionLoader::visitStmNoInit(StmNoInit* stmnoinit)
{
    visitIdent(stmnoinit->ident_);
}

void FunctionLoader::visitStmInit(StmInit* stminit)
{
    visitIdent(stminit->ident_);
    stminit->expr_->accept(this);
}

void FunctionLoader::visitStmInitArray(StmInitArray *stminitarray)
{
    /* Code For StmInitArray Goes Here */
    /* Latte++ */

    visitIdent(stminitarray->ident_);
    stminitarray->type_->accept(this);
    stminitarray->expr_->accept(this);

}

void FunctionLoader::visitStmInitObj(StmInitObj *stminitobj)
{
    /* Code For StmInitObj Goes Here */
    /* Latte++ */

    visitIdent(stminitobj->ident_);
    stminitobj->type_->accept(this);

}

void FunctionLoader::visitSingleIdent(SingleIdent *singleident)
{
    /* Code For SingleIdent Goes Here */
    /* Latte++ */

    visitIdent(singleident->ident_);

}

void FunctionLoader::visitTableVal(TableVal *tableval)
{
    /* Code For TableVal Goes Here */
    /* Latte++ */

    visitIdent(tableval->ident_);
    tableval->listarrayindex_->accept(this);

}

void FunctionLoader::visitSelfIdent(SelfIdent *selfident)
{
  /* Code For SelfIdent Goes Here */


}

void FunctionLoader::visitExprIndex(ExprIndex *exprindex)
{
    /* Code For ExprIndex Goes Here */
    /* Latte++ */

    exprindex->expr_->accept(this);

}

void FunctionLoader::visitClass(Class *_class)
{
    /* Code For Class Goes Here */
    /* Latte++ */

    visitIdent(_class->ident_);

}

void FunctionLoader::visitInt(Int* integer)
{
}

void FunctionLoader::visitStr(Str* str)
{
}

void FunctionLoader::visitBool(Bool* boolean)
{
}

void FunctionLoader::visitVoid(Void* void_field)
{
}

void FunctionLoader::visitTType(TType *ttype)
{
    /* Code For TType Goes Here */
    /* Latte++ */

    ttype->type_->accept(this);

}

void FunctionLoader::visitEVar(EVar *evar)
{
    /* Code For EVar Goes Here */
    /* Latte++ */

    evar->liststructuredident_->accept(this);

}

void FunctionLoader::visitELitInt(ELitInt* elitint)
{
    visitInteger(elitint->integer_);
}

void FunctionLoader::visitELitTrue(ELitTrue* elittrue)
{
}

void FunctionLoader::visitELitFalse(ELitFalse* elitfalse)
{
}

void FunctionLoader::visitELitNull(ELitNull *elitnull)
{
  /* Code For ELitNull Goes Here */


}

void FunctionLoader::visitEApp(EApp *eapp)
{
    /* Code For EApp Goes Here */
    /* Latte++ */

    eapp->liststructuredident_->accept(this);
    eapp->listexpr_->accept(this);

}

void FunctionLoader::visitEString(EString* estring)
{
    visitString(estring->string_);
}

void FunctionLoader::visitNeg(Neg* neg)
{
    neg->expr_->accept(this);
}

void FunctionLoader::visitNot(Not* not_field)
{
    not_field->expr_->accept(this);
}

void FunctionLoader::visitEDynamicCast(EDynamicCast *edynamiccast)
{
    /* Code For EDynamicCast Goes Here */
    /* Latte++ */

    visitIdent(edynamiccast->ident_);
    edynamiccast->expr_->accept(this);

}

void FunctionLoader::visitEMul(EMul* emul)
{
    emul->expr_1->accept(this);
    emul->mulop_->accept(this);
    emul->expr_2->accept(this);
}

void FunctionLoader::visitEAdd(EAdd* eadd)
{
    eadd->expr_1->accept(this);
    eadd->addop_->accept(this);
    eadd->expr_2->accept(this);
}

void FunctionLoader::visitERel(ERel* erel)
{
    erel->expr_1->accept(this);
    erel->relop_->accept(this);
    erel->expr_2->accept(this);
}

void FunctionLoader::visitEAnd(EAnd* eand)
{
    eand->expr_1->accept(this);
    eand->expr_2->accept(this);
}

void FunctionLoader::visitEOr(EOr* eor)
{
    eor->expr_1->accept(this);
    eor->expr_2->accept(this);
}

void FunctionLoader::visitPlus(Plus* plus)
{
}

void FunctionLoader::visitMinus(Minus* minus)
{
}

void FunctionLoader::visitTimes(Times* times)
{
}

void FunctionLoader::visitDiv(Div* div)
{
}

void FunctionLoader::visitMod(Mod* mod)
{
}

void FunctionLoader::visitLTH(LTH* lth)
{
}

void FunctionLoader::visitLE(LE* le)
{
}

void FunctionLoader::visitGTH(GTH* gth)
{
}

void FunctionLoader::visitGE(GE* ge)
{
}

void FunctionLoader::visitEQU(EQU* equ)
{
}

void FunctionLoader::visitNE(NE* ne)
{
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

void FunctionLoader::visitListClsDef(ListClsDef* listclsdef)
{
    /* Latte++ */
    for (ListClsDef::iterator i = listclsdef->begin() ; i != listclsdef->end() ; ++i)
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

void FunctionLoader::visitListArrayIndex(ListArrayIndex* listarrayindex)
{
    /* Latte++ */
    for (ListArrayIndex::iterator i = listarrayindex->begin() ; i != listarrayindex->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListStructuredIdent(ListStructuredIdent* liststructuredident)
{
    /* Latte++ */
    for (ListStructuredIdent::iterator i = liststructuredident->begin() ; i != liststructuredident->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void FunctionLoader::visitListExpr(ListExpr* listexpr)
{
    /* Latte++ */
    for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void FunctionLoader::visitInteger(Integer x)
{
}

void FunctionLoader::visitChar(Char x)
{
}

void FunctionLoader::visitDouble(Double x)
{
}

void FunctionLoader::visitString(String x)
{
}

void FunctionLoader::visitIdent(Ident x)
{
}

} /* namespace frontend */
