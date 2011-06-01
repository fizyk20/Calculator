#include "variables.h"
#include "parser.h"
#include <fstream>

Variables::Variables()
{
	vars.clear();

	ifstream fin("variables.ini");

	if(!fin.fail())
	{
		while(!fin.eof())
		{
			string name,value;
			fin >> name;
			fin >> value;
			if(name != "") vars[name] = parse_expression(value);
		}

		fin.close();
	}
}

Variables::~Variables()
{
	ofstream fout("variables.ini");

	map<string,Complex>::iterator it;
	int x;

	for(it = vars.begin(); it != vars.end(); it++)
	{
		string c_result = it->second.toString();
		fout << it->first << " " << c_result << endl;
	}

	fout.close();

	vars.clear();
}

bool Variables::exists(string name)
{
	map<string,Complex>::iterator it;

	it = vars.find(name);
	if(it==vars.end())
		return false;
	else
		return true;
}

Complex Variables::var(string name)
{
	map<string,Complex>::iterator it;

	it = vars.find(name);
	if(it==vars.end())
		return CNaN();
	else
		return it->second;
}

void Variables::assign(string name, Complex value)
{
	vars[name] = value;
}

void Variables::list()
{
	map<string,Complex>::iterator it;
	for(it = vars.begin(); it != vars.end(); it++)
	{
		cout << it->first << " = " << it->second.toString() << endl;
	}
}