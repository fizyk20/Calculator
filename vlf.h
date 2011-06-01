#ifndef __VLF_H__
#define __VLF_H__

#include <vector>
#include <string>
#include <sstream>
using namespace std;

#define ___int64 long long

typedef unsigned long ulong;

#define PI 3.1415926535897

extern ulong divlimit;
void SetDivLimit(ulong);  //sets division precision limit

class vlf	//floating point numbers (base 1000000000)
{
	vector<ulong> b; //b[0] - integer part
	long e;	//exponent
	char sgn; //sign

	void AddDigit(ulong); //adds a trailing digit
	void ShiftR(int); //shifts the number right (changing exponent too)
	void Expand(int); //adds trailing zeros

	bool valid;
public:
	vlf();
	vlf(float);
	vlf(double);
	vlf(int);
	vlf(ulong);
	vlf(const vlf&);
	~vlf();

	void Normalize();	//cuts leading and trailing zeros
	bool isValid();

//operators
	vlf& operator=(const vlf&);
	vlf operator+(const vlf&) const;
	vlf operator+=(const vlf&);
	vlf operator-(const vlf&) const;
	vlf operator-=(const vlf&);
	vlf operator*(const vlf&) const;
	vlf operator*=(const vlf&);
	vlf operator/(const vlf&) const;
	vlf operator/=(const vlf&);
	vlf operator^(const int&) const;
	vlf operator^=(const int&);

//	ulong operator[](const unsigned int);

	bool operator>(const vlf&);
	bool operator>=(const vlf&);
	bool operator<(const vlf&);
	bool operator<=(const vlf&);
	bool operator==(const vlf&);
	bool operator!=(const vlf&);

	string toString();
	operator double();

	void Round();

	friend vlf floor(vlf);
	friend vlf frac(vlf);
	friend vlf abs(vlf);

	friend vlf exp(vlf);
	friend vlf ln(vlf);

	friend vlf sin(vlf);
	friend vlf cos(vlf);

	friend vlf sinh(vlf);
	friend vlf cosh(vlf);

	//friend vlf asin(vlf);
	friend vlf atan(vlf arg);
	friend vlf atan2(vlf,vlf);
	
	friend vlf asinh(vlf arg);
	friend vlf acosh(vlf arg);
	friend vlf atanh(vlf arg);

	friend vlf NaN();
};

vlf sqrt(vlf);

vlf floor(vlf);
vlf frac(vlf);
vlf abs(vlf);

vlf exp(vlf);
vlf ln(vlf);

vlf sin(vlf);
vlf cos(vlf);

vlf sinh(vlf);
vlf cosh(vlf);

vlf atan(vlf arg);
vlf atan2(vlf,vlf);
	
vlf asinh(vlf arg);
vlf acosh(vlf arg);
vlf atanh(vlf arg);

vlf NaN();

#endif
