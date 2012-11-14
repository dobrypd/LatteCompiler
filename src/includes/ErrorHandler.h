/*
 * ErrorHandler.h
 *
 *  Created on: 14-11-2012
 *      Author: piotrek
 */

#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include <stdio.h>

namespace frontend
{

class ErrorHandler
{
private:
    FILE* file;
    int current_line;
public:
    ErrorHandler(FILE* file);
    //void handle(int line, std::string description, bool warning=false);
};

} /* namespace frontend */
#endif /* ERRORHANDLER_H_ */
