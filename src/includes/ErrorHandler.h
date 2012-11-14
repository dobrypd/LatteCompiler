/*
 * ErrorHandler.h
 *
 *  Created on: 14-11-2012
 *      Author: piotrek
 */

#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include <string>
#include <vector>
#include <stdio.h>

namespace frontend
{

class ErrorHandler
{
public:
    typedef std::pair<bool, std::string> error_type;

private:
    FILE* file;
    std::vector<std::pair<int, error_type> > errors;

    int current_pos;
    char* get_line(int pos);

public:
    ErrorHandler(FILE* file);
    void handle(int line, error_type error);
    void flush();
};

} /* namespace frontend */
#endif /* ERRORHANDLER_H_ */
