GXX=g++ -fPIC
CFLAGS= -c   -std=c++11 -I $(matlabroot)/extern/include/ 
WFLAGS= -Wall -Wextra
LDFLAGS= -g
LIBS= -L$(matlabroot)/bin/glnxa64 -leng -lmx -lmat
matlabroot= /usr/local/MATLAB/R2017a
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
	$(GXX) -g -o $@ $^ $(LIBS) -Wl,-rpath-link,$(matlabroot)/bin/glnxa64 
	mex mexGate.cpp $(LIBS)  -output $(BINDIR)/mexGate.mexa64 $^

$(OBJDIR)/%.o :  $(SRCDIR)/%.cpp $(OBJDIR)  
	$(GXX) $(CFLAGS) $(LDFLAGS) $(WFLAGS) $< -o $@  $(LIBS)  
	
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
	rm -rf $(OBJDIR) $(BINDIR) $(SRCDIR)/*.sw* $(INCDIR)/*.sw* ./*.sw*




