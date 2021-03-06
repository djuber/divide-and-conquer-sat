/** CS408 Project 1
    Daniel Uber
    D&C Sat Algorithm
*/
#ifndef CNF_H
#define CNF_H


// define types

// our possible assignment states for each variable
// all variables begin unassigned, and are set true or false
// by introduce()'ing them
enum assignment
{
  FALSE = -1,
  UNASSIGNED = 0,
  TRUE = 1
};

// variable list holds assignments in our problem to this point
typedef assignment *variable_list;
// the length list records the number of free variables in each clause
typedef int *length_list;
// fresh flags indicates whether a structure owns this clause
typedef bool *fresh_list;
// a clause is an array of assignments
typedef assignment *clause;
// a cnf formula is an array of clauses
typedef clause *form;

// a uniform way to represent satisfied clauses.
const clause the_true_clause = (clause) 0;

// problem_struct is the representation of a cnf form plus the 
// assigned interpretations, and information about the problem
// i.e. the number of variables and clauses, and the lengths of each clause
struct problem_struct
{
  variable_list variables;	// current interpretation
  form clauses;			// the cnf formula
  length_list clause_length;	// array mapping clauses to number of literals
  fresh_list fresh_clause;	// true when we've alloc'd this data
  int variable_count;		// variable array bound
  int clause_count;		// clauses, clause_length, and fresh_clause bound
};

// utility typedef
typedef struct problem_struct *problem;

// declarations

// print help information. Argv passed to show program name
void usage (char **args);

// main algorithm function. True if a satisfying assignment was found
bool satisfiable (problem phi);



// return the index of the shortest clause
clause shortest (problem phi);

// return the index of the first variable in the clause
int first (clause c, problem phi);

// test if all clauses have been satisfied 
bool already_satisfied (problem phi);

// print assigned variables, assigning TRUE to free variables
void print_assignment (problem phi);

// release allocated resources
void release_problem (problem phi);

// introduce and simplify with an assigned variable
problem introduce (problem phi, int variable, assignment a);

// problem creation/allocation utility functions
problem empty_form_of_size (int clauses, int variables);
void copy_variables (problem from, problem to);
void copy_clauses (problem from, problem to);
clause copy_clause (clause from, int size);

problem copy_problem (problem from, problem to);

// used for debugging
void print_form (problem phi);

#endif // CNF_H
