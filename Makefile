# Author Piotr Dobrowolski
# pd291528@students.mimuw.edu.pl
include bin/Paths.mk

ARCH=x86

PROJECT=Latte
BINARY=latc_$(ARCH)
GRAMMAR_OBJ=Absyn.o Lexer.o Parser.o Printer.o
GRAMMAR_SUFIX=

OBJECTS:=$(patsubst $(SRC)/kernel/%.cpp,$(BIN)/%.o,$(wildcard $(SRC)/kernel/*.cpp))

RUNTIME_OBJ:=$(LIB)/runtime.o

OPTDBG= -O2 -DNDEBUG
ifeq ($(debug), 1)
	OPTDBG= -g
endif


BNFC=bnfc
BNFC_FLAGS=-m -cpp_stl -l
CC=g++
CFLAGS=-Wall -c $(OPTDBG) -D_ARCH_$(ARCH)
LFLAGS=-Wall $(OPTDBG)

all : $(GRAMMAR_BIN)/Parser.C $(PROJECT) $(RUNTIME_OBJ)

$(GRAMMAR_BIN)/Parser.C: $(GRAMMAR)/$(PROJECT).cf
	@echo -en "\033[38m\033[32mCompiling grammar...\033[0m\n"
	mkdir -p $(GRAMMAR_BIN)
	cd $(GRAMMAR_BIN) && $(BNFC) $(BNFC_FLAGS) $(GRAMMAR)/$(PROJECT)$(GRAMMAR_SUFIX).cf
	@echo Fixing bison file
	@sed "3 a #include <algorithm>" $(GRAMMAR_BIN)/$(PROJECT)$(GRAMMAR_SUFIX).y > $(GRAMMAR_BIN)/$(PROJECT)$(GRAMMAR_SUFIX).y.tmp
	@mv $(GRAMMAR_BIN)/Latte.y.tmp $(GRAMMAR_BIN)/$(PROJECT)$(GRAMMAR_SUFIX).y
	@echo Adding fixups to Parser.C
	@$(MAKE) -C $(GRAMMAR_BIN) Parser.C
	$(SCRIPTS)/fix_grammar.sh
	@$(MAKE) -C $(GRAMMAR_BIN) $(GRAMMAR_OBJ)
	
$(RUNTIME_OBJ): $(LIB)/runtime.c
	@$(MAKE) -C $(LIB)


$(PROJECT): $(OBJECTS)
	@echo -en "\033[38m\033[32mLinking $(PROJECT)...\033[0m\n"
	$(CC) $(LFLAGS) $^ $(patsubst %, $(GRAMMAR_BIN)/%, $(GRAMMAR_OBJ)) -I $(INCLUDES) -I $(GRAMMAR_BIN) -o $(BINARY)

$(OBJECTS): $(BIN)/%.o : $(SRC)/kernel/%.cpp
	@echo -en "Compiling \033[38m\033[33m$(patsubst $(SRC)/kernel/%,%, $<)\033[0m...\n"
	$(CC) $(CFLAGS) $< -I $(INCLUDES) -I $(GRAMMAR_BIN) -o $@

test: all
	cd $(SCRIPTS); ./test_runner.sh
	cd $(ROOT)


clean:
	rm -f $(BIN)/*.o
	rm -f $(BINARY)
	find $(ROOT) -iname "*.bak" | xargs rm -f
	find $(ROOT) -iname "*.swp" | xargs rm -f
	find $(ROOT) -iname "*~" | xargs rm -f
	@$(MAKE) -C $(LIB) clean

distclean: clean
	rm -f $(BIN)/$(PROJECT)
	rm -rf $(GRAMMAR_BIN)

.PHONY: clean, distclean, test
