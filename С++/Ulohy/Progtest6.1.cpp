#ifndef __PROGTEST__

#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

class AbstractTableCell;

class CText;

class CEmpty;

class CImage;

class CTable;

void Empty_line(ostream& os, size_t max_width)//vypisuje prazdnou radku zadane sirky
{
	size_t a = 0;
	while (a != max_width) {
		os << " ";
		a++;
	}
}
//--------------------------------------------------------------------------------------------------

//Třída reprezentuje buňku
class AbstractTableCell {
public:
	AbstractTableCell() : max_length(0) {}

	AbstractTableCell(const AbstractTableCell&) = default;

	AbstractTableCell& operator=(const AbstractTableCell&) = default;

	virtual void Print(ostream& os, size_t index, size_t max_width, size_t max_high) const = 0; //funkce vypisu objektu podle radek
	virtual shared_ptr<AbstractTableCell> Clone() const = 0; //kopirijici funkce
	virtual ~AbstractTableCell() = default;

	virtual const size_t& Get_max_length() const = 0;

	virtual const size_t Get_count_items() const = 0;

protected:
	size_t max_length;
};

//--------------------------------------------------------------------------------------------------

//Třída reprezentuje text.
class CText : public AbstractTableCell {
public:

	enum align 
	{
		ALIGN_LEFT, ALIGN_RIGHT
	};

	CText(const string line, const align a);

	CText(const CText&) = default;

	CText& operator=(const CText& other) = default;

	void SetText(const string line);

	shared_ptr<AbstractTableCell> Clone() const;

	void Print(ostream& os, size_t index, size_t max_width, size_t max_high) const override;

	const size_t& Get_max_length() const override { return max_length; }

	const size_t Get_count_items() const override { return max_high; }

private:
	align al;
	vector<string> items;
	size_t max_high;
};

CText::CText(const string line, const align a) : AbstractTableCell(), al(a), max_high(0) {
	string loc;
	int i = 0;
	char currentChar = line[i];
	while (currentChar != '\0') 
	{
		if (currentChar != '\n') 
		{
			loc += currentChar;
		}
		else 
		{
			items.push_back(loc);
			if (max_length < loc.size()) max_length = loc.size();
			loc.clear();
		}
		i++;
		currentChar = line[i];
	}

	if (loc.size() != 0) 
	{
		items.push_back(loc);
		if (max_length < loc.size()) max_length = loc.size();
		loc.clear();
	}
	max_high = items.size();
	if (line[line.size() - 1] == '\n')
	{
		max_high += 1;
	}
}

void CText::SetText(const string line) {
	items.clear();
	max_length = 0;
	string loc;
	int i = 0;
	char currentChar = line[i];
	while (currentChar != '\0') {
		if (currentChar != '\n') {
			loc += currentChar;
		}
		else 
		{
			items.push_back(loc);
			if (max_length < loc.size()) max_length = loc.size();
			loc.clear();
		}
		i++;
		currentChar = line[i];
	}

	if (loc.size() != 0) {
		items.push_back(loc);
		if (max_length < loc.size()) max_length = loc.size();
		loc.clear();
	}
	max_high = items.size();
	if (line[line.size() - 1] == '\n')
	{
		max_high += 1;
	}
}

shared_ptr<AbstractTableCell> CText::Clone() const {
	return make_shared<CText>(*this);
}

void CText::Print(ostream& os, size_t index, size_t max_width, size_t max_high) const {
	if (items.size() > index) {
		if (items.at(index).size() == max_width) {
			os << items.at(index);
		}
		else {
			if (al == 0) {
				os << items.at(index);
				size_t a = max_width - items.at(index).size();
				Empty_line(os, a);
			}
			else {
				size_t a = max_width - items.at(index).size();
				Empty_line(os, a);
				os << items.at(index);
			}

		}
	}
	else {
		Empty_line(os, max_width);
	}
}
//--------------------------------------------------------------------------------------------------

//Třída reprezentuje prázdnou buňku.
class CEmpty : public AbstractTableCell {
public:
	CEmpty();

	CEmpty(const CEmpty&) = default;

	CEmpty& operator=(const CEmpty& other) = default;

	shared_ptr<AbstractTableCell> Clone() const override;

	void Print(ostream& os, size_t index, size_t max_width, size_t max_high) const override;

	const size_t& Get_max_length() const override { return max_length; }

	const size_t Get_count_items() const override { return 0; }
};

CEmpty::CEmpty() : AbstractTableCell() {}

shared_ptr<AbstractTableCell> CEmpty::Clone() const
{
	return make_shared<CEmpty>(*this);
}

void CEmpty::Print(ostream& os, size_t index, size_t max_width, size_t max_high) const
{
	Empty_line(os, max_width);
}
//--------------------------------------------------------------------------------------------------

//Třída reprezentuje obrázek.
class CImage : public AbstractTableCell {
public:
	CImage();

	CImage(const CImage&) = default;

	CImage& operator=(const CImage& other) = default;

	CImage& AddRow(const char* line);

	shared_ptr<AbstractTableCell> Clone() const override;

	void Print(ostream& os, size_t index, size_t max_width, size_t max_high) const override;

	const size_t& Get_max_length() const override { return max_length; }

	const size_t Get_count_items() const override { return items.size(); }

private:
	vector<string> items;
};

CImage::CImage() : AbstractTableCell() {}

CImage& CImage::AddRow(const char* line)
{
	auto len = strlen(line);
	if (max_length < len) max_length = len;
	items.push_back(line);
	return *this;
}

shared_ptr<AbstractTableCell> CImage::Clone() const
{
	return make_shared<CImage>(*this);
}

void CImage::Print(ostream& os, size_t index, size_t max_width, size_t max_high) const
{
	if (items.size() < max_high)
	{
		size_t dif = max_high - items.size();
		size_t dif_half = dif / 2;
		if (index - dif_half < 0)
		{
			Empty_line(os, max_width);
		}
		else {
			index = index - dif_half;
			if (items.size() > index)
			{
				if (max_length == max_width)
				{
					os << items.at(index);
				}
				else
				{
					size_t a = max_width - items.at(index).size();
					if (a % 2 == 1)
					{
						size_t b = a / 2;
						Empty_line(os, b);
						a = a / 2 + 1;
						os << items.at(index);
						Empty_line(os, a);
					}
					else
					{
						size_t b = a / 2;
						Empty_line(os, b);
						a = a / 2;
						os << items.at(index);
						Empty_line(os, a);
					}
				}
			}
			else {
				Empty_line(os, max_width);
			}
		}
	}
	else {
		if (items.size() > index) {
			if (max_length == max_width) {
				os << items.at(index);
			}
			else {
				size_t a = max_width - items.at(index).size();
				if (a % 2 == 1) {
					size_t b = a / 2;
					Empty_line(os, b);
					a = a / 2 + 1;
					os << items.at(index);
					Empty_line(os, a);
				}
				else {
					size_t b = a / 2;
					Empty_line(os, b);
					a = a / 2;
					os << items.at(index);
					Empty_line(os, a);
				}

			}
		}
		else {
			Empty_line(os, max_width);
		}

	}
}
//--------------------------------------------------------------------------------------------------

class CTable {
public:
	CTable(size_t rows,
		size_t cols);//	inicializuje instanci a nastaví požadovaný počet řádek a sloupců tabulky.Takto inicializovaná tabulka bude mít všechny buňky prázdné.
	CTable(const CTable& other);

	CTable& operator=(const CTable& other);

	void SetCell(size_t rows, size_t cols,
		const AbstractTableCell& newContent); //metoda změní obsah zadané buňky tabulky na zadaný nový obsah.
	AbstractTableCell& GetCell(size_t row,
		size_t col); //metoda zpřístupní buňku v tabulce pro další operace(nastavení vlastností obsaženého textu / obrázku / ... či čtení obsahu buňky.
	friend ostream&
		operator<<(ostream& os, const CTable& x); //umožní výpis zformátované tabulky do zadaného výstupního proudu.
private:
	vector<vector<shared_ptr<AbstractTableCell>>> table;
	size_t rows, cols;
};

CTable::CTable(size_t rows, size_t cols) : table(rows, vector<shared_ptr<AbstractTableCell>>(cols)), rows(rows), cols(cols) 
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
				table.at(i).at(j) = make_shared<CEmpty>(CEmpty());
		}
	}
}

CTable::CTable(const CTable& other) : table(other.rows, vector<shared_ptr<AbstractTableCell>>(other.cols)), rows(other.rows), cols(other.cols)
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			if (other.table.at(i).at(j) != nullptr)
				table.at(i).at(j) = other.table.at(i).at(j).operator*().Clone();
		}
	}
}

CTable& CTable::operator=(const CTable& other)
{
	if (this != &other)
	{
		rows = other.rows;
		cols = other.cols;
		table.clear();
		table.resize(rows);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				table.at(i).resize(cols);
			}
		}

		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				if (other.table.at(i).at(j) != nullptr)
					table.at(i).at(j) = other.table.at(i).at(j).operator*().Clone();
			}
		}
	}
	return *this;
}

void CTable::SetCell(size_t rows, size_t cols, const AbstractTableCell& newContent)
{
		table.at(rows).at(cols) = newContent.Clone();
}

AbstractTableCell& CTable::GetCell(size_t row, size_t col)
{
	return *(table.at(row).at(col));
}

ostream& operator<<(ostream& os, const CTable& x)
{
	vector<size_t> max_high_of_rows = {}; //bude se obsahovat max vysku kazke radky
	vector<size_t> max_width_of_cols = {}; //bude obsahovat max sirku kazdeho sloupce
	while (max_high_of_rows.size() != x.rows)
	{
		max_high_of_rows.push_back(0);
	}

	while (max_width_of_cols.size() != x.cols)
	{
		max_width_of_cols.push_back(0);
	}
	if (!x.table.empty()) {
		for (size_t i = 0; i < x.rows; i++) {

			for (size_t j = 0; j < x.cols; j++)
			{
				if (x.table.at(i).at(j) != nullptr)
				{
					auto w = x.table.at(i).at(j).operator*().Get_max_length();
					auto h = x.table.at(i).at(j).operator*().Get_count_items();
					if (max_width_of_cols[j] < w) max_width_of_cols[j] = w;
					if (max_high_of_rows[i] < h) max_high_of_rows[i] = h;
				}

			}

		}

	}
	//*************************************************************************************************
	string line; //radka mezi radkami
	line += "+";
	for (size_t i : max_width_of_cols) {
		size_t n = i;
		while (n != 0)
		{
			line += "-";
			n--;
		}
		line += "+";
	}
	line += "\n";
	os << line;

	//*************************************************************************************************

	size_t counter, j; //counter pocita pocet radek uz vypsanych
	for (size_t i = 0; i < x.rows; i++) {
		counter = 0;
		while (counter != max_high_of_rows[i]) //pokud counter nedosahne max pocta radek bude neco vypisovat
		{

			for (j = 0; j < x.cols; j++) {
				os << "|";
				if (x.table.at(i).at(j) != nullptr)
					x.table.at(i).at(j).operator*().Print(os, counter, max_width_of_cols[j], max_high_of_rows[i]);
				else
				{
					Empty_line(os, max_width_of_cols[j]);
				}
			}
			os << "|\n";
			counter++;
		}
		os << line;
	}
	return os;
}
//--------------------------------------------------------------------------------------------------

#ifndef __PROGTEST__

int main(void) {
	ostringstream oss;
	CTable t0(3, 2);
	t0.SetCell(0, 0, CText("Hello,\n""Hello Kitty", CText::ALIGN_LEFT));
	t0.SetCell(1, 0, CText("Lorem ipsum dolor sit amet", CText::ALIGN_LEFT));
	t0.SetCell(2, 0, CText("Bye,\n"
		"Hello Kitty", CText::ALIGN_RIGHT));
	t0.SetCell(1, 1, CImage()
		.AddRow("###                   ")
		.AddRow("#  #                  ")
		.AddRow("#  # # ##   ###    ###")
		.AddRow("###  ##    #   #  #  #")
		.AddRow("#    #     #   #  #  #")
		.AddRow("#    #     #   #  #  #")
		.AddRow("#    #      ###    ###")
		.AddRow("                     #")
		.AddRow("                   ## ")
		.AddRow("                      ")
		.AddRow(" #    ###   ###   #   ")
		.AddRow("###  #   # #     ###  ")
		.AddRow(" #   #####  ###   #   ")
		.AddRow(" #   #         #  #   ")
		.AddRow("  ##  ###   ###    ## "));
	t0.SetCell(2, 1, CEmpty());
	oss.str("");
	oss.clear();
	oss << t0;
	assert(oss.str() ==
		"+--------------------------+----------------------+\n"
		"|Hello,                    |                      |\n"
		"|Hello Kitty               |                      |\n"
		"+--------------------------+----------------------+\n"
		"|Lorem ipsum dolor sit amet|###                   |\n"
		"|                          |#  #                  |\n"
		"|                          |#  # # ##   ###    ###|\n"
		"|                          |###  ##    #   #  #  #|\n"
		"|                          |#    #     #   #  #  #|\n"
		"|                          |#    #     #   #  #  #|\n"
		"|                          |#    #      ###    ###|\n"
		"|                          |                     #|\n"
		"|                          |                   ## |\n"
		"|                          |                      |\n"
		"|                          | #    ###   ###   #   |\n"
		"|                          |###  #   # #     ###  |\n"
		"|                          | #   #####  ###   #   |\n"
		"|                          | #   #         #  #   |\n"
		"|                          |  ##  ###   ###    ## |\n"
		"+--------------------------+----------------------+\n"
		"|                      Bye,|                      |\n"
		"|               Hello Kitty|                      |\n"
		"+--------------------------+----------------------+\n");
	t0.SetCell(0, 1, t0.GetCell(1, 1));
	t0.SetCell(2, 1, CImage()
		.AddRow("*****   *      *  *      ******* ******  *")
		.AddRow("*    *  *      *  *      *            *  *")
		.AddRow("*    *  *      *  *      *           *   *")
		.AddRow("*    *  *      *  *      *****      *    *")
		.AddRow("****    *      *  *      *         *     *")
		.AddRow("*  *    *      *  *      *        *       ")
		.AddRow("*   *   *      *  *      *       *       *")
		.AddRow("*    *    *****   ****** ******* ******  *"));
	/*t0.SetCell(2, 1, t0.GetCell(2, 1));*/
	dynamic_cast<CText&> (t0.GetCell(1, 0)).SetText("Lorem ipsum dolor sit amet,\n"
		"consectetur adipiscing\n"
		"elit. Curabitur scelerisque\n"
		"lorem vitae lectus cursus,\n"
		"vitae porta ante placerat. Class aptent taciti\n"
		"sociosqu ad litora\n"
		"torquent per\n"
		"conubia nostra,\n"
		"per inceptos himenaeos.\n"
		"\n"
		"Donec tincidunt augue\n"
		"sit amet metus\n"
		"pretium volutpat.\n"
		"Donec faucibus,\n"
		"ante sit amet\n"
		"luctus posuere,\n"
		"mauris tellus");
	oss.str("");
	oss.clear();
	oss << t0;
	assert(oss.str() ==
		"+----------------------------------------------+------------------------------------------+\n"
		"|Hello,                                        |          ###                             |\n"
		"|Hello Kitty                                   |          #  #                            |\n"
		"|                                              |          #  # # ##   ###    ###          |\n"
		"|                                              |          ###  ##    #   #  #  #          |\n"
		"|                                              |          #    #     #   #  #  #          |\n"
		"|                                              |          #    #     #   #  #  #          |\n"
		"|                                              |          #    #      ###    ###          |\n"
		"|                                              |                               #          |\n"
		"|                                              |                             ##           |\n"
		"|                                              |                                          |\n"
		"|                                              |           #    ###   ###   #             |\n"
		"|                                              |          ###  #   # #     ###            |\n"
		"|                                              |           #   #####  ###   #             |\n"
		"|                                              |           #   #         #  #             |\n"
		"|                                              |            ##  ###   ###    ##           |\n"
		"+----------------------------------------------+------------------------------------------+\n"
		"|Lorem ipsum dolor sit amet,                   |                                          |\n"
		"|consectetur adipiscing                        |          ###                             |\n"
		"|elit. Curabitur scelerisque                   |          #  #                            |\n"
		"|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
		"|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
		"|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
		"|torquent per                                  |          #    #     #   #  #  #          |\n"
		"|conubia nostra,                               |          #    #      ###    ###          |\n"
		"|per inceptos himenaeos.                       |                               #          |\n"
		"|                                              |                             ##           |\n"
		"|Donec tincidunt augue                         |                                          |\n"
		"|sit amet metus                                |           #    ###   ###   #             |\n"
		"|pretium volutpat.                             |          ###  #   # #     ###            |\n"
		"|Donec faucibus,                               |           #   #####  ###   #             |\n"
		"|ante sit amet                                 |           #   #         #  #             |\n"
		"|luctus posuere,                               |            ##  ###   ###    ##           |\n"
		"|mauris tellus                                 |                                          |\n"
		"+----------------------------------------------+------------------------------------------+\n"
		"|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
		"|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
		"|                                              |*    *  *      *  *      *           *   *|\n"
		"|                                              |*    *  *      *  *      *****      *    *|\n"
		"|                                              |****    *      *  *      *         *     *|\n"
		"|                                              |*  *    *      *  *      *        *       |\n"
		"|                                              |*   *   *      *  *      *       *       *|\n"
		"|                                              |*    *    *****   ****** ******* ******  *|\n"
		"+----------------------------------------------+------------------------------------------+\n");
	CTable t1(t0);
	t1.SetCell(1, 0, CEmpty());
	t1.SetCell(1, 1, CEmpty());
	oss.str("");
	oss.clear();
	oss << t0;
	assert(oss.str() ==
		"+----------------------------------------------+------------------------------------------+\n"
		"|Hello,                                        |          ###                             |\n"
		"|Hello Kitty                                   |          #  #                            |\n"
		"|                                              |          #  # # ##   ###    ###          |\n"
		"|                                              |          ###  ##    #   #  #  #          |\n"
		"|                                              |          #    #     #   #  #  #          |\n"
		"|                                              |          #    #     #   #  #  #          |\n"
		"|                                              |          #    #      ###    ###          |\n"
		"|                                              |                               #          |\n"
		"|                                              |                             ##           |\n"
		"|                                              |                                          |\n"
		"|                                              |           #    ###   ###   #             |\n"
		"|                                              |          ###  #   # #     ###            |\n"
		"|                                              |           #   #####  ###   #             |\n"
		"|                                              |           #   #         #  #             |\n"
		"|                                              |            ##  ###   ###    ##           |\n"
		"+----------------------------------------------+------------------------------------------+\n"
		"|Lorem ipsum dolor sit amet,                   |                                          |\n"
		"|consectetur adipiscing                        |          ###                             |\n"
		"|elit. Curabitur scelerisque                   |          #  #                            |\n"
		"|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
		"|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
		"|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
		"|torquent per                                  |          #    #     #   #  #  #          |\n"
		"|conubia nostra,                               |          #    #      ###    ###          |\n"
		"|per inceptos himenaeos.                       |                               #          |\n"
		"|                                              |                             ##           |\n"
		"|Donec tincidunt augue                         |                                          |\n"
		"|sit amet metus                                |           #    ###   ###   #             |\n"
		"|pretium volutpat.                             |          ###  #   # #     ###            |\n"
		"|Donec faucibus,                               |           #   #####  ###   #             |\n"
		"|ante sit amet                                 |           #   #         #  #             |\n"
		"|luctus posuere,                               |            ##  ###   ###    ##           |\n"
		"|mauris tellus                                 |                                          |\n"
		"+----------------------------------------------+------------------------------------------+\n"
		"|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
		"|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
		"|                                              |*    *  *      *  *      *           *   *|\n"
		"|                                              |*    *  *      *  *      *****      *    *|\n"
		"|                                              |****    *      *  *      *         *     *|\n"
		"|                                              |*  *    *      *  *      *        *       |\n"
		"|                                              |*   *   *      *  *      *       *       *|\n"
		"|                                              |*    *    *****   ****** ******* ******  *|\n"
		"+----------------------------------------------+------------------------------------------+\n");
	oss.str("");
	oss.clear();
	oss << t1;
	assert(oss.str() ==
		"+-----------+------------------------------------------+\n"
		"|Hello,     |          ###                             |\n"
		"|Hello Kitty|          #  #                            |\n"
		"|           |          #  # # ##   ###    ###          |\n"
		"|           |          ###  ##    #   #  #  #          |\n"
		"|           |          #    #     #   #  #  #          |\n"
		"|           |          #    #     #   #  #  #          |\n"
		"|           |          #    #      ###    ###          |\n"
		"|           |                               #          |\n"
		"|           |                             ##           |\n"
		"|           |                                          |\n"
		"|           |           #    ###   ###   #             |\n"
		"|           |          ###  #   # #     ###            |\n"
		"|           |           #   #####  ###   #             |\n"
		"|           |           #   #         #  #             |\n"
		"|           |            ##  ###   ###    ##           |\n"
		"+-----------+------------------------------------------+\n"
		"+-----------+------------------------------------------+\n"
		"|       Bye,|*****   *      *  *      ******* ******  *|\n"
		"|Hello Kitty|*    *  *      *  *      *            *  *|\n"
		"|           |*    *  *      *  *      *           *   *|\n"
		"|           |*    *  *      *  *      *****      *    *|\n"
		"|           |****    *      *  *      *         *     *|\n"
		"|           |*  *    *      *  *      *        *       |\n"
		"|           |*   *   *      *  *      *       *       *|\n"
		"|           |*    *    *****   ****** ******* ******  *|\n"
		"+-----------+------------------------------------------+\n");
	t1 = t0;
	t1.SetCell(0, 0, CEmpty());
	t1.SetCell(1, 1, CImage()
		.AddRow("  ********                    ")
		.AddRow(" **********                   ")
		.AddRow("**        **                  ")
		.AddRow("**             **        **   ")
		.AddRow("**             **        **   ")
		.AddRow("***         ********  ********")
		.AddRow("****        ********  ********")
		.AddRow("****           **        **   ")
		.AddRow("****           **        **   ")
		.AddRow("****      **                  ")
		.AddRow(" **********                   ")
		.AddRow("  ********                    "));
	oss.str("");
	oss.clear();
	oss << t0;
	assert(oss.str() ==
		"+----------------------------------------------+------------------------------------------+\n"
		"|Hello,                                        |          ###                             |\n"
		"|Hello Kitty                                   |          #  #                            |\n"
		"|                                              |          #  # # ##   ###    ###          |\n"
		"|                                              |          ###  ##    #   #  #  #          |\n"
		"|                                              |          #    #     #   #  #  #          |\n"
		"|                                              |          #    #     #   #  #  #          |\n"
		"|                                              |          #    #      ###    ###          |\n"
		"|                                              |                               #          |\n"
		"|                                              |                             ##           |\n"
		"|                                              |                                          |\n"
		"|                                              |           #    ###   ###   #             |\n"
		"|                                              |          ###  #   # #     ###            |\n"
		"|                                              |           #   #####  ###   #             |\n"
		"|                                              |           #   #         #  #             |\n"
		"|                                              |            ##  ###   ###    ##           |\n"
		"+----------------------------------------------+------------------------------------------+\n"
		"|Lorem ipsum dolor sit amet,                   |                                          |\n"
		"|consectetur adipiscing                        |          ###                             |\n"
		"|elit. Curabitur scelerisque                   |          #  #                            |\n"
		"|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
		"|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
		"|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
		"|torquent per                                  |          #    #     #   #  #  #          |\n"
		"|conubia nostra,                               |          #    #      ###    ###          |\n"
		"|per inceptos himenaeos.                       |                               #          |\n"
		"|                                              |                             ##           |\n"
		"|Donec tincidunt augue                         |                                          |\n"
		"|sit amet metus                                |           #    ###   ###   #             |\n"
		"|pretium volutpat.                             |          ###  #   # #     ###            |\n"
		"|Donec faucibus,                               |           #   #####  ###   #             |\n"
		"|ante sit amet                                 |           #   #         #  #             |\n"
		"|luctus posuere,                               |            ##  ###   ###    ##           |\n"
		"|mauris tellus                                 |                                          |\n"
		"+----------------------------------------------+------------------------------------------+\n"
		"|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
		"|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
		"|                                              |*    *  *      *  *      *           *   *|\n"
		"|                                              |*    *  *      *  *      *****      *    *|\n"
		"|                                              |****    *      *  *      *         *     *|\n"
		"|                                              |*  *    *      *  *      *        *       |\n"
		"|                                              |*   *   *      *  *      *       *       *|\n"
		"|                                              |*    *    *****   ****** ******* ******  *|\n"
		"+----------------------------------------------+------------------------------------------+\n");
	oss.str("");
	oss.clear();
	oss << t1;
	assert(oss.str() ==
		"+----------------------------------------------+------------------------------------------+\n"
		"|                                              |          ###                             |\n"
		"|                                              |          #  #                            |\n"
		"|                                              |          #  # # ##   ###    ###          |\n"
		"|                                              |          ###  ##    #   #  #  #          |\n"
		"|                                              |          #    #     #   #  #  #          |\n"
		"|                                              |          #    #     #   #  #  #          |\n"
		"|                                              |          #    #      ###    ###          |\n"
		"|                                              |                               #          |\n"
		"|                                              |                             ##           |\n"
		"|                                              |                                          |\n"
		"|                                              |           #    ###   ###   #             |\n"
		"|                                              |          ###  #   # #     ###            |\n"
		"|                                              |           #   #####  ###   #             |\n"
		"|                                              |           #   #         #  #             |\n"
		"|                                              |            ##  ###   ###    ##           |\n"
		"+----------------------------------------------+------------------------------------------+\n"
		"|Lorem ipsum dolor sit amet,                   |                                          |\n"
		"|consectetur adipiscing                        |                                          |\n"
		"|elit. Curabitur scelerisque                   |        ********                          |\n"
		"|lorem vitae lectus cursus,                    |       **********                         |\n"
		"|vitae porta ante placerat. Class aptent taciti|      **        **                        |\n"
		"|sociosqu ad litora                            |      **             **        **         |\n"
		"|torquent per                                  |      **             **        **         |\n"
		"|conubia nostra,                               |      ***         ********  ********      |\n"
		"|per inceptos himenaeos.                       |      ****        ********  ********      |\n"
		"|                                              |      ****           **        **         |\n"
		"|Donec tincidunt augue                         |      ****           **        **         |\n"
		"|sit amet metus                                |      ****      **                        |\n"
		"|pretium volutpat.                             |       **********                         |\n"
		"|Donec faucibus,                               |        ********                          |\n"
		"|ante sit amet                                 |                                          |\n"
		"|luctus posuere,                               |                                          |\n"
		"|mauris tellus                                 |                                          |\n"
		"+----------------------------------------------+------------------------------------------+\n"
		"|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
		"|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
		"|                                              |*    *  *      *  *      *           *   *|\n"
		"|                                              |*    *  *      *  *      *****      *    *|\n"
		"|                                              |****    *      *  *      *         *     *|\n"
		"|                                              |*  *    *      *  *      *        *       |\n"
		"|                                              |*   *   *      *  *      *       *       *|\n"
		"|                                              |*    *    *****   ****** ******* ******  *|\n"
		"+----------------------------------------------+------------------------------------------+\n");

	return 0;
}

#endif /* __PROGTEST__ */
