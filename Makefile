GXX=g++
CFLAGS= -c -g -Wall -Wextra -std=c++11


SRCDIR = ./src
OBJDIR = ./obj
INCDIR = ./include
BINDIR = ./bin
TESTDIR = ./tests

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
INCFILES =  $(wildcard $(INCDIR)/*.hpp)
OBJFILES=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCFILES))
EXEC = $(BINDIR)/main
EXECTEST = $(TESTDIR)/test

all: $(EXEC)

$(EXEC) : $(OBJFILES)
	$(GXX) $^ -o $@ 

$(OBJDIR)/%.o :  $(SRCDIR)/%.cpp $(OBJDIR) 
	$(GXX) $(CFLAGS) $< -o $@

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

test: $(EXECTEST)


$(EXECTEST):	$(TESTDIR)/main.cpp
	$(GXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)


