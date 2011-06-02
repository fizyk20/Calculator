#include "vlfcomplex.h"
#include "parser.h"
#include <fstream>
#include <sstream>
#include "variables.h"
#include <time.h>

#define VER "0.8"

void help()
{
	cout << endl;
	cout << "This is Calculator v" << VER << " written by Fizyk." << endl;
	cout << endl;
	cout << "This calculator supports complex numbers. Use 'i' as the imaginary unit." << endl;
	cout << endl;
	cout << "Available features:" << endl;
	cout << "1. Operators: + - * / ^" << endl;
	cout << "2. Functions: " << endl;
	cout << "   abs, real, imag," << endl;
	cout << "   exp, ln, sqrt, cbrt," << endl;
	cout << "   sin, cos, tan, atan" << endl;
	cout << "   sinh, cosh, tanh, asinh" << endl;
	cout << "3. Constants: " << endl;
	cout << "   e, pi, phi" << endl;
	cout << "4. Variables " << endl;
	cout << "   You can define variables by writing name=value." << endl;
	cout << "   Name has to begin with a letter and it should not be identical" << endl;
	cout << "   to the name of any constant (\"e\",\"pi\" etc.)." << endl;
	cout << "5. setprec - it sets decimal precision to argument*9, so setprec(5) " << endl;
	cout << "   will give you 45 decimal places (don't ask why it's like that). " << endl;
	cout << "   Precision defaults to 8." << endl;
	cout << "   Beware with precision values higher than 20, the program starts to be really slow then " << endl;
	cout << "   (but *usually* it still works)." << endl;
	cout << endl;
	cout << "Commands:" << endl;
	cout << "help - prints this text" << endl;
	cout << "dump - dumps last request and result to a file" << endl;
	cout << "quit - quits the program" << endl;
	cout << endl;
	cout << "Have fun!" << endl;
	cout << endl;
}

Variables variables;

extern Complex last_result;

int main()
{
	SetDivLimit(8);

	string last_request;
	int file_no = 0;
	char line[5000];
	clock_t t;

	cout << "Calculator v" << VER << " by Fizyk" << endl;
	cout << "Type 'help' to get help." << endl;

	string expr;
	init_function_table();

	while(expr!="quit")
	{
		cout << "> ";
		cin.getline(line,5000);
		expr = string(line);
		if(expr=="help")
		{	
			help();
			continue;
		}
		if(expr=="quit") break;

		if(expr=="dump")
		{
			string name;
			name = "result_";
			stringstream ss;
			ss << file_no;
			name += ss.str();
			name += ".txt";
			file_no++;
			ofstream fout(name.c_str());
			string c_result = last_result.toString();
			fout << "Request: " << last_request << endl << endl << "Result:" << endl;
			fout << c_result << endl << endl;
			fout << "Calculated in: " << (double)t/CLOCKS_PER_SEC << " seconds." << endl;
			fout.close();
			cout << "Saved to file " << name << endl;
			continue;
		}

		if(expr=="list_vars")
		{
			variables.list();
			continue;
		}

		last_request = expr;

		//Remove spaces
		int i;
		while((i = expr.find(' ')) != string::npos)
		{
			expr.erase(i,1);
		}

		t = clock();

		Complex result = parse_expression(expr);

		t = clock() - t;

		if(Error()) {
			cout << "Parse error." << endl;
		} else {
			string c_result = result.toString();
			cout << c_result << endl;
		}
		last_result = result;
	}
	return 0;
}