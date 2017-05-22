GXX=g++
CFLAGS= -c -g -Wall -Wextra -std=c++11


SRCDIR = ./src
OBJDIR = ./obj
INCDIR = ./include
BINDIR = ./bin
TESTDIR = ./tests

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
INCFILES =  $(wildcard $(INCDIR)/*.hpp)
OBJFILES = $(SRCFILES :.cpp=.o)
EXEC = $(BINDIR)/main

all: $(EXEC)

$(EXEC) : $(OBJFILES)
	$(GXX) $^ -o $@ 

$(OBJDIR)/%.o :  $(SRCDIR)/%.cpp $(INCDIR)/%.hpp 
	$(GXX) $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)


