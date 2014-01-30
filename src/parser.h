#ifndef __PARSER__
#define __PARSER__

#include "vlfcomplex.h"
#include <iostream>
#include <string>

using namespace std;

typedef Complex (*CALC_FUN)(Complex);
typedef Complex (*CONST_FUN)(void);

void init_function_table();
Complex parse_expression(string);
bool Error();

#endif