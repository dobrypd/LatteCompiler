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
#include "ASCreator.h"

using std::cerr;
using std::cout;
using std::endl;

#ifndef NDEBUG
    const bool debug=true;
#else
    const bool debug=false;
#endif

// Architecture and system based constants.

enum arch_t {x86, x86_64};

#ifdef _ARCH_x86_64
    const int arch = x86_64;
    #error Currently cannot build x86_64 late compiler.
#else
    #ifdef _ARCH_x86
        const int arch = x86;
    #else
        #error You must define one of _ARCH_x86 _ARCH_x86_64.
    #endif
#endif

const char* compiler_executable = "gcc";
const char* linker_executable = "ld";

const char* compiler_flags = "-c";
const char* linker_flags = "";

#ifdef _ARCH_x86_64
    const char* compiler_arch_flags = "-m64";
    const char* linker_arch_flags = "";
#endif
#ifdef _ARCH_x86
    const char* compiler_arch_flags = "-m32";
    const char* linker_arch_flags =
            "-melf_i386 -l:./lib/lib32/libc.a ./lib/lib32/crt?.o";
#endif


const char* stdin_in_filename = "from_stdin.lat";
const char* default_output_filename = "a.out";

// Constants end.




void show_help(char* prog_name)
{
    cout << "using " << prog_name << " [options] infile..." << endl
            << "options:" << endl
            << "-h\t- show this help message," << endl
            << "-o\t- define output file "
            << "(default this same as infile for assembler code (*.s) "
            << "and a.out for binary)." << endl << endl
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
 * Open stream (file or stdio).
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

std::string create_out_name(const char* input_file_name,
        const char* extension)
{
    std::string ofn(input_file_name);
    size_t found = ofn.rfind('.');
    if (found != std::string::npos){
        ofn = ofn.substr(0, found);
    }
    ofn += ".";
    ofn += extension;
    return ofn;
}


/*
 * Will close file after parse, or in case of an error.
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
        return EXIT_FAILURE;
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

/*
 * Compile using ast.
 */
void compile_file(Visitable* ast_root, const char* input_file_name,
        frontend::Environment& env)
{
    // Create assembly file.
    std::string assembly_file_name = create_out_name(input_file_name, "s");
    backend::ASCreator as_generator(assembly_file_name, env);
    as_generator.generate(ast_root);


    // Call assembler to create output binary file.
    FILE* cmd = NULL;
    std::string command(compiler_executable);
    command.append(" ");
    command.append(compiler_flags);
    command.append(" ");
    command.append(compiler_arch_flags);
    command.append(" -o");
    command.append(create_out_name(input_file_name, "o"));
    command.append(" ");
    command.append(assembly_file_name);
    if (debug) cout << command << endl;
    cmd = popen(command.c_str(), "r");
    pclose(cmd);
}


/*
 * Link compiled files.
 */
void link_files(const int number_of_inputs, char** input_files,
        bool is_from_stdin, const char* output_file_name)
{
    // Call linker.
    FILE* cmd = NULL;
    std::string command(linker_executable);
    command.append(" ");
    command.append(linker_flags);
    command.append(" ");
    command.append(linker_arch_flags);
    command.append(" -o");
    command.append(output_file_name);
    for(short i = 0; i < number_of_inputs; i++) {
        command.append(" ");
        if (is_from_stdin)
            command.append(create_out_name(stdin_in_filename, "o"));
        else
            command.append(create_out_name(input_files[i], "o"));
    }
    if (debug) cout << command << endl;
    cmd = popen(command.c_str(), "r");
    pclose(cmd);
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

    for(short i = 0; i < number_of_inputs; i++)
    {
        if (debug)
            std::cout << std::endl << "-- checking file "
            << ((arguments.input_count > 0)
                    ? arguments.input_files[i] : stdin_in_filename)
            << " --" << std::endl << std::endl;

        FILE* input = open_file(arguments.input_count,
                (arguments.input_count > 0)
                    ? arguments.input_files[i]
                    : NULL);

        frontend::ParserManager parser_mngr(input);
        frontend::Environment env;
        Visitable* ast_root;


        int check_status = check_file(input,
            (arguments.input_count > 0)
            ? arguments.input_files[i] : stdin_in_filename,
            parser_mngr, env, ast_root);

        if (check_status == 0) {
            compile_file(ast_root,
                    (arguments.input_count > 0)
                    ? arguments.input_files[i] : stdin_in_filename,
                    env);
        } else {
            return EXIT_FAILURE;
        }
    }

    link_files(number_of_inputs, arguments.input_files,
               (arguments.input_count == 0),
               (arguments.output_file != NULL)
                   ? arguments.output_file
                   : default_output_filename);

    std::cerr << "OK" << std::endl;
    return EXIT_SUCCESS;
}
