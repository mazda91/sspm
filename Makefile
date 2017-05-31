GXX=g++
CFLAGS= -c  -std=c++11 
WFLAGS= -Wall -Wextra
LDFLAGS= -g
CImgFLAGS = -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

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
	$(GXX) -g -o $@  $^ $(CImgFLAGS)

$(OBJDIR)/%.o :  $(SRCDIR)/%.cpp $(OBJDIR) 
	$(GXX) $(CFLAGS) $(LDFLAGS) $(WFLAGS) $< -o $@

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

test: $(EXECTEST)


$(EXECTEST): ./obj/daphnia.o ./obj/model.o ./obj/mmi.o ./tests/test.o
	$(GXX)  $^ -o $@

$(TESTDIR)/test.o: $(TESTDIR)/test.cpp $(INCFILES)
	$(GXX) $(CFLAGS) $(LDFLAGS) $(WFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)


