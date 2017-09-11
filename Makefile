GXX=g++ -fPIC
WGXX =x86_64-w64-mingw32-g++  -fPIC 
CFLAGS= -c -std=c++11 -I $(matlabroot)/extern/include/ 
IFLAGS = -I /usr/local/MATLAB/new-R2017a/include 
WFLAGS= -Wall -Wextra 
LDFLAGS= -g
LIBS=  -L$(matlabroot)/bin/glnxa64  -leng -lmx -lmat  
WLIBS = -L/usr/local/MATLAB/new-R2017a/win64/ -leng -lmx -lmat  

matlabroot= /usr/local/MATLAB/R2017a
SRCDIR = ./src
OBJDIR = ./obj
LINDIR = ./linux
INCDIR = ./include
BINDIR = ./bin
TESTDIR = ./tests

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
INCFILES =  $(wildcard $(INCDIR)/*.hpp)
OBJLINFILES=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/$(LINDIR)/%.o, $(SRCFILES))

LINEXT= mexa64
SSPM = $(BINDIR)/sspm
EXECTEST = $(TESTDIR)/test

all: exec
	
exec : $(OBJLINFILES)
	$(GXX) -g -o $(SSPM) $(OBJLINFILES)  -Wl,-rpath-link,$(matlabroot)/bin/glnxa64 -lstdc++  -lm  $(LIBS) 
	mex  mexGate.cpp  -output $(SSPM).$(LINEXT) $(OBJLINFILES) $(LIBS) 
	x86_64-w64-mingw32-g++ -std=c++11 -static-libgcc -static-libstdc++ -m64 -shared  -I $(matlabroot)/extern/include/  -DMATLAB_MEX_FILE -L/usr/x86_64-w64-mingw32/lib/ $(WLIBS) -o bin/main.mexw64 mexGate.cpp src/*.cpp -leng -lmx -lmex


$(OBJDIR)/$(LINDIR)/%.o :  $(SRCDIR)/%.cpp $(OBJDIR)  
	$(GXX) $(CFLAGS) $(LDFLAGS) $(WFLAGS) $< -o $@   $(LIBS)  
	

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/$(LINDIR)
	mkdir -p $(BINDIR)

test: $(EXECTEST)


$(EXECTEST): ./obj/daphnia.o ./obj/model.o ./obj/mmi.o ./tests/test.o
	$(GXX)  $^ -o $@

$(TESTDIR)/test.o: $(TESTDIR)/test.cpp $(INCFILES)
	$(GXX) $(CFLAGS) $(LDFLAGS) $(WFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(SRCDIR)/*.sw* $(INCDIR)/*.sw* ./*.sw*




