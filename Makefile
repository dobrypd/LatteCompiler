# Author Piotr Dobrowolski
# pd291528@students.mimuw.edu.pl
include bin/Paths.mk

PROJECT=Latte
BINARY=latec
GRAMMAR_OBJ=Absyn.o Lexer.o Parser.o Printer.o

OBJECTS:=$(patsubst $(SRC)/kernel/%.cpp,$(BIN)/%.o,$(wildcard $(SRC)/kernel/*.cpp))

OPTDBG= -O2 -DNDEBUG
ifeq ($(debug), 1)
	OPTDBG= -g
endif


BNFC=bnfc
BNFC_FLAGS=-m -cpp_stl -l
CC=g++
CFLAGS=-Wall -c $(OPTDBG)
LFLAGS=-Wall $(OPTDBG)

all : $(PROJECT)

grammar: $(GRAMMAR)/$(PROJECT).cf
	@echo -en "\033[38m\033[32mCompiling grammar...\033[0m\n"
	mkdir -p $(GRAMMAR_BIN)
	cd $(GRAMMAR_BIN) && $(BNFC) $(BNFC_FLAGS) $(GRAMMAR)/$(PROJECT).cf
	# bnfc / gcc verions collision fixup
	@sed "3 a #include <algorithm>" $(GRAMMAR_BIN)/Latte.y > $(GRAMMAR_BIN)/Latte.y.tmp
	@mv $(GRAMMAR_BIN)/Latte.y.tmp $(GRAMMAR_BIN)/Latte.y
	@$(MAKE) -C $(GRAMMAR_BIN) $(GRAMMAR_OBJ)


$(PROJECT): $(OBJECTS)
	@echo -en "\033[38m\033[32mLinking $(PROJECT)...\033[0m\n"
	$(CC) $(LFLAGS) $^ $(patsubst %, $(GRAMMAR_BIN)/%, $(GRAMMAR_OBJ)) -I $(INCLUDES) -I $(GRAMMAR_BIN) -o $(BINARY)

$(OBJECTS): $(BIN)/%.o : $(SRC)/kernel/%.cpp
	@echo -en "Compiling \033[38m\033[33m$(patsubst $(SRC)/kernel/%,%, $<)\033[0m...\n"
	$(CC) $(CFLAGS) $< -I $(INCLUDES) -I $(GRAMMAR_BIN) -o $@

test: $(PROJECT)
	cd $(SCRIPTS); ./test_runner.sh
	cd $(ROOT)


clean:
	@$(MAKE) -C $(GRAMMAR_BIN) clean
	rm -f $(BIN)/*.o	
	find $(ROOT) -iname "*.bak" | xargs rm -f
	find $(ROOT) -iname "*.swp" | xargs rm -f
	find $(ROOT) -iname "*~" | xargs rm -f

distclean: clean
	rm -f $(BIN)/$(PROJECT)
	rm -rf $(GRAMMAR_BIN)

.PHONY: clean, distclean, test
