#!/bin/bash

#open from root!

sed -i ./bin/grammar/Parser.C -e 's/^      yyerror (YY_("syntax error"));$/       fprintf(stderr, "ERROR\\n");yyerror (YY_("syntax error"));/'
