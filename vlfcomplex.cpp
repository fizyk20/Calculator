#include "vlfcomplex.h"

//----------------------Constructors---------------------------

Complex::Complex()
{
	a = vlf(0);
	b = vlf(0);
	valid = true;
}

Complex::Complex(vlf a1)
{
	a = a1;
	b = vlf(0);
	valid = a1.isValid();
}

Complex::Complex(vlf a1, vlf b1)
{
	a = a1;
	b = b1;
	valid = a1.isValid() && b1.isValid();
}

Complex::Complex(const Complex& arg)
{
	a = arg.a;
	b = arg.b;
	valid = arg.valid;
}

Complex::~Complex()
{
}

void Complex::Normalize()
{
	a.Normalize();
	b.Normalize();
}

bool Complex::isValid()
{
	return valid;
}

//----------------------Operators---------------------------

Complex& Complex::operator=(const Complex& arg)
{
	a = arg.a;
	b = arg.b;
	valid = arg.valid;
	return (*this);
}

Complex Complex::operator+(const Complex& arg)
{
	if(!valid || !arg.valid) return CNaN();

	Complex res;
	res.a = a + arg.a;
	res.b = b + arg.b;
	return res;
}

Complex Complex::operator+=(const Complex& arg)
{
	if(!valid || !arg.valid)
	{
		valid = false;
		Normalize();
		return CNaN();
	}
	vlf a1,b1;
	a1 = a + arg.a;
	b1 = b + arg.b;
	a = a1;
	b = b1;
	return (*this);
}

Complex Complex::operator-(const Complex& arg)
{
	if(!valid || !arg.valid) return CNaN();

	Complex res;
	res.a = a - arg.a;
	res.b = b - arg.b;
	return res;
}

Complex Complex::operator-=(const Complex& arg)
{
	if(!valid || !arg.valid)
	{
		valid = false;
		Normalize();
		return CNaN();
	}

	vlf a1,b1;
	a1 = a - arg.a;
	b1 = b - arg.b;
	a = a1;
	b = b1;
	return (*this);
}

Complex Complex::operator*(const Complex& arg)
{
	if(!valid || !arg.valid) return CNaN();

	Complex res;
	res.a = a*arg.a - b*arg.b;
	res.b = b*arg.a + a*arg.b;
	return res;
}

Complex Complex::operator*=(const Complex& arg)
{
	if(!valid || !arg.valid)
	{
		valid = false;
		Normalize();
		return CNaN();
	}

	vlf a1,b1;
	a1 = a*arg.a - b*arg.b;
	b1 = b*arg.a + a*arg.b;
	a = a1;
	b = b1;
	return (*this);
}

Complex Complex::operator/(const Complex& arg)
{
	if(!valid || !arg.valid) return CNaN();

	Complex res;
	vlf abs;
	abs = arg.a*arg.a + arg.b*arg.b;
	if(abs == vlf(0)) return CNaN();
	res.a = (a*arg.a + b*arg.b)/abs;
	res.b = (b*arg.a - a*arg.b)/abs;
	return res;
}

Complex Complex::operator/=(const Complex& arg)
{
	vlf abs;
	vlf a1,b1;
	abs = arg.a*arg.a + arg.b*arg.b;

	if(!valid || !arg.valid || abs == vlf(0))
	{
		valid = false;
		Normalize();
		return CNaN();
	}

	a1 = (a*arg.a + b*arg.b)/abs;
	b1 = (b*arg.a - a*arg.b)/abs;
	a = a1;
	b = b1;
	return (*this);
}

Complex Complex::operator^(const int& arg) const
{
	if(!valid) return CNaN();

	Complex temp(*this);
	int ex=(arg>0)?arg:-arg;
	Complex res(1.0);
	while(ex>0)
	{
		if(ex%2==1)
		{
			res*=temp;
		}
		ex/=2;
		temp*=temp;
	}
	if(arg<0) return (Complex(1)/res);
	else return res;
}

Complex Complex::operator^=(const int& arg)
{
	(*this)=(*this)^arg;
	return(*this);
}

Complex Complex::operator^(const Complex& arg)
{
	return cexp(cln(*this)*arg);
}

Complex Complex::operator^=(const Complex& arg)
{
	(*this) = cexp(cln(*this)*arg);
	return (*this);
}

bool Complex::operator==(const Complex& arg)
{
	return (a==arg.a && b==arg.b);
}

string Complex::toString()
{
	if(!valid) return "NaN";

	string res_s="";
	res_s += a.toString();
	if(b > vlf(0)) res_s += "+";
	if(b != vlf(0))
	{
		res_s += b.toString();
		res_s += "i";
	}
	return res_s;
}

Complex::operator double()
{
	return (double)a;
}

vlf Complex::tovlf()
{
	return a;
}

//----------------------Functions---------------------------

Complex i()
{
	return Complex(0,1);
}

Complex i(vlf n)
{
	return Complex(0,n);
}

vlf real(Complex z)
{
	return z.a;
}

vlf imag(Complex z)
{
	return z.b;
}

Complex CNaN()
{
	Complex res;
	res.valid = false;
	res.Normalize();
	return res;
}

Complex cfloor(Complex arg)
{
	if(!arg.valid) return CNaN();

	return Complex(floor(real(arg)),0);
}

Complex abs(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(imag(arg)==vlf(0))
	{
		return abs(real(arg));
	}
	vlf abs2 = (real(arg)^2) + (imag(arg)^2);
	Complex res = Complex(exp(ln(abs2)/vlf(2)),0);
	return res;
}

Complex creal(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	return Complex(real(arg),0);
}

Complex cimag(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	return Complex(imag(arg),0);
}

Complex cexp(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(imag(arg)==vlf(0))
	{
		return Complex(exp(real(arg)));
	}
	Complex res = Complex(exp(real(arg))*cos(imag(arg)),exp(real(arg))*sin(imag(arg)));
	return res;
}

Complex cln(Complex arg)
{
	if(!arg.isValid() || arg == Complex()) return CNaN();

	if(imag(arg)==vlf(0) && real(arg) > vlf(0))
	{
		return Complex(ln(real(arg)));
	}
	Complex abs1 = abs(arg);
	Complex res = Complex(ln(real(abs1)),atan2(imag(arg),real(arg)));
	return res;
}

Complex csqrt(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(arg == Complex()) return Complex();
	return cexp(cln(arg)/Complex(2));
}

Complex ccbrt(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	return cexp(cln(arg)/Complex(3));
}

Complex csin(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(imag(arg)==vlf(0))
	{
		return Complex(sin(real(arg)));
	}
	return (cexp(arg*i())-cexp(arg*i(-1)))/Complex(0,2);
}

Complex ccos(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(imag(arg)==vlf(0))
	{
		return Complex(cos(real(arg)));
	}
	return (cexp(arg*i())+cexp(arg*i(-1)))/Complex(2,0);
}

Complex ctan(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	return csin(arg)/ccos(arg);
}

Complex csinh(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(imag(arg)==vlf(0))
	{
		return Complex(sinh(real(arg)));
	}
	return (cexp(arg)-cexp(arg*Complex(-1)))/Complex(2,0);
}

Complex ccosh(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(imag(arg)==vlf(0))
	{
		return Complex(cosh(real(arg)));
	}
	return (cexp(arg)+cexp(arg*Complex(-1)))/Complex(2,0);
}

Complex ctanh(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	return csinh(arg)/ccosh(arg);
}

Complex casin(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	return vlf(0);
}

Complex cacos(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	return vlf(0);
}

Complex catan(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(imag(arg)==vlf(0))
	{
		return Complex(atan(real(arg)));
	}
	return i(-1)*cln((i()-arg)/(i()+arg))/Complex(2);
}

Complex casinh(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(imag(arg)==vlf(0))
	{
		return Complex(asinh(real(arg)));
	}
	return cln(arg+csqrt((arg^2)+Complex(1)));
}

Complex cacosh(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	if(imag(arg)==vlf(0) && real(arg)>=vlf(1))
	{
		return Complex(acosh(real(arg)));
	}
	return cln(arg+csqrt((arg^2)-Complex(1)));
}

Complex catanh(Complex arg)
{
	if(!arg.isValid()) return CNaN();

	return vlf(0);
}

//------------------------------------------------------------

Complex cpi()
{
	return Complex(atan(vlf(1))*vlf(4));
}

Complex ce()
{
	return Complex(vlf(exp(1)));
}

Complex cphi()
{
	return Complex((sqrt(vlf(5))+vlf(1))/vlf(2));
}