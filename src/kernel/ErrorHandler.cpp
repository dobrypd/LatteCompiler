/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include <algorithm>
#include <fstream>
#include "ErrorHandler.h"

namespace frontend
{

ErrorHandler::ErrorHandler(const char* input_file)
{
    if (input_file != NULL)
    {
        this->is_file = true;
        this->file_name = input_file;
        this->file.open(input_file);
        this->current_pos = 0;
    }
    else
        this->is_file = false;
}

ErrorHandler::~ErrorHandler()
{
    if (this->is_file)
        this->file.close();
}

void ErrorHandler::handle(int line, error_type error)
{
    this->errors.push_back(std::make_pair(line, error));
}

void ErrorHandler::error(int line, std::string msg)
{
    ErrorHandler::error_type error = std::make_pair(true, msg);
    this->handle(line, error);
}

void ErrorHandler::warning(int line, std::string msg)
{
    ErrorHandler::error_type warning = std::make_pair(false, msg);
    this->handle(line, warning);
}

std::string ErrorHandler::get_line(int pos)
{
    if (pos >= this->current_pos)
    {
        while(pos > this->current_pos)
        {
            if(std::getline(this->file, this->current_line))
            {
                this->current_pos++;
            }
            else
            {
                return "Line not existing";
            }
        }
        return this->current_line;
    }
    else
    {
        this->current_pos = 0;
        this->file.seekg(this->current_pos);
        return this->get_line(pos);
    }
}

void ErrorHandler::flush()
{
    std::sort(this->errors.begin(), this->errors.end());
    for(std::vector<std::pair<int, error_type> >::iterator
            it = this->errors.begin(); it != this->errors.end(); it++)
    {
        if (this->is_file)
            std::cout << this->file_name << ':' <<  it->first << ":`"
            << this->get_line(it->first) << '`' << std::endl;
        std::cout << (it->second.first ? "Error" : "Warning")
                << ": " << it->second.second << std::endl;
    }
}

bool ErrorHandler::has_errors()
{
    return !(this->errors.empty());
}

} /* namespace frontend */
