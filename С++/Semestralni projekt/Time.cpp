#include "Time.h"

Time::Time(int hour, int minute)
{
    SetTime(hour, minute);
}

int Time::GetMinute()const { return minute;}

int Time::GetHour()const { return hour;}

bool Time::SetTime(int h, int m)
{
    if((h>23 || h<0) || (m>59 || m<0)) return false;
    else
    {
        hour=h;
        minute=m;
    }
    return true;
}
bool Time::operator ==(const Time & other)const
{
    return hour==other.hour && minute==other.minute;
}
bool Time::operator <(const Time & other)const
{
    return hour<other.hour || (hour==other.hour && minute<other.minute);
}
bool Time::operator >(const Time & other)const
{
    return hour>other.hour || (hour==other.hour && minute>other.minute);
}

ostream& operator<<(ostream& os, const Time& time)
{
    if(time.GetHour()<10) os<<"0"<<time.GetHour()<<":";
    else os<<time.GetHour()<<":";
    if(time.GetMinute()<10) os<<"0"<<time.GetMinute();
    else os<<time.GetMinute();
    return os;
}