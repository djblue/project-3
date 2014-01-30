# A simple make file to build a project.

# make automatic variables info:
# $@ - target name
# $^ - dependency names
# $< - first dependency name
# $* - match of the implicit

# -- setup ----------------------------------------------------

.SILENT: all clean tc1 tc2 tc3

# set files to be archived for submission
ASSIGN_FILES=*

# setup compiler variables
CC=g++
CFLAGS=-c -Wall
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
zip: $(ASSIGN_FILES); zip -r CSE310-P03-Badahdah-Abdullah.zip $^

test: unit tc1 tc2 tc3
#test: unit

# run all tests
unit: ; @$(MAKE) -sC tests

# run all test cases
tc%: tc/input%.txt
	rm -f input.txt
	cp tc/input$*.txt input.txt
	./run input.txt output.txt
	diff -bc output.txt tc/output$*.txt && echo "PASSED $*"
	rm -f input.txt output.txt