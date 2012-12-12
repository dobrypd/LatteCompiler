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
#include "JVMGenerator.h"

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

/*
 * open stream (file or stdio)
 */
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

/*
 * will close file after parse, or in case of an error
 */
int check_file(FILE* input, const char* file_name,
        frontend::ParserManager& parser_mngr,
        frontend::Environment& env,
        Visitable*& ast_root)
{
    if (not parser_mngr.try_to_parse())
    {
        if (fclose(input) != 0) {
            cerr << "ERROR" << endl;
            cerr << "Cannot close file stream " << file_name << endl;
        }
        std::cerr << "ERROR" << std::endl;
        return EXIT_FAILURE;
    }

    // Parse.
    ast_root = parser_mngr.get();

    // Close file.
    if (fclose(input) != 0) {
        cerr << "ERROR" << endl;
        cerr << "Cannot close file stream " << file_name << endl;
    }

    frontend::ErrorHandler file_error_handler(file_name);

    // Load functions.
    frontend::FunctionLoader function_loader(file_error_handler, env);
    function_loader.check(ast_root);

    if (!file_error_handler.has_errors()){
        // Type check (without returns).
        Ident pr_name(file_name);
        frontend::ASTChecker checker(file_error_handler, env, pr_name);
        checker.check(ast_root);
    }

    if (!file_error_handler.has_errors()){
        // Returns checker.
        frontend::ReturnsChecker returns_checker(file_error_handler, env);
        returns_checker.check(ast_root);
    }

    // End of semantic check, typecheck and tree optimization.
    if (file_error_handler.has_errors()){
        std::cerr << "ERROR" << std::endl;
        file_error_handler.flush();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void compile_file(Visitable*& ast_root, const char* output_file_name)
{
    // Compilation
    jvm::JVMGenerator jvm_generator(output_file_name);
    jvm_generator.generate(ast_root);
    std::cerr << "OK" << std::endl;
}


/*
 * Parse arguments, open files, check and compile all of it.
 */
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

    char* output_file_name = arguments.output_file;

    for(short i = 0; i < number_of_inputs; i++)
    {
        if (debug)
            std::cout << std::endl << "-- new file "
            << ((arguments.input_count > 0)
                    ? arguments.input_files[i] : "stdin")
            << " --" << std::endl << std::endl;

        FILE* input = open_file(arguments.input_count,
                (arguments.input_count > 0)
                    ? arguments.input_files[i]
                    : NULL);

        frontend::ParserManager parser_mngr(input);
        frontend::Environment env;
        Visitable* ast_root;


        int check_status = check_file(input,
            (arguments.input_count > 0) ? arguments.input_files[i] : "stdin",
            parser_mngr, env, ast_root);

        if (check_status == 0) {
            compile_file(ast_root, output_file_name);
        } else {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
