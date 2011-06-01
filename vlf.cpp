#include "vlf.h"
#include <math.h>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

ulong divlimit=10;

vlf::vlf()
{
	e=0;
	sgn=1;
	valid = true;
}

vlf::vlf(float x)
{
	sgn=1;
	if(x==0.0f)
	{
		b.push_back(0);
	}
	else
	{
		if(x<0.0)
		{
			sgn=-1;
			x*=-1.0;
		}
		e=(long)floor(log(x)/log(1000000000.0));
		x/=(float)pow(1000000000.0,(double)e);
		b.push_back((ulong)floor(x));
		x-=(float)floor(x);

		while(x>0)
		{
			x*=1000000000.0f;
			b.push_back((ulong)floor(x));
			x-=(float)floor(x);
		}
	}
	valid = true;
}

vlf::vlf(double x)
{
	sgn=1;
	if(x==0.0)
	{
		b.push_back(0);
		e=0;
	}
	else
	{
		if(x<0.0)
		{
			sgn=-1;
			x*=-1.0;
		}
		e=(long)floor(log(x)/log(1000000000.0));
		x/=pow(1000000000.0,(double)e);
		b.push_back((ulong)floor(x));
		x-=floor(x);	

		while(x>0)
		{
			x*=1000000000.0;
			b.push_back((ulong)floor(x));
			x-=floor(x);
		}
	}
	valid = true;
}

vlf::vlf(int a)
{
	sgn=1;
	if(a<0)
	{
		sgn=-1;
		a*=-1;
	}
	e=0;
	b.push_back((ulong)a);
	valid = true;
}

vlf::vlf(ulong a)
{
	sgn=1;
	e=0;
	b.push_back(a);
	valid = true;
}

vlf::vlf(const vlf& arg)
{
	e=arg.e;
	sgn=arg.sgn;
	b = arg.b;
	valid = arg.valid;
}

vlf::~vlf()
{
	e=0;
	sgn=1;
	valid = false;
}

void vlf::AddDigit(ulong d)
{
	b.push_back(d);
}

void vlf::Normalize()
{
	if(!valid)
	{
		b.clear();
		e = 0;
		sgn = 1;
		return;
	}

	unsigned i;
	if(b.size()==0) AddDigit(0);
	while(b[0]==0&&b.size()>1)
	{
		b.erase(b.begin());
		e--;
	}
	int z=0;
	i=b.size()-1;
	while(i>0 && b[i]==0)
	{
		z++;
		i--;
	}
	if(z>0)
	{
		unsigned n_b = b.size();
		n_b-=z;
		if(n_b > divlimit+1) n_b=divlimit+1;
		b.erase(b.begin()+n_b,b.end());
	}

	if(b.size()==1 && b[0]==0)
	{
		sgn = 1;
		e = 0;
	}
}

void vlf::ShiftR(int d)
{
	int i;
	if(d<=0) return;
	for(i=0; i < d; i++)
		b.insert(b.begin(),0);
	e+=d;
}

void vlf::Expand(int n)
{
	if(n<=0) return;
	int i;
	for(i=0; i<n; i++)
		b.push_back(0);
}

bool vlf::isValid()
{
	return valid;
}

//-----------------------operatory-------------------------

vlf& vlf::operator=(const vlf& arg)
{
	e=arg.e;
	sgn=arg.sgn;
	b = arg.b;
	valid = arg.valid;
	return(*this);
}

vlf vlf::operator+(const vlf& arg) const
{
	if(!valid || !arg.valid) return NaN();

	vlf temp(*this),arg2(arg);
	if(temp.sgn==-1&&arg2.sgn==1)
	{
		temp.sgn=1;
		return(arg2-temp);
	}
	if(temp.sgn==1&&arg2.sgn==-1)
	{
		arg2.sgn=1;
		return(temp-arg2);
	}

	if(temp.e>arg2.e)
		arg2.ShiftR(temp.e-arg2.e);
	else
		temp.ShiftR(arg2.e-temp.e);
	if(temp.b.size()>arg2.b.size()) 
		arg2.Expand(temp.b.size()-arg2.b.size());
	else 
		temp.Expand(arg2.b.size()-temp.b.size());

	int i;
	ulong carry=0;
	for(i=temp.b.size()-1;i>=0;i--)
	{
		temp.b[i]+=arg2.b[i]+carry;
		carry=0;
		while(temp.b[i]>=1000000000)
		{
			temp.b[i]-=1000000000;
			carry++;
		}
	}
	if(carry>0)
	{
		temp.ShiftR(1);
		temp.b[0]=carry;
	}
	return(temp);
}

vlf vlf::operator+=(const vlf& arg)
{
	(*this)=(*this)+arg;
	return(*this);
}

vlf vlf::operator-(const vlf& arg) const
{
	if(!valid || !arg.valid) return NaN();

	vlf temp(*this),arg2(arg);
	if(temp.sgn==-1&&arg2.sgn==1)
	{
		arg2.sgn=-1;
		return(arg2+temp);
	}
	if(temp.sgn==1&&arg2.sgn==-1)
	{
		arg2.sgn=1;
		return(temp+arg2);
	}
	char sign=1;
	if(temp.sgn==-1&&arg2.sgn==-1)
	{
		sign=-1;
		temp.sgn=arg2.sgn=1;
	}
	if(temp<=arg2)
	{
		vlf temp2;
		temp2=temp;
		temp=arg2;
		arg2=temp2;
		sign*=-1;
	}

	if(temp.e>arg2.e)
		arg2.ShiftR(temp.e-arg2.e);
	else
		temp.ShiftR(arg2.e-temp.e);
	if(temp.b.size()>arg2.b.size())
		arg2.Expand(temp.b.size()-arg2.b.size());
	else
		temp.Expand(arg2.b.size()-temp.b.size());

	int i;
	ulong borrow=0;
	for(i=temp.b.size()-1;i>=0;i--)
	{
		if(temp.b[i]<arg2.b[i]+borrow)
		{
			temp.b[i]+=1000000000;
			temp.b[i]-=arg2.b[i]+borrow;
			borrow=1;
		}
		else
		{
			temp.b[i]-=arg2.b[i]+borrow;
			borrow=0;
		}
	}
	if(borrow>0)
	{
		temp.ShiftR(1);
		temp.b[0]=borrow;
		sign*=-1;
	}
	temp.sgn=sign;

	return temp;
}

vlf vlf::operator-=(const vlf& arg)
{
	(*this)=(*this)-arg;
	return(*this);
}

vlf vlf::operator*(const vlf& arg) const
{
	if(!valid || !arg.valid) return NaN();

	vlf temp(*this),arg2(arg),res(0.0),*res1=new vlf[arg.b.size()];
	int i,j,cyf=0;
	___int64 c_res;
	ulong *resx=new ulong[temp.b.size()+1],carry=0;
	for(i=arg2.b.size()-1;i>=0;i--)
	{
		for(j=temp.b.size()-1;j>=0;j--)
		{
			c_res=(___int64)arg2.b[i]*(___int64)temp.b[j]+(___int64)carry;
			resx[cyf]=(ulong)(c_res%1000000000);
			carry=(ulong)(c_res/1000000000);
			cyf++;
		}
		if(carry>0)
		{
			resx[cyf]=carry;
			cyf++;
		}
		for(j=cyf-1;j>=0;j--)
			res1[i].AddDigit(resx[j]);
		res1[i].e=res1[i].b.size()-2+arg2.b.size()-i;
		cyf=0;
		carry=0;
		res+=res1[i];
	}
	res.e=(arg2.e+1-arg2.b.size())+(temp.e+1-temp.b.size())-1+res.b.size();
	res.sgn=sgn*arg.sgn;
	res.Normalize();
	res.Round();
	delete[] resx;
	delete[] res1;
	return res;
}

vlf vlf::operator*=(const vlf& arg)
{
	(*this)=(*this)*arg;
	return(*this);
}

vlf vlf::operator/(const vlf& arg) const
{
	if(!valid || !arg.valid) return NaN();

	if(vlf(0) == arg) return NaN();

	vlf q,r(*this),y(arg);
	ulong q0;
	long q_e;
	char q_sgn;
	___int64 r1;
	
	r.Normalize();
	y.Normalize();
	
	q_e=r.e-y.e;
	q_sgn=r.sgn*y.sgn;
	
	r.e=y.e=0;
	r.sgn=y.sgn=1;
	
	while(q.b.size()<divlimit+1 && r!=vlf(0))
	{
		if(r>=y)
		{
			r1=(___int64)r.b[0];
			if(r.e>0)
			{
				r1*=1000000000;
				if(r.b.size()>1) r1+=(___int64)r.b[1];
			}
			double q0d,off,krok;
			off=((y.b.size()>1)?((double)y.b[1]/1.e9):0.0); krok=1.0;
			do {
				q0d=(double)r1/((double)y.b[0]+off);
				q0=(ulong)q0d;
				if(y*vlf(q0)>r) off+=krok;
				if(y*vlf(q0)<r) off-=krok;
				krok/=2.0;
			} while(y*vlf(q0)>r||y*vlf(q0+1)<r);
			r-=y*vlf(q0);
			q.AddDigit(q0);
			r.e++;
			r.Normalize();
		}
		else
		{
			q.AddDigit(0);
			r.e++;
		}
	}
	q.e=q_e;
	q.sgn=q_sgn;
	q.Normalize();
	q.Round();
	return q;
}

vlf vlf::operator/=(const vlf& arg)
{
	(*this)=(*this)/arg;
	return(*this);
}

vlf vlf::operator^(const int& arg) const
{
	if(!valid) return NaN();

	int ex=(arg>0)?arg:-arg;
	int i = 1 << 30;

	while(!(ex & i)) i >>= 1;

	vlf res(1.0);

	while(i>0)
	{
		res *= res;
		if(ex & i) res *= (*this);
		i >>= 1;
	}
	if(arg<0) return (vlf(1)/res);
	else return res;
}

vlf vlf::operator^=(const int& arg)
{
	(*this)=(*this)^arg;
	return(*this);
}

//-----------------------------operatory logiczne-----------------------------------

bool vlf::operator>(const vlf& arg)
{
	if(!valid || !arg.valid) return false;

	return(((*this)>=arg)&&((*this)!=arg));
}

bool vlf::operator>=(const vlf& arg)
{
	if(!valid || !arg.valid) return false;

	vlf temp(*this),arg2(arg);
	if(sgn==-1&&arg.sgn==1)
		return false;
	if(sgn==1&&arg.sgn==-1)
		return true;
	if(sgn==-1&&arg.sgn==-1)
	{
		temp.sgn=1;
		arg2.sgn=1;
		return(temp<=arg2);
	}

	if(temp.e>arg2.e)
		arg2.ShiftR(temp.e-arg2.e);
	else
		temp.ShiftR(arg2.e-temp.e);

	if(temp.b.size()>arg2.b.size())
		arg2.Expand(temp.b.size()-arg2.b.size());
	else
		temp.Expand(arg2.b.size()-temp.b.size());

	for(unsigned i=0;i<temp.b.size();i++)
	{
		if(temp.b[i]<arg2.b[i])
			return false;
		if(temp.b[i]>arg2.b[i]) 
			return true;
	}
	return true;
}

bool vlf::operator<(const vlf& arg)
{
	if(!valid || !arg.valid) return false;

	return(((*this)<=arg)&&((*this)!=arg));
}

bool vlf::operator<=(const vlf& arg)
{
	if(!valid || !arg.valid) return false;

	vlf temp(*this),arg2(arg);
	if(sgn==-1&&arg.sgn==1)
		return true;
	if(sgn==1&&arg.sgn==-1)
		return false;
	if(sgn==-1&&arg.sgn==-1)
	{
		temp.sgn=1;
		arg2.sgn=1;
		return(temp>=arg2);
	}

	if(temp.e>arg2.e)
		arg2.ShiftR(temp.e-arg2.e);
	else
		temp.ShiftR(arg2.e-temp.e);

	if(temp.b.size()>arg2.b.size())
		arg2.Expand(temp.b.size()-arg2.b.size());
	else
		temp.Expand(arg2.b.size()-temp.b.size());

	for(unsigned i=0;i<temp.b.size();i++)
	{
		if(temp.b[i]>arg2.b[i])
			return false;
		if(temp.b[i]<arg2.b[i]) 
			return true;
	}
	return true;
}

bool vlf::operator==(const vlf& arg)
{
	if(!valid || !arg.valid) return false;

	vlf temp(*this),arg2(arg);
	temp.Normalize();
	arg2.Normalize();
	if(temp.b.size()!=arg2.b.size()) return false;
	if(temp.e!=arg2.e) return false;
	if(temp.sgn!=arg2.sgn) return false;
	unsigned i;
	for(i=0;i<temp.b.size();i++)
		if(temp.b[i]!=arg2.b[i]) return false;
	return true;
}

bool vlf::operator!=(const vlf& arg)
{
	return(!((*this)==arg));
}

//-------------------------------------------------------------------------

vlf floor(vlf arg)
{
	if(!arg.isValid()) return NaN();

	unsigned i;
	if(arg.e<0) {
		return vlf(0);
	}
	for(i=arg.e+1;i<arg.b.size();i++)
	{
		arg.b[i] = 0;
	}
	arg.Normalize();
	return arg;
}

vlf frac(vlf arg)
{
	if(!arg.isValid()) return NaN();

	int i;
	if(arg.e<0) {
		return arg;
	}
	for(i=0; i<arg.e+1; i++)
	{
		if(i < (int)arg.b.size()) arg.b[i] = 0;
	}
	arg.Normalize();
	return arg;
}

vlf exp(vlf arg)
{
	if(!arg.isValid()) return NaN();

	divlimit += 3;
	vlf result(1), add(1), dokl(0.000000001);
	dokl^=divlimit;
	vlf mdokl = vlf(0) - dokl;
	int i=1;
	while(add>dokl || add<mdokl)
	{
		add *= arg;
		add /= vlf(i);
		result += add;
		i++;
	}
	divlimit -= 3;
	result.Round();
	return result;
}

vlf ln(vlf arg)
{
	if(!arg.isValid()) return NaN();

	if(arg <= vlf(0)) return NaN();

	divlimit+=3;

	vlf e=exp(vlf(1)),i;

	i = vlf((int)arg.e*20);

	vlf div1 = exp(i);
	vlf arg2(arg);
	arg2 /= div1;

	while(arg2>vlf(1.65))
	{
		i += vlf(1);
		arg2 /= e;
	}
	while(arg2<vlf(0.607))
	{
		i -= vlf(1);
		arg2 *= e;
	}

	arg /= exp(i);
	arg -= vlf(1);

	vlf result(arg), add(arg), dokl(0.000000001);
	arg2 = arg;
	dokl^=divlimit;
	vlf mdokl = vlf(0) - dokl;
	int j=2;
	while(add > dokl || add < mdokl)
	{
		arg2 *= arg;
		add = arg2/vlf(j);
		if(j%2==1)
			result += add;
		else
			result -= add;
		j++;
	}

	result += i;
	divlimit -= 3;
	result.Round();

	return result;
}

/* Logarithm using Newton's method

vlf ln(vlf arg)
{
	divlimit+=3;

	if(arg <= vlf(0))
		return vlf(0);

	vlf res1,res2;
	vlf dokl(0.000000001),diff(1),mdokl;

	dokl ^= divlimit;
	mdokl = vlf(0)-dokl;

	res1 = vlf(arg.e*20);	//guesstimation

	while(diff > dokl || diff < mdokl)
	{
		vlf expres1 = exp(res1);
		res2 = res1 - (expres1-arg)/expres1;
		res2.Round();
		diff = res2 - res1;
		res1 = res2;
	}

	divlimit -= 3;
	res1.Round();

	return res1;
}*/

vlf sin(vlf arg)
{
	if(!arg.isValid()) return NaN();

	if(arg>vlf(20) || arg<vlf(-20)) {
		vlf arg2 = arg/vlf(2);
		return vlf(2)*sin(arg2)*cos(arg2);
	}
	divlimit += 2;
	vlf result(arg), add(arg), dokl(0.000000001), arg2;
	dokl^=divlimit;
	vlf mdokl = vlf(0) - dokl;
	arg2 = arg^2;
	int i=3;
	while(add > dokl || add < mdokl)
	{
		add *= arg2;
		add /= vlf(i-1)*vlf(i);
		if(i%4==3)
			result -= add;
		else
			result += add;
		i+=2;
	}
	divlimit -= 2;
	result.Round();
	return result;
}

vlf cos(vlf arg)
{	
	if(!arg.isValid()) return NaN();

	if(arg>vlf(20) || arg<vlf(-20)) {
		vlf arg2 = arg/vlf(2);
		return (vlf(2)*(cos(arg2)^2)-vlf(1));
	}
	divlimit += 2;
	vlf result(1), add(1), dokl(0.000000001), arg2;
	dokl^=divlimit;
	vlf mdokl = vlf(0) - dokl;
	int i=2;
	arg2 = arg^2;
	while(add > dokl || add < mdokl)
	{
		add *= arg2;
		add /= vlf(i-1)*vlf(i);
		if(i%4==2)
			result -= add;
		else
			result += add;
		i+=2;
	}
	divlimit -= 2;
	result.Round();
	return result;
}

vlf sinh(vlf arg)
{
	if(!arg.isValid()) return NaN();

	if(arg>vlf(20) || arg<vlf(-20)) {
		vlf arg2 = arg/vlf(2);
		return vlf(2)*sinh(arg2)*cosh(arg2);
	}
	divlimit += 2;
	vlf result(arg), add(arg), dokl(0.000000001), arg2;
	dokl^=divlimit;
	vlf mdokl = vlf(0) - dokl;
	arg2 = arg^2;
	int i=3;
	while(add > dokl || add < mdokl)
	{
		add *= arg2;
		add /= vlf(i-1)*vlf(i);
		result += add;
		i+=2;
	}
	divlimit -= 2;
	result.Round();
	return result;
}

vlf cosh(vlf arg)
{	
	if(!arg.isValid()) return NaN();

	if(arg>vlf(20) || arg<vlf(-20)) {
		vlf arg2 = arg/vlf(2);
		return (vlf(2)*(cosh(arg2)^2)-vlf(1));
	}
	divlimit += 2;
	vlf result(1), add(1), dokl(0.000000001), arg2;
	dokl^=divlimit;
	vlf mdokl = vlf(0) - dokl;
	int i=2;
	arg2 = arg^2;
	while(add > dokl || add < mdokl)
	{
		add *= arg2;
		add /= vlf(i-1)*vlf(i);
		result += add;
		i+=2;
	}
	divlimit -= 2;
	result.Round();
	return result;
}

/*vlf asin(vlf arg)
{
	divlimit += 2;

	vlf result(arg), arg2(arg), dokl(0.000000001), add(arg);
	dokl^=divlimit;
	vlf mdokl = vlf(0) - dokl;

	vlf fact2n(1),factn(1),fourn(1);

	int n=1;
	arg ^= 2;
	while(add > dokl || add < mdokl)
	{
		arg2 *= arg;
		fact2n *= vlf(2*n-1)*vlf(2*n);
		factn *= vlf(n);
		fourn *= vlf(4);
		add = arg2*fact2n/factn/factn/fourn/vlf(2*n+1);
		result += add;
		n++;
	}

	divlimit -= 2;
	result.Round();

	return result;
}*/

//test - Newton's method
vlf atan(vlf arg)
{
	if(!arg.isValid()) return NaN();

	if(arg > vlf(1)) return atan(vlf(1))*vlf(2) - atan(vlf(1)/arg);
	if(arg < vlf(-1)) return atan(vlf(-1))*vlf(2) - atan(vlf(1)/arg);

	divlimit+=2;

	vlf res1,res2;
	vlf dokl(0.000000001),diff(1),mdokl;

	dokl ^= divlimit;
	mdokl = vlf(0)-dokl;

	res1 = vlf(0);	//guesstimation

	while(diff > dokl || diff < mdokl)
	{
		vlf sinres1 = sin(res1);
		vlf cosres1 = cos(res1);
		vlf cos2res1 = cosres1^2;
		res2 = res1 - (sinres1/cosres1-arg)*cos2res1;
		res2.Round();
		diff = res2 - res1;
		res1 = res2;
	}

	divlimit -= 2;
	res1.Round();

	return res1;
}

vlf atan2(vlf y,vlf x)
{
	if(!y.isValid() || !x.isValid()) return NaN();

	divlimit += 2;
	vlf vpi4=atan(vlf(1));

	if(x==vlf(0)) {
		if(y<vlf(0)) return vpi4*vlf(-2);
		if(y>vlf(0)) return vpi4*vlf(2);
		return vlf(0);
	}

	/*double yx = (double)(y/x);
	vlf result;

	result = vlf(atan(yx));
	result.Round();*/

	vlf yx = y/x;
	vlf result = atan(yx);

	if(x<vlf(0)) {
		if(y >= vlf(0)) result += vpi4*vlf(4);
		if(y < vlf(0)) result -= vpi4*vlf(4);
	}

	divlimit -= 2;
	result.Round();

	return result;
}

vlf asinh(vlf arg)
{
	if(!arg.isValid()) return NaN();

	return ln(arg+sqrt((arg^2)+vlf(1)));
}

vlf acosh(vlf arg)
{
	if(!arg.isValid()) return NaN();

	return ln(arg+sqrt((arg^2)-vlf(1)));
}

vlf sqrt(vlf arg)
{
	if(!arg.isValid()) return NaN();

	if(arg==vlf(0)) return vlf(0);

	return exp(ln(arg)/vlf(2));
}

vlf abs(vlf arg)
{
	if(!arg.isValid()) return NaN();

	vlf res(arg);
	res.sgn = 1;
	return res;
}

//----------------------------------dodatki--------------------------------


vlf::operator double()
{
	double a=0.0;
	if(b.size()>1)
	{
		a+=b[1];
		a/=1000000000.0;
	}
	if(b.size()>0) a+=b[0];
	double expn = pow(1000000000.0,e);

	return a*expn*(double)sgn;
}

string cyfra(ulong c,string cyf)
{
	unsigned i;
	stringstream ss;

	ss << c;

	cyf = ss.str();

	if(cyf.length()<9)
	{
		unsigned a = cyf.length(),b = 9-a;
		for(i=0;i<b;i++)
			cyf = "0" + cyf;
	}
	return cyf;
}

string vlf::toString()
{
	string pom;
	string cyf;
	if(!valid) return "NaN";

	pom=(sgn==-1?"-":"");
	if(b.size()==1&&b[0]==0) return "0";
	bool trun=false;	//czy obcinaæ koñcowe zera? (konieczne tylko po przecinku)
	if(e<0)
	{
		trun=true;
		pom+="0.";
		for(long i=0;i<-1-e;i++)
			pom+="000000000";
		for(ulong j=0;j<b.size();j++)
		{
			cyf = cyfra(b[j],cyf);
			pom+=cyf;
		}
	}
	else
	{
		ulong i;
		stringstream ss;
		ss << b[0];
		cyf = ss.str();

		pom+=cyf;
		for(i=1;(int)i<e+1;i++)
		{
			if(i<b.size()) cyf = cyfra(b[i],cyf);
			else cyf = "000000000";
			pom+=cyf;
		}
		if(e+1<(int)b.size())
		{
			pom+=".";
			trun=true;
		}
		for(i=e+1;i<b.size();i++)
		{
			cyf = cyfra(b[i],cyf);
			pom+=cyf;
		}
	}
	
	if(trun)
	{
		unsigned i=pom.length()-1;
		while(pom[i]=='0')
		{
			pom = pom.substr(0,pom.length()-1);
			i--;
		}
	}

	return pom;
}

void vlf::Round()
{
	if(b.size()>divlimit)
	{
		if(b.size()>divlimit+1) b.erase(b.begin()+divlimit+1,b.end());
		if(b[b.size()-1] >= 500000000) {
			vlf a;
			a.b.clear();
			a.b.push_back(1);
			a.e = e - b.size() + 2;
			(*this) += a;
		}
		b.erase(b.begin()+divlimit,b.end());
	}
}

void SetDivLimit(ulong a)
{
	divlimit=a;
}

vlf NaN()
{
	vlf result;
	result.valid = false;
	result.Normalize();
	return result;
}
