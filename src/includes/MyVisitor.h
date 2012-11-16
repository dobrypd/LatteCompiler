/*
 * MyVIsitor.h
 *
 *  Created on: 16-11-2012
 *      Author: piotrek
 */

#ifndef MYVISITOR_H_
#define MYVISITOR_H_

#include "Environment.h"
#include "ErrorHandler.h"

namespace frontend
{

class MyVisitor
{
private:
    ErrorHandler& error_handler;
    Environment& env;
public:
    MyVisitor(ErrorHandler& error_handler, Environment& env);
};

} /* namespace frontend */
#endif /* MYVISITOR_H_ */
