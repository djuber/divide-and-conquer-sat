/** CS408 Project 1
    D&C 3-Sat Algorithm
    Daniel Uber
*/

#include "CNF.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cassert>

/** 
    main()
    verify an argument was provided.
    read problem from file, and pass to the satisfiable function.
    if problem is not satisfiable, print NO, and exit.
*/
int main(int argc, char** argv){
  if(argc < 2)
    usage(argv);
  else if(!satisfiable(read_problem_file(argv[1])))
    std::cout<<"NO"<<std::endl;
  return 0; 
}

/** 
    usage(args)
    print message reminding user to pass a filename when calling
 */
void usage(char** supplied_args){
  std::cout<<"Usage: "<< supplied_args[0]<<" filename"<<std::endl
	   <<"Where filename is the name of a cnf problem file."
	   <<std::endl;
  return;
}

/** 
    satisfiable(phi)
    given a non-null phi, implement divide and conquer strategy.
    if phi is satisfied, print satisfying assignment and return true
    otherwise, introduce a variable assignment to phi and recur
    if no satisfying assignment is found, return false
 */
bool satisfiable(problem phi){
  if(phi){ // skip over null problems
    if (already_satisfied(phi)){ // phi is satisfied
      print_assignment(phi);
      release_problem(phi);
      return true;
    } else if 
	(satisfiable(introduce(phi, first(shortest(phi), phi), TRUE)) || 
	 satisfiable(introduce(phi, first(shortest(phi), phi), FALSE)))
      {
	release_problem(phi);
	return true;
      } else  // phi is valid but not satisfied
      {
	release_problem(phi);
      }
  }
  return false;
}

/**
   already_satisfied(phi)
   if there are no clauses with free variables, then phi has been satisfied.
 */
bool already_satisfied(problem phi){
  for(int index = 0; index < phi->clause_count; index++)
    if(phi->clause_length[index])
      return false;
  return true;
}


/**
   shortest(phi)
   return the first non-empty clause in phi of minimum length
 */
clause shortest(problem phi){
  int result = 0;
  // skip over any satisfied clauses at the beginning
  while(phi->clause_length[result] == 0)
    result++;
  // identify the shortest unsatisfied clause
  for(int index = result+1 ; index < phi->clause_count ; index++)
    if(phi->clause_length[index] && // only non-empty clauses matter
       phi->clause_length[index] < phi->clause_length[result])
      if(phi->clauses[index])
	result = index;
  return phi->clauses[result];
}

/** first(clause, problem)
    clause c is a non-empty clause in problem phi
    first returns the first variable number present in that clause
 */
int first(clause c, problem phi){
  for(int index = 0; index < phi->variable_count ; index++)
    if(c && c[index])
      return index;
  // never get here in a working program, but muffle the compiler
  // and warn the user that something went wrong
  std::cerr
    <<"In call to first(clause, problem), no literals in clause"
    <<std::endl;
  return 0;
}

  
/**
   print_assignment(phi)
   if phi has been satisfied, we can read of the variables from the variable
   list, and print them as our result.
   if some variables were not assigned in the process of satisfying phi
   then they are free, and we assign them true.
 */
void print_assignment(problem phi){
  for(int index = 0; index < phi->variable_count; index++) {
    std::cout<<std::setw(4)
      <<(index + 1)*(phi->variables[index]? phi->variables[index] : 1)
      <<"  "; 
    if(!((index+1) % 10))
      std::cout<<std::endl;
  }
  std::cout<<std::endl<<std::endl;
  return;
}

/**
   empty_form_of_size(clauses, variables)
   allocate a problem structure and its lists
   initialize all values to 0. (unassigned/not-present)
   return pointer to the new problem
 */
problem empty_form_of_size(int clauses, int variables){
  problem rho = (problem)malloc(sizeof(problem_struct));
  rho->variable_count = variables;
  rho->clause_count = clauses;
  rho->clause_length = (length_list) calloc(clauses, sizeof(int));
  rho->clauses = (form)calloc(clauses, sizeof(clause));
  rho->variables = (variable_list) calloc(variables, sizeof(assignment));
  rho->fresh_clause = (fresh_list) calloc(clauses, sizeof(bool));
  // removed for sharing : use pointers to parent instead.
  // for(int index=0; index < clauses; index++)
  //   rho->clauses[index] = (clause)calloc(variables, sizeof(assignment));
  return rho;
}

/**
   release_problem(phi)
   free() allocated resources for the problem struct.
 */
// sharing : only free memory we allocated for fresh clauses
// i.e. don't damage parent!
void release_problem(problem phi){
  if(phi){ // don't try to look inside a null pointer
    free(phi->variables);
    free(phi->clause_length);
    for(int i = 0; i < phi->clause_count; i++){
      if(phi->fresh_clause[i] && phi->clauses[i]) // only free memory we own
	free(phi->clauses[i]);
    }
    free(phi->clauses);
    free(phi->fresh_clause);
    free(phi);
  }
  return;
}

/**
   copy_variables(from, to)
   given from and to of the same size, 
   copy variable assignment from from to to.
 */
void copy_variables(problem from, problem to) {
  assert(from->variable_count == to->variable_count);
  for(int index=0; index < to->variable_count; index++)
    to->variables[index] = from->variables[index];
  return;
}

/**
   copy_clauses(from, to)
   given from and to of the same size
   copy clauses from from to to
   and set clause lengths in to to match from
 */
// sharing feature : copy pointer instead of new arrays
void copy_clauses(problem from, problem to){
  for(int clause_index = 0; clause_index < from->clause_count; 
      clause_index++){
    to->clauses[clause_index] = from->clauses[clause_index];
    to->clause_length[clause_index] = from->clause_length[clause_index];
    to->fresh_clause[clause_index]=false;
  }
  return;
}
/**
copy_clause(from, size)
create a new clause array, and copy existing values into it.
 */
clause copy_clause(clause from, int size){
  clause to = (clause)calloc(size, sizeof(assignment));
  for(int i = 0; i < size; i++)
    to[i] = from[i];
  return to;
}

/** 
    copy_problem(from)
    allocate a new problem structure of same size, 
    copy variables and clauses into it, and return the pointer.
*/
problem copy_problem(problem from){
  problem to = empty_form_of_size(from->clause_count, from->variable_count);
  copy_variables(from, to);
  copy_clauses(from, to);
  return to;
}

/**
   introduce(phi, variable, a)
   create a new problem based on phi, with variable assigned value a
   simplify satisfied clauses, and remove variables which are not satisfied
   from the clauses, updating clause_length for each clause changed.
   return the modified problem.
   In the scope of the program, this is where memory is allocated.
   All problems alloc()'d here are free()'d in satisfiable
   If introducing the variable makes some clause empty,
   we abort and return NULL.
 */
// sharing : added constant true clause, and 
// copy clause before changing it, updating fresh
problem introduce(problem phi, int variable, assignment a){
  problem rho = copy_problem(phi);
  rho->variables[variable] = a;
  for(int clause_index = 0; clause_index < rho->clause_count; clause_index++)
{
  if(rho->clause_length[clause_index]){
    if(rho->clauses[clause_index][variable] == a) {
      rho->clauses[clause_index] = the_true_clause;
      rho->clause_length[clause_index] = 0;
    }
    else if (rho->clauses[clause_index][variable] == -a){
      if(rho->clause_length[clause_index] == 1){
	// making a clause unsatisfiable, fail
	release_problem(rho);
	return (problem) NULL;
      }
      rho->clauses[clause_index] = copy_clause(phi->clauses[clause_index], rho->clause_count);
      rho->fresh_clause[clause_index]=true;
      rho->clauses[clause_index][variable] = UNASSIGNED;
      rho->clause_length[clause_index] -= 1;
    }
  }
 }
  return rho;
}


/** print_form(phi)
    dumps struct info to standard output
    for debugger inspection
    in gdb: call print_form(phi) where phi is in the local context
*/
void print_form(problem phi){
  std::cout<<"variables: ";
  for(int i = 0; i < phi->variable_count; i++)
    std::cout<<i+1<<":"<<(int)phi->variables[i]<<" ";
  std::cout<<std::endl;
    
  for(int c = 0; c < phi->clause_count; c++){
    if(phi->clauses[c]){
      for(int v = 0; v < phi->variable_count; v++)
	if( phi->clauses[c][v]) // skip unassigned
	  std::cout<<c+1<<":"<<v+1<<":"<<(int)phi->clauses[c][v]<<" ";
      std::cout<<std::endl;
    }
  }
  return;
}
