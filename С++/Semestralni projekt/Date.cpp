#include <iostream>
#include <istream>
#include <cmath>
#include <sstream>
#include "Date.h"

static const int day_number[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const string month_name[]={"Jan", "Feb", "Mar", "Apl", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nom", "Dec"};
static const string day_name[]={"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

bool const IsYearLeap(int y)
{
    return y % 4 == 0 && (y % 400 == 0 || y % 100 != 0);
}

Date::Date(int d, int m, int y, int h, int min)
{
    SetDate(d, m, y, h, min);
}

Date::Date(){}

bool Date::SetDate(int d, int m, int y, int h, int min)
{
    if(m>12 || m<1)
    {
        cout<<"Number of month is wrong"<<endl;
        return false;
    }

    if(IsYearLeap(y) && m==2 && d>29)
    {
        cout<<"Number of day is wrong"<<endl;
        return false;
    }
    else
    {
        if(d>day_number[m-1])
        {
            cout<<"Number of day is wrong"<<endl;
            return false;
        }
        else
         {
             date.tm_mday=d;
             date.tm_mon=m;
             date.tm_year=y;
        }
    }

    if(h>23 || h<0)
    {
        cout<<"Number of our is wrong"<<endl;
        return false;
    }
    else
    {
        if(min>59 || min<0)
        {
            cout<<"Number of minute is wrong"<<endl;
            return false;
        } else
        {
            date.tm_hour=h;
            date.tm_min=min;
        }

    }
    date.tm_sec=0;
    DetermineNameDay();
    return true;
}

Date & Date::operator=(const Date & x)
{
    date.tm_year=x.GetYear();
    date.tm_mon=x.GetMonth();
    date.tm_mday=x.GetDay();
    date.tm_hour=x.GetHour();
    date.tm_min=x.GetMinute();
    date.tm_sec=0;
    date.tm_wday=x.GetNumberDay();

    return *this;
}

int Date::GetYear()const {return date.tm_year;}

const string & Date::GetNameMonth()const {return month_name[date.tm_mon-1];}

int Date::GetMonth()const {return date.tm_mon;}

void Date::DetermineNameDay()
{ int mon;
    if(date.tm_mon > 2)
        mon = date.tm_mon; //for march to december month code is same as month
    else{
        mon = (12+date.tm_mon); //for Jan and Feb, month code will be 13 and 14 year--; //decrease year for month Jan and Feb
    }
    int y = date.tm_year % 100; //last two digit
    int c = date.tm_year / 100; //first two digit
    int w = (double)(date.tm_mday + floor((13*(mon+1))/5) + y + floor(y/4) + floor(c/4) + (5*c));
    date.tm_wday = w % 7;
}

int Date::GetNumberDay() const
{
    return date.tm_wday;
}

const string & Date::GetNameDay()const
{
    return day_name[date.tm_wday];
}

int Date::GetDay()const {return date.tm_mday;}

int Date::GetHour()const {return date.tm_hour;}

int Date::GetMinute()const {return date.tm_min;}

ostream & operator << (ostream & os, const Date & other)
{
    if(other.date.tm_mday<10) os<<"0"<<other.date.tm_mday<<".";
    else os<<other.date.tm_mday<<".";
    if(other.date.tm_mon<10) os<<"0"<<other.date.tm_mon<<".";
    else os<<other.date.tm_mon<<".";
    os<<other.date.tm_year<<endl;
    return os;
}

string Date::GetTime() const
{
    ostringstream os;
    if(date.tm_hour<10) os<<"0"<<date.tm_hour<<":";
    else os<<date.tm_hour<<":";
    if(date.tm_min<10) os<<"0"<<date.tm_min;
    else os<<date.tm_min;
    return os.str();
}

bool Date::operator ==(const Date & other)const
{
    return date.tm_mday==other.GetDay()
    && date.tm_mon==other.GetMonth()
    && date.tm_year==other.GetYear()
    && date.tm_hour==other.GetHour()
    && date.tm_min==other.GetMinute();
}

bool Date::operator !=(const Date & other)const
{
    return !(*this==other);
}

bool Date::operator >(const Date & other)const
{
    return date.tm_year>other.GetYear() || (date.tm_year==other.GetYear() && date.tm_mon>other.GetMonth())
    || (date.tm_year==other.GetYear() && date.tm_mon==other.GetMonth() && date.tm_mday>other.GetDay())
    || (date.tm_year==other.GetYear() && date.tm_mon==other.GetMonth() && date.tm_mday==other.GetDay() && date.tm_hour>other.GetHour())
    || (date.tm_year==other.GetYear() && date.tm_mon==other.GetMonth() && date.tm_mday==other.GetDay() && date.tm_hour==other.GetHour() && date.tm_min>other.GetMinute());
}

bool Date::operator <(const Date & other)const
{
    return date.tm_year<other.GetYear() || (date.tm_year==other.GetYear() && date.tm_mon<other.GetMonth())
    || (date.tm_year==other.GetYear() && date.tm_mon==other.GetMonth() && date.tm_mday<other.GetDay())
    || (date.tm_year==other.GetYear() && date.tm_mon==other.GetMonth() && date.tm_mday==other.GetDay() && date.tm_hour<other.GetHour())
    || (date.tm_year==other.GetYear() && date.tm_mon==other.GetMonth() && date.tm_mday==other.GetDay() && date.tm_hour==other.GetHour() && date.tm_min<other.GetMinute());
}

bool Date::operator <=(const Date & other)const
{
    return *this<other || *this==other;
}