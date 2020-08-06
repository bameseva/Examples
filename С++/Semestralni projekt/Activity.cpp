#include "Activity.h"
#include <iostream>
#include <sstream>

void Activity::SetContent()
{
    string task;
    while (task.empty())
    {
    getline(cin, task);
    }
    adds.SetContent(task);
}

void  Activity::FullPrint(ostream & os) const
{
    os<<"\tWhen: "<<plan_date.GetTime()<<endl;
    os<<"\tWhat: "<<name<<endl;
    os<<"\tWhere: "<<place<<endl;
    os<<"\tDetails: ";
    string detail=adds.GetContent();
    for(char x: detail)
    {
        if(x=='\n')
        {
            os<<"\n\t\t\t ";
        } else
        {
            os<<x;
        }
    }
    os<<endl;
}

void  Activity::ShortPrint(ostream & os) const {
    string norm = "\033[0m";
    string red = "\033[31m";
    string intensive = "\033[1m";
    string name_of_day = plan_date.GetNameDay();
    string short_name;
    os << "|" << plan_date.GetTime() << " ";
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

    os<<red<<short_name<<norm<<"|\n";
}

void Activity::ChangePlan()
{
    char choice;
    cout<<"What you want to change?\n";
    cout<<"0.Exit\n1.Content\n2.Place\n";
    cin>>choice;
    cin.clear();
    cin.ignore(INTMAX_MAX, '\n');
    switch(choice)
    {
        case '0':
            break;
        case '1':
            cout<<"Enter a new content, please\n";
            SetContent();
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

shared_ptr<AbstractPlans> Activity::ClonePlan() const
{
    return make_shared<Activity>(*this);
}

shared_ptr<AbstractTimeConsuming> Activity::CloneTCPlan() const
{
    return make_shared<Activity>(*this);
}

const Content& Activity::GetContent()const { return adds;}

void Activity::Export(string & plan)
{
    stringstream ss;

    ss<<GetIntRepetition()<<endl;
    ss<<plan_date.GetDay()<<endl;
    ss<<plan_date.GetMonth()<<endl;
    ss<<plan_date.GetYear()<<endl;
    ss<<plan_date.GetHour()<<endl;
    ss<<plan_date.GetMinute()<<endl;
    ss<<name<<endl;
    ss<<place<<endl;
    ss<<adds.GetContent()<<endl;
    ss<<endl;
    ss<<'~';
    plan=ss.str();
}