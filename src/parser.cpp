#include "parser.h"
#include "variables.h"

bool error;
Complex last_result;

bool Error()
{
	bool er = error;
	error = false;
	return er;
}

Complex setprec(Complex arg)
{
	int arg1 = (int)(double)arg;
	if(arg1<2) arg1=2;
	SetDivLimit(arg1);
	return Complex(1);
}

struct calc_function {
	string name;
	CALC_FUN fun;
};

calc_function* functions;
unsigned n_functions;

struct calc_constant {
	string name;
	CONST_FUN value;
};

calc_constant* constants;
unsigned n_constants;

Complex ans()
{
	return last_result;
}

void init_function_table()
{
	n_functions = 20;
	functions = new calc_function[n_functions];

	functions[0].name = "setprec";
	functions[0].fun = setprec;

	functions[1].name = "abs";
	functions[1].fun = abs;
	functions[2].name = "real";
	functions[2].fun = creal;
	functions[3].name = "imag";
	functions[3].fun = cimag;

	functions[4].name = "exp";
	functions[4].fun = cexp;
	functions[5].name = "ln";
	functions[5].fun = cln;

	functions[6].name = "sqrt";
	functions[6].fun = csqrt;
	functions[7].name = "cbrt";
	functions[7].fun = ccbrt;

	functions[8].name = "sin";
	functions[8].fun = csin;
	functions[9].name = "cos";
	functions[9].fun = ccos;
	functions[10].name = "tan";
	functions[10].fun = ctan;

	functions[11].name = "asin";
	functions[11].fun = casin;
	functions[12].name = "acos";
	functions[12].fun = cacos;
	functions[13].name = "atan";
	functions[13].fun = catan;
	
	functions[14].name = "sinh";
	functions[14].fun = csinh;
	functions[15].name = "cosh";
	functions[15].fun = ccosh;
	functions[16].name = "tanh";
	functions[16].fun = ctanh;
	
	functions[17].name = "asinh";
	functions[17].fun = casinh;
	functions[18].name = "acosh";
	functions[18].fun = cacosh;
	functions[19].name = "atanh";
	functions[19].fun = catanh;

	n_constants = 4;
	constants = new calc_constant[n_constants];

	constants[0].name = "ans";
	constants[0].value = ans;
	constants[1].name = "pi";
	constants[1].value = cpi;
	constants[2].name = "e";
	constants[2].value = ce;
	constants[3].name = "phi";
	constants[3].value = cphi;
}

int skip_parentheses(string expr,int start)	//returns the position of the closing parenthesis
{
	unsigned i;
	int depth = 0;

	for(i=start;i<expr.length();i++)
	{
		if(expr[i]=='(') depth++;
		if(expr[i]==')') depth--;
		if(depth==0) break;
	}
	if(i==expr.length()) {
		i=0;
		error = true;
	}
	return i;
}

Complex strToComplex(string expr)
{
	bool after_decimal = false;
	bool imag = false;
	int a = 1;
	vlf res(0);
	unsigned i;
	for(i=0;i<expr.length();i++)
	{
		if(expr[i]<'0' || expr[i]>'9') { //check if it's a proper character
			if(expr[i]=='i')
			{
				if(i<expr.length()-1)
				{
					error = true;
					return Complex();
				}
				imag = true;
				break;
			}
			if(expr[i]!='.' || after_decimal) {
				error = true;
				return Complex();
			} else {
				after_decimal = true;
				continue;
			}
		}

		if(!after_decimal) {
			res *= 10;
			res += (int)(expr[i]-48);
		} else {
			vlf ten(10);
			res += vlf((int)(expr[i]-48))/(ten^a);
			a++;
		}
	}
	if(imag) return Complex(vlf(0),res);
	else return Complex(res);
}

Complex level3(string expr)
{
	if(expr.length()==0) return Complex();
	if(expr[0]=='(')
	{
		if(expr[expr.length()-1]==')')
			return parse_expression(expr.substr(1,expr.length()-2));
		else {
			error = true;
			return Complex();
		}
	}
	if(expr=="i") return i();
	if(expr[0]>='0' && expr[0]<='9') //number
	{
		return strToComplex(expr);
	}
	if( (expr[0]>='a' && expr[0]<='z') || (expr[0]>='A' && expr[0]<='Z') ) //letter = name of a function
	{
		unsigned i;
		for(i=0;i<expr.length();i++)
		{
			if(expr[i]=='(') break;
			if( (expr[i]<'a' || expr[i]>'z') && (expr[i]<'A' || expr[i]>'Z') )
			{
				error = true;
				break;
			}
		}

		if(i<expr.length())	//function
		{
			Complex arg = parse_expression(expr.substr(i+1,expr.length()-i-2));
			if(error) return Complex();
			string name = expr.substr(0,i);
			for(i=0;i<n_functions;i++)
				if(functions[i].name==name) return functions[i].fun(arg);
		}
		else	//constant
		{
			for(i=0;i<n_constants;i++)
				if(constants[i].name==expr) return constants[i].value();

			//if we are here, there was no constant matching the name
			if(!variables.exists(expr))
			{
				error = true;
				return Complex();
			}
			return variables.var(expr);
		}
	}
	error=true;
	return Complex();
}

Complex level2(string expr)
{
	unsigned i=0,start=0;
	string arg;
	Complex res(0);
	bool first = true;

	while(i<=expr.length())
	{
		if(expr[i]=='(') i = skip_parentheses(expr,i);
		if(error) return Complex();
		if(expr[i]=='^' || i==expr.length())
		{
			arg = expr.substr(start,i-start);
			if(first)
			{
				res = level3(arg);
				first = false;
			}
			else
			{
				Complex carg = level3(arg);
				if(error) return Complex();
				if(carg == cfloor(carg) && (carg.tovlf()) < (vlf(2)^32)) res ^= (ulong)(double)carg;
				else res ^= carg;
			}
			if(error) return Complex();
			start = i+1;
		}
		i++;
	}

	return res;
}

Complex level1(string expr)
{
	unsigned i=0,start=0;
	string arg;
	Complex res(1);
	bool neg = false;

	while(i<=expr.length())
	{
		if(expr[i]=='(') i = skip_parentheses(expr,i);
		if(error) return Complex();
		if(expr[i]=='*' || expr[i]=='/' || i==expr.length())
		{
			arg = expr.substr(start,i-start);
			if(neg) res /= level2(arg);
			else res *= level2(arg);
			if(error) return Complex();
			if(expr[i]=='/') neg = true;
			else neg = false;
			start = i+1;
		}
		i++;
	}

	return res;
}

Complex level0(string expr)
{
	unsigned i=0,start=0;
	string arg;
	Complex res;
	bool neg = false;

	while(i<=expr.length())
	{
		if(expr[i]=='(') i = skip_parentheses(expr,i);
		if(error) return Complex();
		if(expr[i]=='+' || expr[i]=='-' || i==expr.length())
		{
			arg = expr.substr(start,i-start);
			if(neg) res -= level1(arg);
			else res += level1(arg);
			if(error) return Complex();
			if(expr[i]=='-') neg = true;
			else neg = false;
			start = i+1;
		}
		i++;
	}

	res.Normalize();
	return res;
}

Complex parse_expression(string expr)
{
	int i = expr.find('=');
	if(i==string::npos) return level0(expr);

	if( (expr[0]>='A' && expr[0]<='Z') || (expr[0]>='a' && expr[0]<='z') )
	{
		string name = expr.substr(0,i);
		Complex val = parse_expression(expr.substr(i+1));
		variables.assign(name, val);
		return val;
	}
	else
	{
		error = true;
		return Complex();
	}
}