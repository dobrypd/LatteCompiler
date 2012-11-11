/*
 * Author: Piotr Dobrowolski
 * pd291528@students.mimuw.edu.pl
 *
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Parser.H"
#include "Absyn.H"
#include "Printer.H"

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

void open_files(int count, char** file_names, FILE** inputs)
{
    for (int i = 0; i < count; i++)
    {
        inputs[i] = fopen(file_names[i], "r");
        if (!inputs[i])
        {
            cerr << "Cannot open file: " << file_names[i] << endl;
            exit(EXIT_FAILURE);
        }
    }
    if (count == 0)
    {
        if (debug)
            cerr << "Will load Latte program from stdio." << endl;
        inputs[1] = stdin;
    }
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

    // Open files.
    FILE* inputs[arguments.input_count == 0 ? 1 : arguments.input_count];
    open_files(arguments.input_count, arguments.input_files, inputs);
    if (debug)
        cerr << "Opened " << arguments.input_count << " file(s)." << endl;

    // Parse input files.
    Prog* parse_tree = pProg(inputs[0]);

    // Close files.
    for (int i = 0; i < arguments.input_count; i++)
        fclose(inputs[i]);

    if (!parse_tree)
            //cerr << parse_tree << endl;
            return EXIT_FAILURE;

    ShowAbsyn *s = new ShowAbsyn();
    cout << s->show(parse_tree) << endl;
    delete s;
    delete parse_tree;

    // Check semantics.
    cout << "HERE WILL DO IT...";

    // Check types.

    // Compile.

    return EXIT_SUCCESS;
}
