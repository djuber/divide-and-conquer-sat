# CS408 Project 1
# Daniel Uber
headers = CNF.h
objects = CNF.o readfile.o
winobjects = CNF.obj readfile.obj
# the native unix compiler
CXX = g++
CXXFLAGS = -Wall -Werror -g
# the windows cross compiler 
CXXWIN = i586-mingw32msvc-g++ 

# dummy target
all: unix windows

unix : cnf
windows: cnf.exe
# Unix Targets
# the cnf executable
cnf: $(objects)
	$(CXX) $(CXXFLAGS) -o cnf $(objects)

# remove temp files and objects
clean: clean-objects
	rm -f  *~ cnf cnf.exe;

clean-objects:
	rm -f *.o *.obj;

test: all
	sh test.sh

# Windows Targets
# use distinct object extension for PE objects
# add explicit rules since make only knows how to build .o
CNF.obj: $(headers) CNF.cc
	$(CXXWIN) $(CXXFLAGS) -o CNF.obj -c CNF.cc

readfile.obj: $(headers) readfile.cc
	$(CXXWIN) $(CXXFLAGS) -o readfile.obj -c readfile.cc

# the cnf executable for windows cnf.exe
cnf.exe: $(winobjects)
	$(CXXWIN) $(CXXFLAGS) -o cnf.exe $(winobjects)


windows-test: windows
	sh test-windows.sh	

zipfile: 
	sh package.sh
