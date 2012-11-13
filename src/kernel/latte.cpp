/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "ParserManager.h"
#include "ASTChecker.h"
#include "Absyn.H"

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

    frontend::ASTChecker checker;

    short number_of_inputs =
            arguments.input_count == 0 ? 1 : arguments.input_count;
    for(short i = 0; i < number_of_inputs; i++)
    {
        FILE* input = open_file(arguments.input_count,
                (arguments.input_count > 0)
                    ? arguments.input_files[i]
                    : 0);
        frontend::ParserManager parser_mngr(input);
        if (not parser_mngr.try_to_parse())
        {
            cerr << "Parse error! " << endl;  // TODO: more sophisticated error msg.
            if (fclose(input) != 0)
                cerr << "Cannot close file stream " << arguments.input_files[i] << endl;
            continue;
        }

        Prog* program = parser_mngr.get_prog();

        // Close file.
        if (fclose(input) != 0)
            cerr << "Cannot close file stream " << arguments.input_files[i] << endl;

        // Check AST. Semantics and types.
        checker.check(program);

        // TODO: Compile.
    }

    return EXIT_SUCCESS;
}
