#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#endif /* __PROGTEST */
using namespace std;

#ifndef __PROGTEST__

/*
Úkolem je vytvořit třídy CMailBox a CMail, které budou simulovat poštovní schránku na poštovním serveru.
 */

//Je jednoduchá třída reprezentující datum a čas.
class CTimeStamp
{
public:
	CTimeStamp( int               year,
				int               month,
				int               day,
				int               hour,
				int               minute,
				int               sec);
	int Compare(const CTimeStamp& x) const;
	friend ostream& operator << (ostream& os, const CTimeStamp& x);
private:
	int year, month, day, hour, minute, sec;
};
CTimeStamp::CTimeStamp(int               year,
	int               month,
	int               day,
	int               hour,
	int               minute,
	int               sec):year(year), month(month), day(day), hour(hour), minute(minute), sec(sec){}
int CTimeStamp::Compare(const CTimeStamp& x) const
{
	if (year > x.year) return 1;
	else if (year < x.year) return -1;
	else 
	{
		if (month > x.month) return 1;
		else if (year < x.year) return -1;
		else
		{
			if (month > x.month) return 1;
			else if (month < x.month) return -1;
			else
			{
				if (day > x.day) return 1;
				else if (day < x.day) return -1;
				else
				{
					if (hour > x.hour) return 1;
					else if (hour < x.hour) return -1;
					else
					{
						if (minute > x.minute) return 1;
						else if (minute < x.minute) return -1;
						else
						{
							if (sec > x.sec) return 1;
							else if (sec < x.sec) return -1;
							else
							{
								return 0;
							}
						}
					}
				}
			}
		}
	}

}
//YYYY-MM-DD HH24:MI:SS.
ostream& operator << (ostream& os, const CTimeStamp& x)
{
	os << x.year << "-";
	if (x.month < 10) os <<"0"<< x.month << "-";
	else  os << x.month << "-";
	if (x.day < 10) os << "0" << x.day << " ";
	else  os<< x.day << " ";
	if (x.hour < 10) os << "0" << x.hour << ":";
	else os<< x.hour << ":";
	if (x.minute < 10) os << "0" << x.minute << ":";
	else os<< x.minute << ":"; 
	if (x.sec < 10) os << "0" << x.sec;
	else os<< x.sec; 
	
	
	
	return os;
}
//=================================================================================================
//Tato třída zabaluje tělo e - mailu.
class CMailBody
{
public:
	CMailBody(int size, const char* data);
	// copy cons/op=/destructor is correctly implemented in the testing environment
	CMailBody(const CMailBody& other);
	CMailBody& operator=(const CMailBody& other);
	~CMailBody();
	friend ostream& operator << (ostream& os, const CMailBody& x)
	{
		return os << "mail body: " << x.m_Size << " B";
	}
	
private:
	int m_Size;
	char* m_Data;
};

CMailBody::CMailBody(int size, const char* data)
{
	m_Size = size;
	if (m_Size != 0)
	{
		m_Data = new char[size + 1];
		strcpy(m_Data, data);
	}
	else
	{
		m_Data = nullptr;;
	}
}

CMailBody::CMailBody(const CMailBody& other)
{
	m_Size = other.m_Size;
	if (m_Size != 0)
	{
		m_Data = new char[other.m_Size + 1];
		strcpy(m_Data, other.m_Data);
	}
	else
	{
		m_Data = nullptr;;
	}
}

CMailBody& CMailBody::operator=(const CMailBody& other)
{
	if (m_Size !=0) delete[] m_Data;
	m_Size = other.m_Size;
	m_Data = new char[other.m_Size +1];
	strcpy(m_Data, other.m_Data);
	return *this;
}

CMailBody::~CMailBody()
{
	if (m_Size != 0) delete[] m_Data;
	m_Size = 0;
}
//=================================================================================================
//Tato třída reprezentuje přílohy.
class CAttach
{
public:
	CAttach(int x): m_X(x), m_RefCnt(1)
	{
	}
	void AddRef(void) const
	{
		m_RefCnt++;
	}
	void Release(void) const
	{
		if (!--m_RefCnt)
			delete this;
	}
private:
	int            m_X;
	mutable int    m_RefCnt;
	CAttach(const CAttach& x);
	CAttach& operator = (const CAttach& x);
	~CAttach(void) = default;
	friend ostream& operator << (ostream& os, const CAttach& x)
	{
		return os << "attachment: " << x.m_X << " B";
	}
};
CAttach::CAttach(const CAttach& x)
{
	m_X = x.m_X;
	m_RefCnt = x.m_RefCnt;
	AddRef();
	x.AddRef();
}
CAttach& CAttach::operator = (const CAttach& x)
{
	m_X = x.m_X;
	m_RefCnt = x.m_RefCnt;
	return *this;
}

//=================================================================================================
#endif /* __PROGTEST__, DO NOT remove */

//Tato třída reprezentuje jeden e-mail. E-mail je popsán datem a časem doručení, odesilatelem, obsahem a přílohami.
class CMail
{
public:
	CMail():from(""), em_body(0, nullptr), ar_time(0, 0, 0, 0, 0, 0), adds(nullptr){}
	CMail(const CMail& other);
	//CMail(CMail&& other);
	CMail(const CTimeStamp& timeStamp, const string& from, const CMailBody& body, const CAttach* attach);
	const string& From(void) const; //odesilatelem
	const CMailBody& Body(void) const; //obsahem
	const CTimeStamp& TimeStamp(void) const;//datem a časem doručení
	const CAttach* Attachment(void) const; //přílohami
	bool operator<(const CMail& other) const;
	friend ostream& operator << (ostream& os, const CMail& x);
	~CMail();
	bool Cmp_by_time(const CTimeStamp& x) const;
private:
	string from;
	CMailBody em_body;
	CTimeStamp ar_time;
	const CAttach* adds;
	//void Swap(CMail& other);
};
//
//void CMail::Swap(CMail& other)
//{
//	from.swap(other.from);
//	em_body.
//}

bool CMail::Cmp_by_time(const CTimeStamp& x) const
{
	if (ar_time.Compare(x) > 0) return true;
	else return false;
}


CMail::CMail(const CTimeStamp& timeStamp, const string& from="", const CMailBody& body= CMailBody(0, nullptr), const CAttach* attach = nullptr):from(from), em_body(body), ar_time(timeStamp), adds(attach)
{
	if (attach != nullptr) adds->AddRef();
}

CMail::CMail(const CMail& other) : from(other.from), em_body(other.em_body), ar_time(other.ar_time), adds(other.adds)
{
	if (adds != nullptr) adds->AddRef();
}

//CMail::CMail(CMail&& other): Swap(CMail&& other)
//{}
const string& CMail::From(void) const
{
	return from;
}

const CMailBody& CMail::Body(void) const
{
	return em_body;
}

const CTimeStamp& CMail::TimeStamp(void) const
{
	return ar_time;
}

const CAttach* CMail::Attachment(void) const
{
	return adds;
}

CMail::~CMail()
{
	if (adds != nullptr)adds->Release();
}

ostream& operator << (ostream& os, const CMail& x)
{
	os << x.ar_time <<" "<< x.from <<" "<< x.em_body;
	if (x.adds != nullptr)
	{
		os << " + ";
		os << *(x.adds);
	}
	return os;
}

bool CMail::operator<(const CMail& other) const
{
	
	if (ar_time.Compare(other.ar_time) > 0) return true;
	else  return false;
	//return from < other.from;
}
//=================================================================================================
//Tato třída reprezentuje jednu e-mailovou schránku. schránka může obsahovat došlou poštu (instance třídy CMail), pošta může být rozdělena do složek.

struct By_time
{
	bool operator() (const shared_ptr<CMail> a, const shared_ptr<CMail> b) const
	{
		return a.operator*() < b.operator*();
	}
};
bool cmp_by_time(const shared_ptr<CMail> a, const CTimeStamp& b)
{
	return a.operator*().Cmp_by_time(b) ;
}
class CMailBox
{
public:
	CMailBox(void);
	bool           Delivery(const CMail& mail);
	bool           NewFolder(const string& folderName);
	bool           MoveMail(const string& fromFolder, const string& toFolder);
	list<CMail>    ListMail(const string& folderName, const CTimeStamp& from, const CTimeStamp& to) const;
	set<string>    ListAddr(const CTimeStamp& from, const CTimeStamp& to) const;
private:
	multiset<shared_ptr<CMail>, By_time> mails_by_time;
	map<string, multiset<shared_ptr<CMail>, By_time>> folders;
};

CMailBox::CMailBox(void)
{
	folders.emplace("inbox", multiset<shared_ptr<CMail>, By_time>());
}

bool CMailBox::Delivery(const CMail& mail)
{
	//shared_ptr<Foo> sh2(new Foo)
	shared_ptr<CMail> a(new CMail(mail));
	mails_by_time.insert(a); 
	folders["inbox"].insert(a);
	/*auto it = folders.find("inbox");
	if (it != folders.end()) 
	{
		it->second.insert(a);
	}*/
	return true;

	/*CMail a(mail);
	auto it = folders.find("inbox");
	if (it != folders.end())
	{
		it->second.insert(a);
		cout << "ulozilo"<<endl;
	}
	mails_by_time.insert(move(a));

	return true;*/
}

bool CMailBox::NewFolder(const string& folderName)
{
	return folders.emplace(folderName, multiset<shared_ptr<CMail>, By_time>()).second;
}
bool CMailBox::MoveMail(const string& fromFolder, const string& toFolder)
{
	auto it_from = folders.find(fromFolder);
	auto it_to = folders.find(toFolder);
	if (it_to != folders.end() && it_from != folders.end())
	{
		//auto begin_from = it_from->second.begin();
		//while (begin_from != it_from->second.end())
		while(!it_from->second.empty())
		{
			it_to->second.insert(*(it_from->second.begin()));
			it_from->second.erase(it_from->second.begin());
			//begin_from++;
		}
		
		//for (CMail x : it_from->second) it_to->second.insert(move(x));

		//move(s.extract(s.begin()).value());
		//auto begin_from = it_from->second.begin();
		//while (begin_from != it_from->second.end())
		//{
		//	it_to->second.insert(move(*(it_from->second.begin())));
		//	begin_from++;
		//}

		//for (size_t i = 0; i < it_from->second.size; i++)
		//{
		//	it_to->second.insert(move(it_from->second[i]));
		//}

	}
	return it_from->second.empty();
}
//Metoda vrátí seznam e - mailů ze zadané složky fld v zadaném rozmezí času(from - to, obě meze včetně).Pokud zadaná složka neexistuje, vrátí prázdný seznam.
list<CMail> CMailBox::ListMail(const string& folderName, const CTimeStamp& from, const CTimeStamp& to) const
{
	auto it= folders.find(folderName);
	list<CMail> a;
	//shared_ptr<CMail> f(new CMail(from));
	//shared_ptr<CMail> t(new CMail(to));

	if (it != folders.end())
	{
			auto start_time = lower_bound(it->second.begin(), it->second.end(), to, cmp_by_time);
			while (start_time != it->second.end())
			{
				if (from.Compare(start_time->operator*().TimeStamp()) > 0) return a;
				a.push_front(start_time->operator*());
				start_time++;
			}
	}
	return a;
}

set<string> CMailBox::ListAddr(const CTimeStamp& from, const CTimeStamp& to) const
{
	set<string>  a;
	//shared_ptr<CMail> t(new CMail(to));

		auto start_time = lower_bound(mails_by_time.begin(), mails_by_time.end(), to, cmp_by_time);
		while (start_time != mails_by_time.end())
		{
			if (from.Compare(start_time->operator*().TimeStamp()) > 0) return a;
			a.insert(start_time->operator*().From());
			start_time++;
		}
	return a;
}
//=================================================================================================
#ifndef __PROGTEST__
static string showMail(const list<CMail>& l)
{
	ostringstream oss;
	for (const auto& x : l)
		oss << x << endl;
	return oss.str();
}
static string showUsers(const set<string>& s)
{
	ostringstream oss;
	for (const auto& x : s)
		oss << x << endl;
	return oss.str();
}
int main(void)
{
	list<CMail> mailList;
	set<string> users;
	CAttach* att;

	CMailBox m0;
	assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 15, 24, 13), "user1@fit.cvut.cz", CMailBody(14, "mail content 1"), nullptr)));
	assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 15, 26, 23), "user2@fit.cvut.cz", CMailBody(22, "some different content"), nullptr)));
	att = new CAttach(200);
	assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 11, 23, 43), "boss1@fit.cvut.cz", CMailBody(14, "urgent message"), att)));
	assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 18, 52, 27), "user1@fit.cvut.cz", CMailBody(14, "mail content 2"), att)));
	att->Release();
	att = new CAttach(97);
	assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 16, 12, 48), "boss1@fit.cvut.cz", CMailBody(24, "even more urgent message"), att)));
	att->Release();
	assert(showMail(m0.ListMail("inbox",
		CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n"
		"2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
		"2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
		"2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n"
		"2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n");
	assert(showMail(m0.ListMail("inbox",
		CTimeStamp(2014, 3, 31, 15, 26, 23),
		CTimeStamp(2014, 3, 31, 16, 12, 48))) == "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
		"2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n");
	assert(showUsers(m0.ListAddr(CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "boss1@fit.cvut.cz\n"
		"user1@fit.cvut.cz\n"
		"user2@fit.cvut.cz\n");
	assert(showUsers(m0.ListAddr(CTimeStamp(2014, 3, 31, 15, 26, 23),
		CTimeStamp(2014, 3, 31, 16, 12, 48))) == "boss1@fit.cvut.cz\n"
		"user2@fit.cvut.cz\n");

	CMailBox m1;
	assert(m1.NewFolder("work"));
	assert(m1.NewFolder("spam"));
	assert(!m1.NewFolder("spam"));
	assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 15, 24, 13), "user1@fit.cvut.cz", CMailBody(14, "mail content 1"), nullptr)));
	att = new CAttach(500);
	assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 15, 26, 23), "user2@fit.cvut.cz", CMailBody(22, "some different content"), att)));
	att->Release();
	assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 11, 23, 43), "boss1@fit.cvut.cz", CMailBody(14, "urgent message"), nullptr)));
	att = new CAttach(468);
	assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 18, 52, 27), "user1@fit.cvut.cz", CMailBody(14, "mail content 2"), att)));
	att->Release();
	assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 16, 12, 48), "boss1@fit.cvut.cz", CMailBody(24, "even more urgent message"), nullptr)));
	assert(showMail(m1.ListMail("inbox",
		CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
		"2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
		"2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
		"2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
		"2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n");
	assert(showMail(m1.ListMail("work",
		CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "");
	assert(m1.MoveMail("inbox", "work"));
	assert(showMail(m1.ListMail("inbox",
		CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "");
	assert(showMail(m1.ListMail("work",
		CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
		"2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
		"2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
		"2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
		"2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n");
	assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 19, 24, 13), "user2@fit.cvut.cz", CMailBody(14, "mail content 4"), nullptr)));
	att = new CAttach(234);
	assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 13, 26, 23), "user3@fit.cvut.cz", CMailBody(9, "complains"), att)));
	att->Release();
	assert(showMail(m1.ListMail("inbox",
		CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
		"2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n");
	assert(showMail(m1.ListMail("work",
		CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
		"2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
		"2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
		"2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
		"2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n");
	assert(m1.MoveMail("inbox", "work"));
	assert(showMail(m1.ListMail("inbox",
		CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "");
	assert(showMail(m1.ListMail("work",
		CTimeStamp(2000, 1, 1, 0, 0, 0),
		CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
		"2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
		"2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
		"2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
		"2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
		"2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n"
		"2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n");

	return 0;
}
#endif /* __PROGTEST__ */
