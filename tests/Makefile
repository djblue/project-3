# A simple make file to build a project. It merges all of the specs
# using the 'cat' command and compiles it into a runner executable.

CC=g++
CFLAGS=-g -Wall

# -- targets --------------------------------------------------

.SILENT: all
# default make target
all: ; @$(CC) $(CFLAGS) runner.cpp -o runner && ./runner
