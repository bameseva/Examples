#include "Event.h"
#include <iostream>
#include <sstream>

void  Event::FullPrint(ostream & os) const
{
    os<<"\tWhen: "<<plan_date.GetTime()<<endl;
    os<<"\tWhat: "<<name<<endl;
    os<<"\tWhere: "<<place<<endl;
    os<<"\tWhen ends: "<<end<<endl;
}

void  Event::ShortPrint(ostream & os) const
{
    string green="\033[32m";
    string intensive="\033[1m";
    string norm="\033[0m";
    string name_of_day=plan_date.GetNameDay();
    string short_name;
    os<<"|"<<plan_date.GetTime()<<" ";
    if (name.length() >= 3)
    {
        for (int i = 0; i < 4; i++) {
            short_name.push_back(name[i]);
        }
        for(int i=0; i<3; i++)
        {
            short_name.push_back('.');
        }
    }
    else
    {
        for (size_t i = 0; i < name.length(); i++) {
            short_name.push_back(name[i]);
        }
        for(size_t i=0; i<7-name.length(); i++)
        {
            short_name.push_back(' ');
        }
    }
    os<<green<<short_name<<norm<<"|\n";
}

void  Event::ChangePlan()
{
    char choice;
    cout<<"What you want to change?\n";
    cout<<"0.Exit\n1.Time end\n3.Place\n";
    cin>>choice;
    cin.clear();
    cin.ignore(INTMAX_MAX, '\n');
    switch(choice)
    {
        case '0':
            break;
        case '1':
            cout<<"Enter a new time, please (HH:MM)\n";
            SetTimeEnd();
            break;
        case '2':
            cout<<"Enter a new place, please\n";
            SetPlace();
            break;
        default:
            cout<<"Wrong number, please try again"<<endl;
            ChangePlan();
    }
}

shared_ptr<AbstractPlans> Event::ClonePlan() const
{
 return make_shared<Event>(*this);
}

shared_ptr<AbstractTimeConsuming> Event::CloneTCPlan() const
{
    return make_shared<Event>(*this);
}

const Time& Event::GetEndTime()const { return end;}

void Event::SetTimeEnd()
{
    int hour, minute;
    char dt;
    cin>>hour>>dt>>minute;
    while(hour>23 || hour<0 || minute>59 || minute<0 || dt!=':')
    {
        cout<<"Wrong time, please, try again\n";
        cin>>hour>>dt>>minute;
    }
    end.SetTime(hour, minute);
}

void Event::Export(string & plan)
{
    stringstream ss;

    ss<<GetIntRepetition()<<endl;
    ss<<plan_date.GetDay()<<endl;
    ss<<plan_date.GetMonth()<<endl;
    ss<<plan_date.GetYear()<<endl;
    ss<<plan_date.GetHour()<<endl;
    ss<<plan_date.GetMinute()<<endl;
    ss<<name<<endl;
    ss<<endl;
    ss<<endl;
    ss<<end.GetHour()<<endl;
    ss<<end.GetMinute()<<endl;
    ss<<'~';
    plan=ss.str();
}