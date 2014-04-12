# A simple make file to build a project. It merges all of the specs
# using the 'cat' command and compiles it into a runner executable.

CC=g++
CFLAGS=-c -Wall -MMD

# setup source files and executable name
SOURCES=$(wildcard *.cpp)
OBJECTS=$(addprefix ../obj/,$(notdir $(SOURCES:.cpp=.o))) $(wildcard ../obj/*.o)
EXECUTABLE=runner

# -- targets --------------------------------------------------

.SILENT: all

# default make target
all: $(EXECUTABLE) $(SOURCES); ./runner

# target to make the executable
$(EXECUTABLE): $(OBJECTS); $(CC) $(LDFLAGS) $^ -o $@

# generic target to setup object source dependency
../obj/%.o: %.cpp; $(CC) $(CFLAGS) $< -o $@

-include $(OBJECTS:.o=.d)

# clean the build
clean: ; rm -rf *.o $(EXECUTABLE)
