#ifndef SEMESTRALNI_PRACE_TIME_H
#define SEMESTRALNI_PRACE_TIME_H

#include <cstdint>
#include <ostream>

using namespace std;
class Time
{
public:
    Time(int hour=0, int minute=0);

    int GetMinute()const;

    int GetHour()const;

    friend ostream & operator<<(ostream & oss, const Time & time);

    bool SetTime(int h, int m);

    bool operator ==(const Time & other)const;

    bool operator <(const Time & other)const;

    bool operator >(const Time & other)const;

    friend ostream & operator <<(ostream & os, const Time & time);

private:
    int hour, minute;

};
#endif //SEMESTRALNI_PRACE_TIME_H
