/**
CS408 Project 1
Daniel Uber
D&C Sat Algorithm

readfile.cc : read problem from cnf file
provides read_problem_file

 */

#include "CNF.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdlib>

/**
   dummy_problem(size)
   stub out read_problem_file
   create a test case to verify algorithm
   this is not the best case, maybe (i < j)?FALSE:TRUE would be better.
   used before read_problem_file was working.
 */
problem dummy_problem(const int size) 
{
  // temporary dummy function for testing
  // any false assignment will satisfy
  problem phi = empty_form_of_size(size, size);
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++)
      phi->clauses[j][i]=FALSE; 
    phi->clause_length[i]  = size;
  }
  return phi;
}

void create_empty_clauses(problem phi){
  for(int index = 0; index < phi->clause_count; index++){
    phi->clauses[index] = (clause)calloc(phi->variable_count, sizeof(assignment));
    phi->fresh_clause[index]=true;
  }
  return;
}



/** 
    read_problem_file(filename)
    open filename, skip comments
    read number of clauses and variables from the problem line
    if the problem is not cnf, abort, since this only handles cnf files.
    create a new problem of the proper size
    read each clause line, and set clause variables for it
    update clause length when setting variable
    when reading zero, advance to next clause
    finally, return the initialized problem.
 */
// sharing feature : need to create clause arrays in root form
problem read_problem_file(const char* filename) {
  std::ifstream input;
  std::string s;
  problem phi;
  int clause_count = 0;
  int specified_variable_count, specified_clause_count;
  input.open(filename);
// outline : find p line, read number of variables and number of clauses 
// allocate a problem of that size
// then seek to first non-comment line and begin reading clauses.
  if(input.is_open()){
    while(input.good()){
      getline(input, s);
      if(s[0]!='c'){ // skip comment lines.
	if(s[0] == 'p') {
	  std::string pchar, cnfword;
	  std::istringstream iss(s);
	  iss>>pchar
	     >>cnfword
	     >>specified_variable_count
	     >>specified_clause_count;
	  if(cnfword != "cnf") {
	    std::cerr<<"Attempting to read problem of type: "
		     <<cnfword<<std::endl;
	    return (problem)NULL;
	  }
	  phi = empty_form_of_size(specified_clause_count, 
				   specified_variable_count);
	  create_empty_clauses(phi);

        } else if (s[0] != '%') {
	  // s is not a comment, a problem statement, or a percentage sign
	  // so it must be clause information : read it
	  if(phi){
	    int var;
	    std::istringstream iss(s);
	    while(iss.good()){
	      iss>>var;
	      if(var != 0) {
		assert(clause_count < specified_clause_count);
		// transform integer in input into offset and assignment
		phi->clauses[clause_count][abs(var) -1] = 
		  (var > 0)?TRUE : FALSE;
		phi->clause_length[clause_count] += 1;
	      } else if(var == 0) { clause_count++; }
	    } // reading line
	  } // phi is not null
	  else {
	    std::cerr<<"trying to access uncreated problem form"
		     <<std::endl
		     <<"This is likely a malformed file."<<std::endl;
	  }
	} // string is not a comment
      } // while input available
    }      
    input.close();
  } else {
    std::cerr<<"file "<< filename<<" not found."<<std::endl;
    return (problem)NULL;
  }
  //    return dummy_problem(10);
  return phi;
}

