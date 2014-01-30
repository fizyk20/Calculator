#ifndef __VLFCOMPLEX__
#define __VLFCOMPLEX__

#include "vlf.h"
#include <string.h>
#include <iostream>
#include <string>

using namespace std;

class Complex
{
	vlf a,b; //a is real part, b is imaginary part
	bool valid;
public:
	Complex();
	Complex(vlf);
	Complex(vlf,vlf);
	Complex(const Complex&);
	~Complex();

	void Normalize();
	bool isValid();

//operators
	Complex& operator=(const Complex&);
	Complex operator+(const Complex&);
	Complex operator+=(const Complex&);
	Complex operator-(const Complex&);
	Complex operator-=(const Complex&);
	Complex operator*(const Complex&);
	Complex operator*=(const Complex&);
	Complex operator/(const Complex&);
	Complex operator/=(const Complex&);
	Complex operator^(const int&) const;
	Complex operator^=(const int&);
	Complex operator^(const Complex&);
	Complex operator^=(const Complex&);

	bool operator==(const Complex&);

	vlf tovlf();
	operator double();

	string toString();

	friend Complex cfloor(Complex);

	friend vlf real(Complex);
	friend vlf imag(Complex);

	friend Complex CNaN();
};

Complex i();
Complex i(vlf);

Complex cpi();
Complex ce();
Complex cphi();

Complex abs(Complex);
Complex creal(Complex);
Complex cimag(Complex);

Complex cexp(Complex);
Complex cln(Complex);

Complex csqrt(Complex);
Complex ccbrt(Complex);

Complex csin(Complex);
Complex ccos(Complex);
Complex ctan(Complex);

Complex csinh(Complex);
Complex ccosh(Complex);
Complex ctanh(Complex);

Complex casin(Complex);
Complex cacos(Complex);
Complex catan(Complex);

Complex casinh(Complex);
Complex cacosh(Complex);
Complex catanh(Complex);

Complex cfloor(Complex);

vlf real(Complex);
vlf imag(Complex);

Complex CNaN();

#endif