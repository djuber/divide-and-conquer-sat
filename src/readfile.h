#ifndef READFILE_H
#define READFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "CNF.h"

void create_empty_clauses(problem phi);

void 
process_string_in_problem
(std::string s, problem phi, int& clause_count);

problem problem_from_problem_line(std::string s);

problem read_problem_from_stream(std::istream &input);

problem read_problem_file(const char* flename);

#endif 
