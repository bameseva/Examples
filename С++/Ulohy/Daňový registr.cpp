#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

/*
Úkolem je realizovat třídu CTaxRegister, která bude implementovat

Pro plánované důslednější potírání daňových úniků je potřeba vybudovat databázi všech obyvatel, ve které budou archivované jejích příjmy a výdaje. 
Předpokládáme, že v databázi je zaveden každý občan v okamžiku jeho narození. Od té doby se mu počítají všechny příjmy a všechny výdaje. 
Záznam je z databáze odstraněn v okamžiku jeho úmrtí. Občan je identifikován svým jménem, adresou a číslem účtu. Číslo účtu je unikátní přes celou databázi. 
Jména a adresy se mohou opakovat, ale dvojice (jméno, adresa) je opět v databázi unikátní. 
Tedy v databázi může být mnoho jmen Jan Novak, mnoho lidí může mít adresu Hlavni 60, ale Jan Novak bydlící na adrese Hlavni 60 může být v databázi pouze jeden.
*/

class Taxpayer {
public:
	Taxpayer() 
	{
		in_pay = 0;
		out_pay = 0;
	}
	Taxpayer(const string& name ="", const string& addr ="", const string& account=""): name(name), address(addr), num_account(account)
	{
		in_pay = 0;
		out_pay = 0;
	}
	~Taxpayer()
	{

	}
	const string& Get_name() const { return name; }
	const string& Get_num_account() const { return num_account; }
	const string& Get_address() const { return address; }
	const int& Get_in_pay() const { return in_pay; }
	const int& Get_out_pay() const { return out_pay; }
	void Set_in_pay(int value) { in_pay+=value; }
	void Set_out_pay(int value) { out_pay += value; }

private:
	string name;
	string address;
	string num_account;
	int in_pay;
	int out_pay;
};
//--------------------------------------------------------------------------------------------
class CIterator
{
public:
	CIterator(const vector<Taxpayer*>* vec);
	bool                     AtEnd(void) const;
	void                     Next(void);
	string                   Name(void) const;
	string                   Addr(void) const;
	string                   Account(void) const;
private:
	const vector<Taxpayer*>* list;
	size_t position;
};

CIterator::CIterator(const vector<Taxpayer*>* vec) : list(vec)
{
	position = 0;
}

bool CIterator::AtEnd(void) const
{
	if (position == list->size())
		return true;
	return false;
}

void CIterator::Next(void)
{
	position++;
}

string CIterator::Name(void) const
{
	return list->at(position)->Get_name();
}

string CIterator::Addr(void) const
{
	return list->at(position)->Get_address();
}

string CIterator::Account(void) const
{
	return list->at(position)->Get_num_account();
}
//--------------------------------------------------------------------------------------------
class CTaxRegister
{
public:
	CTaxRegister();
	CTaxRegister(Taxpayer a);
	bool Birth(const string& name, const string& addr, const string& account);
	~CTaxRegister();
	bool Death(const string& name, const string& addr);
	bool Income(const string& account, int amount);
	bool Income(const string& name, const string& addr, int amount);
	bool Expense(const string& account, int amount);
	bool Expense(const string& name, const string& addr, int amount);
	bool Audit(const string& name, const string& addr, string& account, int& sumIncome, int& sumExpense) const;
	CIterator ListByName(void) const;
private:
	vector<Taxpayer*> vec_of_taxprayers;
	vector<Taxpayer*> vec_of_taxprayers_by_names;
	vector<Taxpayer*> vec_of_taxprayers_by_account;
};

CTaxRegister::CTaxRegister() {}

CTaxRegister::~CTaxRegister() {
	for (auto i : vec_of_taxprayers) delete i;
}

CTaxRegister::CTaxRegister(Taxpayer a)
{
	vec_of_taxprayers.reserve(1000);
	vec_of_taxprayers.push_back(new Taxpayer(a));
	vec_of_taxprayers_by_names.reserve(1000);
	vec_of_taxprayers_by_account.reserve(1000);

}

bool Cmp_name(Taxpayer* a, Taxpayer* b) {
	if (a->Get_name() < b->Get_name()) return true;
	if (b->Get_name() < a->Get_name()) return false;

	return a->Get_address() < b->Get_address();
}

bool Cmp_account(Taxpayer* a, Taxpayer* b) {
	return a->Get_num_account() < b->Get_num_account();
}

bool CTaxRegister::Birth(const string& name, const string& addr, const string& account)
{
	Taxpayer* a= new Taxpayer(name, addr, account);
	vec_of_taxprayers.push_back(a);

	vector<Taxpayer*>::iterator lower_by_name;
	vector<Taxpayer*>::iterator upper_by_name;
	vector<Taxpayer*>::iterator lower_by_account;
	vector<Taxpayer*>::iterator upper_by_account;

	lower_by_name = lower_bound(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), a, Cmp_name);
	upper_by_name = upper_bound(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), a, Cmp_name);

	lower_by_account = lower_bound(vec_of_taxprayers_by_account.begin(), vec_of_taxprayers_by_account.end(), a, Cmp_account);
	upper_by_account = upper_bound(vec_of_taxprayers_by_account.begin(), vec_of_taxprayers_by_account.end(), a, Cmp_account);

	if (lower_by_name == upper_by_name && lower_by_account == upper_by_account)
	{
		vec_of_taxprayers_by_names.insert(lower_by_name, a);
		vec_of_taxprayers_by_account.insert(lower_by_account, a);
		return true;
	} //jsou ruzne, znamena ze neexistuje object s temi data

	return false; 		//jsou stejny
}

bool CTaxRegister::Death(const string& name, const string& addr)
{
	Taxpayer* b = new Taxpayer(name, addr);
	if (binary_search(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), b, Cmp_name))
	{
		vector<Taxpayer*>::iterator lower_by_name = lower_bound(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), b, Cmp_name);
		vector<Taxpayer*>::iterator lower_by_account = lower_bound(vec_of_taxprayers_by_account.begin(), vec_of_taxprayers_by_account.end(), *lower_by_name, Cmp_account);

		for (size_t i = 0; i < vec_of_taxprayers.size(); i++)
		{
			if (vec_of_taxprayers[i]->Get_name() == name && vec_of_taxprayers[i]->Get_address() == addr) { 
				vec_of_taxprayers.erase(vec_of_taxprayers.begin() + i); 
			}
		}
		delete (*lower_by_name);
		vec_of_taxprayers_by_names.erase(lower_by_name);
		vec_of_taxprayers_by_account.erase(lower_by_account);
		delete b;
		return true;
	}
	delete b;
	return false;
}

bool Find_existed_item_by_name(vector<Taxpayer*>& vec_name, Taxpayer*& e, vector<Taxpayer*>::iterator& lower_by_name) {

	if (binary_search(vec_name.begin(), vec_name.end(), e, Cmp_name))
	{
		lower_by_name = lower_bound(vec_name.begin(), vec_name.end(), e, Cmp_name);
		return true;
	}
	return false;
}

bool Find_existed_item_by_account(vector<Taxpayer*>& vec_account, Taxpayer*& e, vector<Taxpayer*>::iterator& lower_by_account) {

	if (binary_search(vec_account.begin(), vec_account.end(), e, Cmp_account))
	{
		lower_by_account = lower_bound(vec_account.begin(), vec_account.end(), e, Cmp_account);
		return true;
	}
	return false;
}

bool CTaxRegister::Income(const string& account, int amount)
{
	Taxpayer* b = new Taxpayer("", "", account);
	vector<Taxpayer*>::iterator lower_by_account;
	if (Find_existed_item_by_account(vec_of_taxprayers_by_account, b, lower_by_account))
	{
		(*lower_by_account)->Set_in_pay(amount);
		delete b;
		return true;
	}
	delete b;
	return false;
}

//bool CTaxRegister::Income(const string& account, int amount)
//{
//	Taxpayer* b = new Taxpayer("", "", account);
//	if(binary_search(vec_of_taxprayers_by_account.begin(), vec_of_taxprayers_by_account.end(), b, Cmp_account))
//	{
//		vector<Taxpayer*>::iterator lower_by_account = lower_bound(vec_of_taxprayers_by_account.begin(), vec_of_taxprayers_by_account.end(), b, Cmp_account);
//		(*lower_by_account)->Set_in_pay(amount);
//		delete b;
//		return true;
//	}
//	delete b;
//	return false;
//}

bool CTaxRegister::Income(const string& name, const string& addr, int amount)
{
	Taxpayer* b = new Taxpayer(name, addr);
	vector<Taxpayer*>::iterator lower_by_name;
	if (Find_existed_item_by_name(vec_of_taxprayers_by_names, b, lower_by_name))
	{
		(*lower_by_name)->Set_in_pay(amount);
		delete b;
		return true;
	}
	delete b;
	return false;
}

//bool CTaxRegister::Income(const string& name, const string& addr, int amount)
//{
//	Taxpayer* b = new Taxpayer(name, addr);
//	if (binary_search(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), b, Cmp_name))
//	{
//		vector<Taxpayer*>::iterator lower_by_name = lower_bound(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), b, Cmp_name);
//		(*lower_by_name)->Set_in_pay(amount);
//		delete b;
//		return true;
//	}
//	delete b;
//	return false;
//}

bool CTaxRegister::Expense(const string& account, int amount)
{
	Taxpayer* b = new Taxpayer("", "", account);
	vector<Taxpayer*>::iterator lower_by_account;
	if (Find_existed_item_by_account(vec_of_taxprayers_by_account, b, lower_by_account))
	{
		(*lower_by_account)->Set_out_pay(amount);
		delete b;
		return true;
	}
	delete b;
	return false;
}

//bool CTaxRegister::Expense(const string& account, int amount)
//{
//	Taxpayer* b = new Taxpayer("", "", account);
//	if (binary_search(vec_of_taxprayers_by_account.begin(), vec_of_taxprayers_by_account.end(), b, Cmp_account))
//	{
//		vector<Taxpayer*>::iterator lower_by_account = lower_bound(vec_of_taxprayers_by_account.begin(), vec_of_taxprayers_by_account.end(), b, Cmp_account);
//		(*lower_by_account)->Set_out_pay(amount);
//		delete b;
//		return true;
//	}
//	delete b;
//	return false;
//}

bool CTaxRegister::Expense(const string& name, const string& addr, int amount)
{
	Taxpayer* b = new Taxpayer(name, addr);
	vector<Taxpayer*>::iterator lower_by_name;
	if (Find_existed_item_by_name(vec_of_taxprayers_by_names, b, lower_by_name))
	{
		(*lower_by_name)->Set_out_pay(amount);
		delete b;
		return true;
	}
	delete b;
	return false;
}

//bool CTaxRegister::Expense(const string& name, const string& addr, int amount)
//{
//	Taxpayer* b = new Taxpayer(name, addr);
//	if (binary_search(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), b, Cmp_name))
//	{
//		vector<Taxpayer*>::iterator lower_by_name = lower_bound(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), b, Cmp_name);
//		(*lower_by_name)->Set_out_pay(amount);
//		delete b;
//		return true;
//	}
//	delete b;
//	return false;
//}

bool CTaxRegister::Audit(const string& name, const string& addr, string& account, int& sumIncome, int& sumExpense) const
{
	Taxpayer* c = new Taxpayer(name, addr);
	if (binary_search(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), c, Cmp_name))
	{
		vector<Taxpayer*>::const_iterator lower_by_name = lower_bound(vec_of_taxprayers_by_names.begin(), vec_of_taxprayers_by_names.end(), c, Cmp_name);
		account=(*lower_by_name)->Get_num_account();
		sumIncome = (*lower_by_name)->Get_in_pay();
		sumExpense = (*lower_by_name)->Get_out_pay();
		delete c;
		return true;
	}
	delete c;
	return false;
}

CIterator CTaxRegister::ListByName(void) const
{
	return CIterator(&vec_of_taxprayers_by_names);
}

#ifndef __PROGTEST__
int main(void)
{
	string acct;
	int    sumIncome, sumExpense;
	CTaxRegister b1;
	assert(b1.Birth("John Smith", "Oak Road 23", "123/456/789"));
	assert(b1.Birth("Jane Hacker", "Main Street 17", "Xuj5#94"));
	assert(b1.Birth("Peter Hacker", "Main Street 17", "634oddT"));
	assert(b1.Birth("John Smith", "Main Street 17", "Z343rwZ"));
	assert(b1.Income("Xuj5#94", 1000));
	assert(b1.Income("634oddT", 2000));
	assert(b1.Income("123/456/789", 3000));
	assert(b1.Income("634oddT", 4000));
	assert(b1.Income("Peter Hacker", "Main Street 17", 2000));
	assert(b1.Expense("Jane Hacker", "Main Street 17", 2000));
	assert(b1.Expense("John Smith", "Main Street 17", 500));
	assert(b1.Expense("Jane Hacker", "Main Street 17", 1000));
	assert(b1.Expense("Xuj5#94", 1300));
	assert(b1.Audit("John Smith", "Oak Road 23", acct, sumIncome, sumExpense));
	assert(acct == "123/456/789");
	assert(sumIncome == 3000);
	assert(sumExpense == 0);
	assert(b1.Audit("Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense));
	assert(acct == "Xuj5#94");
	assert(sumIncome == 1000);
	assert(sumExpense == 4300);
	assert(b1.Audit("Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense));
	assert(acct == "634oddT");
	assert(sumIncome == 8000);
	assert(sumExpense == 0);
	assert(b1.Audit("John Smith", "Main Street 17", acct, sumIncome, sumExpense));
	assert(acct == "Z343rwZ");
	assert(sumIncome == 0);
	assert(sumExpense == 500);
	CIterator it = b1.ListByName();
	assert(!it.AtEnd()
		&& it.Name() == "Jane Hacker"
		&& it.Addr() == "Main Street 17"
		&& it.Account() == "Xuj5#94");
	it.Next();
	assert(!it.AtEnd()
		&& it.Name() == "John Smith"
		&& it.Addr() == "Main Street 17"
		&& it.Account() == "Z343rwZ");
	it.Next();
	assert(!it.AtEnd()
		&& it.Name() == "John Smith"
		&& it.Addr() == "Oak Road 23"
		&& it.Account() == "123/456/789");
	it.Next();
	assert(!it.AtEnd()
		&& it.Name() == "Peter Hacker"
		&& it.Addr() == "Main Street 17"
		&& it.Account() == "634oddT");
	it.Next();
	assert(it.AtEnd());
	assert(b1.Death("John Smith", "Main Street 17"));

	CTaxRegister b2;
	assert(b2.Birth("John Smith", "Oak Road 23", "123/456/789"));
	assert(b2.Birth("Jane Hacker", "Main Street 17", "Xuj5#94"));
	assert(!b2.Income("634oddT", 4000));
	assert(!b2.Expense("John Smith", "Main Street 18", 500));
	assert(!b2.Audit("John Nowak", "Second Street 23", acct, sumIncome, sumExpense));
	assert(!b2.Death("Peter Nowak", "5-th Avenue"));
	assert(!b2.Birth("Jane Hacker", "Main Street 17", "4et689A"));
	assert(!b2.Birth("Joe Hacker", "Elm Street 23", "Xuj5#94"));
	assert(b2.Death("Jane Hacker", "Main Street 17"));
	assert(b2.Birth("Joe Hacker", "Elm Street 23", "Xuj5#94"));
	assert(b2.Audit("Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense));
	assert(acct == "Xuj5#94");
	assert(sumIncome == 0);
	assert(sumExpense == 0);
	assert(!b2.Birth("Joe Hacker", "Elm Street 23", "AAj5#94"));
	return 0;
}
#endif /* __PROGTEST__ */
