.SUFFIXES:
.SUFFIXES: .lex .y .cc .hh .o *.c *.h *.orig

.PHONY: clean all astyle syntax

CC_FILES	:= $(wildcard *.cc)
HH_FILES	:= $(wildcard *.hh)
O_FILES		:= $(CC_FILES:%.cc=%.o)

all: lisp

O_SYNTAX_FILES	:= lisp_lex.o lisp_yacc.o

syntax: $(O_SYNTAX_FILES)

lisp_lex.c: lisp.lex lisp_yacc.c lisp_yacc.h Makefile
	flex -o $@ $<

lisp_yacc.c: lisp.y Makefile
	bison -d -o $@ $<

GCC_FLAGS	:= -Wall -Wno-unused-function -Wno-unused-variable

lisp_lex.o: lisp_lex.c
	g++ -Wall $(GCC_FLAGS) $<  -c

lisp_yacc.o: lisp_yacc.c
	g++ -Wall $(GCC_FLAGS) $<  -c

################################
cell.o: cell.cc cell.hh
env.o: env.cc env.hh object.hh cell.hh
eval.o: eval.cc eval.hh object.hh cell.hh env.hh
main.o: main.cc object.hh cell.hh env.hh eval.hh
object.o: object.cc object.hh cell.hh
cell.o: cell.hh
env.o: env.hh object.hh cell.hh
eval.o: eval.hh object.hh cell.hh env.hh
main.o: main.hh
object.o: object.hh cell.hh
main.o: lisp_yacc.o
################################

$(O_FILES): %.o: %.cc %.hh Makefile
	g++ -Wall $< -c

lisp: $(O_FILES) $(O_SYNTAX_FILES) Makefile
	g++ -Wall $(O_FILES) $(O_SYNTAX_FILES) -o $@ 

ASTYLE_OPTIONS =        --style=attach --indent=spaces=2

astyle:
	astyle $(ASTYLE_OPTIONS) $(CC_FILES) $(HH_FILES)


clean:
	-rm lex.* parse.* heading.h a.out *.o *.output *.c *.h *.orig lisp
	-rm -r *.dSYM
