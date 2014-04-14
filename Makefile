# A simple make file to build a project.

# make automatic variables info:
# $@ - target name
# $^ - dependency names
# $< - first dependency name
# $* - match of the implicit

# -- setup ----------------------------------------------------

.SILENT: all clean tc1 tc2 tc3 tc4 unitk watch

# set files to be archived for submission
ASSIGN_FILES=*

# setup compiler variables
CC=g++
CFLAGS=-c -g -Wall
LDFLAGS=

# setup source files and executable name
SOURCES=main.cpp
OBJECTS=main.o
EXECUTABLE=run

# -- targets --------------------------------------------------

# default make target
all: $(EXECUTABLE) $(SOURCES)

# target to make the executable
$(EXECUTABLE): $(OBJECTS); @$(CC) $(LDFLAGS) $^ -o $@

# generic target to setup object source dependency
%.o: %.cpp; @$(CC) $(CFLAGS) $< -o $@

# clean the build
clean: ; rm -rf *.o $(EXECUTABLE) && rm -f tests/runner

# archive the assignment
zip: ; zip -r Badahdah-Abdullah-P1.zip .

# -q --quiet
# -r --recursive
# -e <event>

watch: ; @while \
    inotifywait -qre close_write \
    --format "`echo -e '\a\n%T \033[0;34m%f\033[0;0m modified...'` " \
    --timefmt "`echo -e '\033[0;32m%r\033[0;0m'`" \
    ./include ./tests \
    --exclude runner; \
    do echo && make unit > /dev/null; done

test: unit lexer
#test: unit

# run all tests
unit: ; @$(MAKE) -sC tests

lexer: ltc1 ltc2 ltc3 ltc4

# run all test cases for lexer
ltc%: tc/lexer/input%.txt
	@rm -f input.txt
	@cp tc/lexer/input$*.txt input.txt
	@./run -l input.txt output.txt
	@diff -bc output.txt tc/lexer/output$*.txt && echo "PASSED $*"
	@rm -f input.txt output.txt

parser: ptc1 ptc2 ptc3 ptc4 ptc5 ptc6

# run all test cases for parser
ptc%: tc/parser/input%.txt
	@rm -f input.txt
	@cp tc/parser/input$*.txt input.txt
	@./run -p input.txt output.txt
	@diff -bc output.txt tc/parser/output$*.txt && echo "PASSED $*"
	@rm -f input.txt output.txt

semantic: stc1 stc2 stc3

# run all test cases for parser
stc%: tc/semantic/input%.txt
	@rm -f input.txt
	@cp tc/semantic/input$*.txt input.txt
	@./run input.txt output.txt
	@diff -bc output.txt tc/semantic/output$*.txt && echo "PASSED $*"
	@rm -f input.txt output.txt
