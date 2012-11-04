include bin/Paths.mk

PROJECT=Latte

OBJECTS:=$(patsubst $(SRC)/%.cpp,$(BIN)/%.o,$(wildcard $(SRC)/grammar/*.cpp))
GRAMMAR_OBJ=Absyn.o Lexer.o Parser.o Printer.o Skeleton.o

OPTDBG= -g
ifeq ($(release), 1)
	OPTDBG= -O2 -DNDEBUG
endif


BNFC=bnfc
BNFC_FLAGS=-m -cpp_stl
CC=g++
CFLAGS=-Wall -c $(OPTDBG)
LFLAGS=-Wall $(OPTDBG)

all : $(PROJECT)

$(PROJECT) : grammar $(OBJECTS)
	echo "TODO"

te:
	echo $(SRC)

pre_grammar:
	mkdir -p $(GRAMMAR_BIN)
	cd $(GRAMMAR_BIN) && $(BNFC) $(BNFC_FLAGS) $(GRAMMAR)/Latte.cf
	@$(MAKE) -C $(GRAMMAR_BIN) $(GRAMMAR_OBJ)

grammar: pre_grammar

test: $(PROJECT)
	cd $(SCRIPTS); ./test_runner.sh
	cd $(ROOT)

clean:
	rm -rf $BIN/Latte

distclean:
	echo "Cleaning all"
	rm -rf $BIN/Latte

.PHONY: clean, distclean, test