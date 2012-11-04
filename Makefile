include bin/Paths.mk

PROJECT=Latte
GRAMMAR_OBJ=Absyn.o Lexer.o Parser.o Printer.o

OBJECTS:=$(patsubst $(SRC)/%.cpp,$(BIN)/%.o,$(wildcard $(SRC)/grammar/*.cpp))

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

$(PROJECT): $(OBJECTS)
	echo "TODO"

te:
	echo $(SRC)

grammar: $(GRAMMAR)/$(PROJECT).cf
	mkdir -p $(GRAMMAR_BIN)
	cd $(GRAMMAR_BIN) && $(BNFC) $(BNFC_FLAGS) $(GRAMMAR)/$(PROJECT).cf
	# bnfc / gcc verions collisions (in gentoo) fixup
	sed "3 a #include <algorithm>" $(GRAMMAR_BIN)/Latte.y > $(GRAMMAR_BIN)/Latte.y.tmp
	mv $(GRAMMAR_BIN)/Latte.y.tmp $(GRAMMAR_BIN)/Latte.y
	@$(MAKE) -C $(GRAMMAR_BIN) $(GRAMMAR_OBJ)

test: $(PROJECT)
	cd $(SCRIPTS); ./test_runner.sh
	cd $(ROOT)

clean:
	rm -rf $BIN/Latte

distclean:
	echo "Cleaning all"
	rm -rf $BIN/Latte

.PHONY: clean, distclean, test