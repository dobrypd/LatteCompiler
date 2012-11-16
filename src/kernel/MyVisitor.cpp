/*
 * MyVIsitor.cpp
 *
 *  Created on: 16-11-2012
 *      Author: piotrek
 */

#include "MyVisitor.h"
#include "ErrorHandler.h"

namespace frontend
{

MyVisitor::MyVisitor(ErrorHandler& error_handler, Environment& env)
    : error_handler(error_handler), env(env)
{
    // TODO Auto-generated constructor stub

}

} /* namespace frontend */
