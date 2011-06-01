#ifndef __VARIABLES__
#define __VARIABLES__

#include <map>
#include <string>
#include "vlfcomplex.h"

using namespace std;

class Variables
{
	map<string,Complex> vars;
public:
	Variables();
	~Variables();

	bool exists(string);
	Complex var(string);
	void assign(string,Complex);
	void list();
};

extern Variables variables;

#endif