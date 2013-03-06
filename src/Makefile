# CS408 Project 1
# Daniel Uber
headers = CNF.h
objects = CNF.o readfile.o
# the native unix compiler
CXX = g++
CXXFLAGS = -Wall -Werror -g

# dummy target
all: cnf

# the cnf executable
cnf: $(objects)
	$(CXX) $(CXXFLAGS) -o cnf $(objects)

# remove temp files and objects
clean:
	rm -f *.o *~ cnf ;

test: all
	sh test.sh

# utility targets to call windows makefile
windows:
	make -f Makefile.windows all

windows-test:
	make -f Makefile.windows test

windows-clean:
	make -f Makefile.windows clean
