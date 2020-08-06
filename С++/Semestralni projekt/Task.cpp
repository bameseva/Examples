#include <iostream>
#include <sstream>
#include "Task.h"

using namespace std;

void  Task::FullPrint(ostream & os) const
{
    os<<"\tWhen: "<<plan_date.GetTime()<<endl;
    os<<"\tWhat: "<<name<<endl;
    os<<"\tDetails: ";
    string detail=tasks.GetContent();
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

void  Task::ShortPrint(ostream & os) const
{
    string aquamarine="\033[36m";
    string norm="\033[0m";
    string intensive="\033[1m";
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
    os<<aquamarine<<short_name<<norm<<"|\n";
}

void  Task::ChangePlan()
{
    char choice;
    cout<<"What you want to change?\n";
    cout<<"0.Exit\n1.Content\n";
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
        default:
            cout<<"Wrong number, please try again"<<endl;
            ChangePlan();
    }
}

shared_ptr<AbstractPlans> Task::ClonePlan() const
{
    return make_shared<Task>(*this);
}

const Content& Task::GetContent()const { return tasks;}

void Task::SetContent()
{
    string task;
    while(task.empty())
    {
        getline(cin, task);
    }
    tasks.SetContent(task);
}

void Task::Export(string & plan)
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
    ss<<tasks.GetContent()<<endl;
    ss<<endl;
    ss<<endl;
    ss<<'~';
    plan=ss.str();
}