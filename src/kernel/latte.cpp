/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Absyn.H"
#include "ParserManager.h"
#include "ASTChecker.h"
#include "ErrorHandler.h"
#include "FunctionLoader.h"
#include "TreeOptimizer.h"
#include "ReturnsChecker.h"

using std::cerr;
using std::cout;
using std::endl;

#ifndef NDEBUG
    const bool debug=true;
#else
    const bool debug=false;
#endif

void show_help(char* prog_name)
{
    cout << "using " << prog_name << " [options] infile..." << endl
            << "options:" << endl
            << "-h\t- show this help message," << endl
            << "-o\t- define output file." << endl
            << endl
            << "infile...\t- list of input files, if none - get from stdin."
            << endl;
}

struct Arguments {
    bool wrong;
    bool help;
    int input_count;
    char** input_files;
    char* output_file;
};

Arguments parse_args(int argc, char** argv)
{
    Arguments args;
    args.wrong = false;
    args.help = false;
    args.input_count = 0;
    args.input_files = 0;
    args.output_file = 0;
    int c;
    while ((c = getopt(argc, argv, "ho:")) != -1)
    {
        switch (c)
        {
        case 'h':
            args.help = true;
            break;
        case 'o':
            args.output_file = optarg;
            break;
        case '?':
            args.wrong = true;
            break;
        default:
            abort();
            break;
        }
    }
    args.input_count = argc - optind;
    if (argc > optind)
        args.input_files = argv + optind;
    return args;
}

FILE* open_file(short files, char* file_name)
{
    FILE* input = 0;
    if (files > 0)
    {
        input = fopen(file_name, "r");
        if (!input)
        {
            cerr << "Cannot open file: " << file_name << endl;
            exit(EXIT_FAILURE);
        }
        if (debug)
            cerr << "Opened " << file_name << endl;
    }
    else
    {
        if (debug)
            cerr << "Will load Latte program from stdio." << endl;
        input = stdin;
    }
    return input;
}

int main(int argc, char** argv)
{
    // Parse arguments.
    Arguments arguments(parse_args(argc, argv));
    if (arguments.help or arguments.wrong)
    {
        show_help(argv[0]);
        return arguments.wrong ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    short number_of_inputs =
            arguments.input_count == 0 ? 1 : arguments.input_count;
    for(short i = 0; i < number_of_inputs; i++)
    {
        if (debug)
            std::cout << std::endl << "-- new file "
            << ((arguments.input_count > 0) ? arguments.input_files[i] : "stdin")
            << " --" << std::endl << std::endl;
        FILE* input = open_file(arguments.input_count,
                (arguments.input_count > 0)
                    ? arguments.input_files[i]
                    : NULL);
        frontend::ParserManager parser_mngr(input);
        if (not parser_mngr.try_to_parse())
        {
            if (fclose(input) != 0)
                cerr << "Cannot close file stream " << arguments.input_files[i] << endl;
            continue;
        }

        // Parse.
        Visitable* ast_root;
        ast_root = parser_mngr.get();

        // Close file.
        if (fclose(input) != 0)
            cerr << "Cannot close file stream " << arguments.input_files[i] << endl;

        // Used in all checkers as references!!! Delete in this same time!!!
        frontend::Environment env;
        frontend::ErrorHandler file_error_handler(
                (arguments.input_count > 0) ? arguments.input_files[i] : NULL);

        // Load functions.
        frontend::FunctionLoader function_loader(file_error_handler, env);
        function_loader.check(ast_root);
        if (!file_error_handler.has_errors()){
            // Type check (without returns).
            Ident pr_name((arguments.input_count > 0) ? arguments.input_files[i] : "stdin");
            frontend::ASTChecker checker(file_error_handler, env, pr_name);
            checker.check(ast_root);
        }

        // Optimizer TODO: next part of assignment. Tree optimization.

        if (!file_error_handler.has_errors()){
            // Returns checker.
            frontend::ReturnsChecker returns_checker(file_error_handler, env);
            returns_checker.check(ast_root);
        }


        // End of semantic check, typecheck and tree optimization.
        if (debug)
            std::cout << "--ERRORS--" << std::endl;
        file_error_handler.flush();

        if (!file_error_handler.has_errors()){
            // COMPILE!
        }
    }

    return EXIT_SUCCESS;
}
