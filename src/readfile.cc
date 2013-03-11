/**
CS408 Project 1
Daniel Uber
D&C Sat Algorithm

readfile.cc : read problem from cnf file
provides read_problem_file

 */

#include "CNF.h"
#include "readfile.h"
#include <cstdlib>


/**
create_empty_clauses(phi)
since empty_form_of_size no longer initializes clauses
when reading from file we need to allocate the arrays.
Function added to factor logic out of read_problem_file
 */
void
create_empty_clauses (problem phi)
{
  for (int index = 0; index < phi->clause_count; index++)
    {
      phi->clauses[index] =
	(clause) calloc (phi->variable_count, sizeof (assignment));
      phi->fresh_clause[index] = true;
    }
  return;
}

/**
process_string_in_problem(s,phi,count)
given an input string which is neither a comment, 
a problem line, nor a percent, read the integers in string 
into the clause designated by clause_count in phi
this function factors out behavior from read_problem_file
 */
void
process_string_in_problem (std::string s, problem phi, int &clause_count)
{
  if (phi)
    {
      int var;
      std::istringstream iss (s);
      while (iss.good ())
	{
	  iss >> var;
	  if (var != 0)
	    {
	      if (clause_count >= phi->clause_count)
		{
		  std::
		    cerr << "number of clauses exceeds expected value.\n" <<
		    std::
		    endl << "Details: \nInputLine: " << s <<
		    "\n clauses expected: " << phi->
		    clause_count << "\ncurrent clause_counter: " <<
		    clause_count << std::endl;
		  return;
		}
	      // transform integer in input into offset and assignment
	      phi->clauses[clause_count][abs (var) - 1] =
		(var > 0) ? TRUE : FALSE;
	      phi->clause_length[clause_count] += 1;
	    }
	  else if (var == 0)
	    {
	      clause_count++;
	    }
	}			// reading line
    }				// phi is not null
  else
    {
      std::cerr << "trying to access uncreated problem form"
	<< std::endl << "This is likely a malformed file." << std::endl;
    }
  return;
}

/**
   problem_from_problem_line(s)
   process string s containing problem description
   creates a new problem of the correct size 
   factoring out behavior in read_problem_file
*/
problem
problem_from_problem_line (std::string s)
{
  std::string pchar, cnfword;
  std::istringstream iss (s);
  problem phi = (problem) NULL;
  int variable_count, clause_count = 0;
  iss >> pchar >> cnfword >> variable_count >> clause_count;
  if (cnfword != "cnf")
    {
      std::cerr << "Attempting to read problem of type: "
	<< cnfword << std::endl;
      return (problem) NULL;
    }
  phi = empty_form_of_size (clause_count, variable_count);
  create_empty_clauses (phi);
  return phi;
}

/** read_problem_from_stream
 */
problem
read_problem_from_stream (std::istream & input)
{
  std::string s;
  problem phi = (problem) NULL;
  int clause_count = 0;
  // outline : find p line, read number of variables and number of clauses 
  // allocate a problem of that size
  // then seek to first non-comment line and begin reading clauses.
  while (input.good ())
    {
      getline (input, s);
      if (s[0] != 'c')
	{			// skip comment lines.
	  if (s[0] == 'p')
	    {
	      phi = problem_from_problem_line (s);
	    }
	  else if (!s.empty () && s[0] != '%')
	    process_string_in_problem (s, phi, clause_count);
	}			// while input available
    }
  return phi;
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
problem
read_problem_file (const char *filename)
{
  std::ifstream input;
  problem phi = (problem) NULL;
  input.open (filename);
  if (input.is_open ())
    {
      phi = read_problem_from_stream (input);
      input.close ();
    }
  else				// file not open
    std::cerr << "file " << filename << " not found." << std::endl;
  return phi;
}
