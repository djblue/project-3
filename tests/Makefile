# A simple make file to build a project. It merges all of the specs
# using the 'cat' command and compiles it into a runner executable.

# -- targets --------------------------------------------------

.SILENT: all
# default make target
all: ; cat *.spec.cpp runner.cpp | g++ -Wall -x c++ - -o runner && ./runner
