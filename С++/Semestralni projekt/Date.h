#ifndef SEMESTRALNI_PRACE_DATE_H
#define SEMESTRALNI_PRACE_DATE_H

#include <cstdint>
#include <ostream>
#include <ctime>
#include <chrono>

using namespace std;

class Date
{
public:
    Date(int d, int m, int y, int h, int min);

    Date();

    Date & operator=(const Date &);

    bool SetDate(int d, int m, int y, int h, int min);

    int GetNumberDay() const;

    int GetYear()const;

    int GetMonth()const;

    const string & GetNameMonth()const;

    int GetDay()const;

    const string & GetNameDay()const;

    int GetHour()const;

    int GetMinute()const;

    void DetermineNameDay();

    friend ostream & operator << (ostream & os, const Date & other);

    string GetTime() const;

    bool operator ==(const Date & other)const;

    bool operator !=(const Date & other)const;

    bool operator <(const Date & other)const;

    bool operator <=(const Date & other)const;

    bool operator >(const Date & other)const;
private:
    tm date{};

};
#endif //SEMESTRALNI_PRACE_DATE_H
