#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

class CBigInt
{
public:
	// default constructor
	CBigInt();

	// copying/assignment/destruction
	CBigInt(const CBigInt& copy_dig);

	CBigInt& operator = (CBigInt other);

	CBigInt& operator=(int other);

	CBigInt& operator=(const string& other);

	~CBigInt() {}

	// int constructor
	CBigInt(int dig);

	// string constructor
	CBigInt(const string& dig_str);
	//********************************************************************************
	// operator +, any combination {CBigInt/int/string} + {CBigInt/int/string}
	friend CBigInt operator+ (int a, const CBigInt& b);

	friend CBigInt operator+ (const string a, const CBigInt& b);

	CBigInt operator+ (const CBigInt& other) const;

	CBigInt operator+ (const string& other) const;

	CBigInt operator+ (int other) const;
	//********************************************************************************
	// operator *, any combination {CBigInt/int/string} * {CBigInt/int/string}
	friend CBigInt operator* (int a, const CBigInt& b);

	friend CBigInt operator* (const string a, const CBigInt& b);

	CBigInt operator* (const CBigInt& other) const;

	CBigInt operator* (const string& other) const;

	CBigInt operator* (int other) const;
	//********************************************************************************
	// operator +=, any of {CBigInt/int/string}
	CBigInt& operator+= (const CBigInt& other);

	CBigInt& operator+= (const string& other);

	CBigInt& operator+= (int other);
	//********************************************************************************
	// operator *=, any of {CBigInt/int/string}
	CBigInt& operator*= (int other);

	CBigInt& operator*= (const CBigInt& other);

	CBigInt& operator*= (const string& other);
	//********************************************************************************
	// comparison operators, any combination {CBigInt/int/string} {<,<=,>,>=,==,!=} {CBigInt/int/string}
	bool operator == (const CBigInt& other)const;
	bool operator == (int other)const;
	bool operator == (const string& other)const;

	bool operator != (const CBigInt& other)const;
	bool operator != (int other)const;
	bool operator != (const string& other)const;

	bool operator > (const CBigInt& other)const;
	bool operator > (int other)const;
	bool operator > (const string& other)const;

	bool operator >= (const CBigInt& other)const;
	bool operator >= (int other)const;
	bool operator >= (const string& other)const;

	bool operator < (const CBigInt& other)const;
	bool operator < (int other)const;
	bool operator < (const string& other)const;

	bool operator <= (const CBigInt& other)const;
	bool operator <= (int other)const;
	bool operator <= (const string& other)const;

	//********************************************************************************
	// output operator <<
	friend ostream& operator << (ostream& os, const CBigInt& x);
	//********************************************************************************
	// input operator >>
	friend istream& operator >> (istream& is, CBigInt& x);
	//********************************************************************************
	friend void Add(const char& big, const char& small, vector<char>& result, bool symbol, size_t& j, size_t i);
private:
	vector<char> digit;
	bool flag_pos;
};
//--------------------------------------------------------------------------------------------------------------
CBigInt::CBigInt() : flag_pos(true)
{}
//--------------------------------------------------------------------------------------------------------------
CBigInt::CBigInt(const CBigInt& copy_dig) ///AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAчто если один длинее
{
	//size_digit = copy_dig.size_digit;
	digit.clear();
	flag_pos = copy_dig.flag_pos;
	for (size_t i=0; i< copy_dig.digit.size(); i++)
	{
		digit.push_back(copy_dig.digit[i]);
	}
}
//--------------------------------------------------------------------------------------------------------------
CBigInt::CBigInt(int dig)
{
	flag_pos = true;
	if (dig < 0)
	{
		flag_pos = false;
		dig *= (-1);
	}
	if (dig == 0)
	{
		digit.push_back(0);
	}
	else
	{
		while (dig != 0) {
			digit.push_back(dig % 10);
			dig /= 10;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
//musi byt vyjenka!!!!
CBigInt::CBigInt(const string& dig_str) {
	int i = 0;
	flag_pos = true;
	string a;
	if (dig_str[0] == '-') 
	{
		flag_pos = false;
		i = 1;
		for (i = dig_str.size() - 1; i > 0; i--)
		{
			a = dig_str[i];
			if ((a > "9" || a < "1") && a != "0") {
				throw invalid_argument("invalid_argument");
			}
			digit.push_back(stoi(a));
		}
	}
	else
	{
		for (i = dig_str.size() - 1; i >=0; i--)
		{
			a = dig_str[i];
			if ((a > "9" || a < "1") && a != "0") {
				throw invalid_argument("invalid_argument");
			}
			digit.push_back(stoi(a));
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
CBigInt& CBigInt::operator = (CBigInt other)
{
	digit.swap(other.digit);
	flag_pos = other.flag_pos;
	return *(this);
}
//--------------------------------------------------------------------------------------------------------------
CBigInt& CBigInt::operator = (int other)
{
	CBigInt a(other);
	digit.swap(a.digit);
	flag_pos = a.flag_pos;
	return *(this);
}
//--------------------------------------------------------------------------------------------------------------
CBigInt& CBigInt::operator = (const string& other)
{
	CBigInt a(other);
	digit.swap(a.digit);
	flag_pos = a.flag_pos;
	return *(this);
}
//--------------------------------------------------------------------------------------------------------------
CBigInt operator+(int a, const CBigInt& b) 
{
	return b + a;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt operator+(const string a, const CBigInt& b) 
{
	return b + a;
}
//--------------------------------------------------------------------------------------------------------------
void Add(const char& big, const char& small, vector<char>& result, bool symbol, size_t& j, size_t i)
{
	if (result.size() <= i) result.reserve(i * 2);
	if (symbol) {
		char a = big + small;
		if (a > 9)
		{
			//if (j == i)
			if (i == result.size())
			{
				result.push_back(a % 10);
				result.push_back(a / 10);
				//j ++;
			}
			else
			{
				result[i] += (a % 10);
				result.push_back(a / 10);
				//j ++;
			}
		}
		else
		{
			//if (j == i)
			if (i == result.size())
			{
				result.push_back(a);
				//j++;
			}
			else
			{
				result[i] += a;
				//j++;
			}
		}
	}
	else
	{
		char a = big - small;
		if (a < 0)
		{
			//if (j == i)
			if(i == result.size())
			{
				result.push_back(a + 10);
				result.push_back(-1);
				//j ++;
			}
			else
			{
				result[i] += (a + 10);
				result.push_back(-1);
				//j ++;
			}
		}
		else
		{
			//if (j == i)
			if(i == result.size())
			{
				result.push_back(a);
				//j++;
			}
			else
			{
				result[i] += a;
				//j++;
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
CBigInt CBigInt::operator+ (const CBigInt& other) const
{
	vector<char> result;
	bool flag_result = true;
	size_t size_this = digit.size();
	size_t size_other = other.digit.size();
	result.reserve(size_this > size_other ? size_this : size_other);
	size_t j = 0;
	if (!flag_pos && !other.flag_pos)
	{
		flag_result = false;
	}
	else if (!flag_pos && other.flag_pos)
	{
		if (size_this > size_other)
		{
			flag_result = false;
		}
		else if (size_this < size_other) 
		{
			flag_result = true; 
		}
		else 
		{
			CBigInt tmp = *this;
			tmp.flag_pos = true;
			if (tmp > other)
			{
				flag_result = false;
			}
			else if (tmp < other)
			{
				flag_result = true;
			}
			else
			{
				CBigInt b(0);
				return  b;
			}
		}
	}
	else if (flag_pos && !other.flag_pos)
	{
		if (size_this > size_other)
		{
			flag_result = true;
		}
		else if (size_this < size_other)
		{
			flag_result = false;
		}
		else
		{
			CBigInt tmp = other;
			tmp.flag_pos = true;
			if (tmp > *this)
			{
				flag_result = false;
			}
			else if (tmp < *this)
			{
				flag_result = true;
			}
			else
			{
				CBigInt b(0);
				return b;
			}
		}
	}
	
	if (size_this > size_other)
	{
		for (size_t i = 0; i < size_other; i++) 
		{
			if (!flag_pos && other.flag_pos)
			{
				Add(digit[i], other.digit[i], result, false, j, i);
			}
			else if (flag_pos && !other.flag_pos)
			{
				Add(digit[i], other.digit[i], result, false, j, i);
			}
			else
			{
				Add(digit[i], other.digit[i], result, true, j, i);
			}
		}

		for (size_t i = size_other; i < size_this; i++)
		{
			Add(digit[i], 0, result, true, j, i);

		}
	}
	else if (size_this < size_other)
	{
		for (size_t i = 0; i < size_this; i++)
		{
			if (!flag_pos && other.flag_pos)
			{
				Add(other.digit[i], digit[i], result, false, j, i);
			}
			else if (flag_pos && !other.flag_pos)
			{
				Add(other.digit[i], digit[i], result, false, j, i);
			}
			else
			{
				Add(other.digit[i], digit[i], result, true, j, i);
			}
		}

		for (size_t i = size_this; i < size_other; i++)
		{
			Add(other.digit[i], 0, result, false, j, i);
		}

	}
	else
	{
		for (size_t i = 0; i < size_this; i++) 
		{
			if (!flag_pos && other.flag_pos)
			{
				Add(other.digit[i], digit[i], result, false, j, i);
			}
			else if (flag_pos && !other.flag_pos)
			{
				Add(other.digit[i], digit[i], result, false, j, i);
			}
			else
			{
				Add(other.digit[i], digit[i], result, true, j, i);
			}
		}
	}

	CBigInt b(0);
	b.digit.swap(result);
	b.flag_pos = flag_result;
	return b;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt CBigInt::operator+ (const string& other) const
{
	CBigInt a(other);
	return *(this) + a;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt CBigInt::operator+(int other) const
{
	CBigInt a(other);
	return *(this) + a;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt & CBigInt::operator+= (const CBigInt& other)
{
	CBigInt a;
	a = *this + other;
	*this = a;
	return *this;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt & CBigInt::operator+= (const string& other)
{
	CBigInt a;
	a = *this + other;
	*this = a;
	return *this;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt & CBigInt::operator+= (int other)
{
	CBigInt a;
	a = *this + other;
	*this = a;
	return *this;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt operator* (int a, const CBigInt& b) 
{
	return b * a;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt operator* (const string a, const CBigInt& b) 
{
	return b * a;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt CBigInt::operator* (const CBigInt& other) const
{
	if (other.digit.back() == 0) return other;
	CBigInt a(0);
	for (size_t i = 0; i < digit.size(); i++)
	{
		for (size_t j = 0; j < other.digit.size(); j++)
		{
			CBigInt b;
			b = (int)pow(10, i) * digit[i] * (int)pow(10, j) * (other.digit[j]);
			a += b;
			//a = a + CBigInt((int)pow(10, i)*digit[i] * (int)pow(10, j)*(other.digit[j]));
		}
	}
	if (flag_pos != other.flag_pos) a.flag_pos = false;
	return a;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt CBigInt::operator* (const string& other) const
{
	CBigInt a(other);
	return *(this) * a;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt CBigInt::operator* (int other) const
{
	CBigInt a(other);
	return *(this) * a;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt& CBigInt::operator*= (const CBigInt& other)
{
	//CBigInt a(other);
	CBigInt a;
	a = *this * other;
	*this = a;
	return *this;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt& CBigInt::operator*= (const string& other)
{
	CBigInt a;
	a = *this * other;
	*this = a;
	return *this;
}
//--------------------------------------------------------------------------------------------------------------
CBigInt& CBigInt::operator*= (int other)
{
	CBigInt a;
	a = *this * other;
	*this = a;
	return *this;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator == (const CBigInt& other)const
{
	if (digit.size() != other.digit.size() || flag_pos != other.flag_pos) return false;
	for (size_t i = 0; i < digit.size(); i++)
	{
		if (digit[i] != other.digit[i])
			return false;
	}
	return true;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator == (int other)const
{
	CBigInt a(other);
	return *this == a;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator == (const string& other)const
{
	CBigInt a(other);
	return *this == a;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator != (const CBigInt& other)const
{
	return !(*this == other);
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator != (int other)const
{
	CBigInt a(other);
	return !(*this == other);
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator != (const string& other)const
{
	CBigInt a(other);
	return !(*this == other);
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator > (const CBigInt& other)const
{
	if (flag_pos && !other.flag_pos) return true;
	if (!flag_pos && other.flag_pos) return false;
	if (flag_pos && other.flag_pos)
	{
		if (digit.size() > other.digit.size()) return true;
		else if (digit.size() < other.digit.size()) return false;
		else 
		{
			for (int i = digit.size()- 1; i >= 0; i--)
			{
				if (digit[i] > other.digit[i])
					return true;
			}
		}
	}
	else if (!flag_pos && !other.flag_pos)
	{
		if (digit.size() > other.digit.size()) return false;
		else if (digit.size() < other.digit.size()) return true;
		else 
		{
			for (int i = digit.size() - 1; i >= 0; i--)
			{
				if (digit[i] < other.digit[i])
					return true;
			}
		}
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator > (int other)const
{
	CBigInt a(other);
	return *this > a;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator > (const string& other)const
{
	CBigInt a(other);
	return *this > a;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator >= (const CBigInt& other)const
{
	return *this == other || *this > other;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator >= (int other)const
{
	CBigInt a(other);
	return *this == other || *this > other;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator >= (const string& other)const
{
	CBigInt a(other);
	return *this == other || *this > other;
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator < (const CBigInt& other)const
{
	return !(*this >= other);
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator < (int other)const
{
	CBigInt a(other);
	return !(*this >= other);
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator < (const string& other)const
{
	CBigInt a(other);
	return !(*this >= other);
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator <= (const CBigInt& other)const
{
	return !(*this > other);
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator <= (int other)const
{
	CBigInt a(other);
	return !(*this > other);
}
//--------------------------------------------------------------------------------------------------------------
bool CBigInt::operator <= (const string& other)const
{
	CBigInt a(other);
	return !(*this > other);
}
//--------------------------------------------------------------------------------------------------------------
ostream& operator << (ostream& os, const CBigInt& x)
{
	if (!x.flag_pos) os << '-';
	bool zacatek = false;
	for (int i = x.digit.size() - 1; i>=0; i--)
	{
		while (!zacatek) 
		{
			if (x.digit[i] != 0) 
			{ 
				zacatek = true; 
			}
			else
			{
				i--;
			}
		}
		os << +x.digit[i];
	}
	return os;
}
//--------------------------------------------------------------------------------------------------------------
istream& operator >> (istream& is, CBigInt& x) //MEZERY СУКА
{
	char c;
	size_t i = 0;
	is >> c;
	if (x.digit.size() == 0)
	{
		x.digit.push_back(0);
	}
	if (c == '-')
	{
		x.flag_pos = false;
		//i = 1;
	}
	else if (!(c > '9' || c < '1'))
	{
		x.digit[0] = (c - 48);
		x.flag_pos = true;
		i = 1;

	}
	while (true) 
	{
		if (is.fail() || !(is >> c) || !(c <= '9' && c >= '1'))
		{
			is.setstate(ios::failbit);
			break;
			/*reverse(x.digit.begin(), x.digit.end());
			return is;*/
		}
		else 
		{
			if (i >= x.digit.size()) 
			{
				x.digit.push_back(c - 48);
			}
			else
			{
				x.digit[i] = (c - 48);
			}
		}	
		i++;
	}
	//reverse(x.digit.begin(), x.digit.end());
	return is;
}
//--------------------------------------------------------------------------------------------------------------

#ifndef __PROGTEST__
static bool equal(const CBigInt& x, const char* val)
{
	ostringstream oss;
	oss << x;
	return oss.str() == val;
}
int main(void)
{
	CBigInt a, b;
	istringstream is;
	a = "100";
	b = "-99";
	a += b;
	assert(equal(a, "1"));
	a = 10;
	a += 20;
	assert(equal(a, "30"));
	a *= 5;
	assert(equal(a, "150"));
	b = a + 3;
	assert(equal(b, "153"));
	b = a * 7;
	assert(equal(b, "1050"));
	assert(equal(a, "150"));

	a = 10;
	a += -20;
	assert(equal(a, "-10"));
	a *= 5;
	assert(equal(a, "-50"));
	b = a + 73;
	assert(equal(b, "23"));
	b = a * -7;
	assert(equal(b, "350"));
	assert(equal(a, "-50"));

	a = "12345678901234567890";
	a += "-99999999999999999999";
	assert(equal(a, "-87654321098765432109"));
	//a *= "54321987654321987654";
	//assert(equal(a, "-4761556948575111126880627366067073182286"));
	a *= 0;
	assert(equal(a, "0"));
	a = 10;
	b = a + "400";
	assert(equal(b, "410"));
	b = a * "15";
	assert(equal(b, "150"));
	assert(equal(a, "10"));

	is.clear();
	is.str(" 1234");
	/*assert(is >> b);
	/*assert(equal(b, "1234"));
	is.clear();
	is.str(" 12 34");
	assert(is >> b);
	assert(equal(b, "12"));
	is.clear();
	is.str("999z");
	assert(is >> b);
	assert(equal(b, "999"));
	is.clear();
	is.str("abcd");
	assert(!(is >> b));
	is.clear();
	is.str("- 758");
	assert(!(is >> b));
	try
	{
		a = "-xyz";
		assert("missing an exception" == NULL);
	}
	catch (const invalid_argument & e)
	{
	}*/
	//a = "73786976294838206464";
	//assert(a < "1361129467683753853853498429727072845824");
	/*assert(a <= "1361129467683753853853498429727072845824");
	assert(!(a > "1361129467683753853853498429727072845824"));
	assert(!(a >= "1361129467683753853853498429727072845824"));
	assert(!(a == "1361129467683753853853498429727072845824"));
	assert(a != "1361129467683753853853498429727072845824");
	assert(!(a < "73786976294838206464"));
	assert(a <= "73786976294838206464");
	assert(!(a > "73786976294838206464"));
	assert(a >= "73786976294838206464");
	assert(a == "73786976294838206464");
	assert(!(a != "73786976294838206464"));
	assert(a < "73786976294838206465");
	assert(a <= "73786976294838206465");
	assert(!(a > "73786976294838206465"));
	assert(!(a >= "73786976294838206465"));
	assert(!(a == "73786976294838206465"));
	assert(a != "73786976294838206465");*/
	//a = "2147483648";
	//assert(!(a < -2147483648));
	/*assert(!(a <= -2147483648));
	assert(a > -2147483648);
	assert(a >= -2147483648);
	assert(!(a == -2147483648));
	assert(a != -2147483648);
	*/
	return 0;
}
#endif /* __PROGTEST__ */
