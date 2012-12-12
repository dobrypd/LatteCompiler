/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include <string>
#include <vector>
#include <fstream>

namespace frontend
{

class ErrorHandler
{
public:
    typedef std::pair<bool, std::string> error_type;

private:
    bool is_file;
    std::string file_name;
    std::ifstream file;
    std::vector<std::pair<int, error_type> > errors;

    int current_pos;
    std::string current_line;
    std::string get_line(int pos);

public:
    ErrorHandler(const char* input_file);
    ~ErrorHandler();
    void handle(int line, error_type error);
    void error(int line, std::string msg);
    void warning(int line, std::string msg);
    void flush();

    bool has_errors();
};

} /* namespace frontend */
#endif /* ERRORHANDLER_H_ */
