#include "Console.h"
#include "Calendar.h"
#include <iostream>
#include <sstream>

using namespace std;

void Console::StartAplication()
{
    cout<<"Hello!\nWelcome to your calendar!\nWhat would you like to do?\n";
    cout<<"Set, please, a number of your choice\n";
    do
    {
        PrintMainMenu();
    }
    while (DeterminateMainChoice());
}

void Console::PrintMainMenu()
{
    cout<<"0. Exit\n";
    cout<<"1. Print plans\n";
    cout<<"2. Add a plan\n";
    cout<<"3. Delete a plan\n";
    cout<<"4. Shift a plan\n";
    cout<<"5. Change a plan\n";
    cout<<"6. Search a free date\n";
    cout<<"7. Import\n";
    cout<<"8. Export\n";
}

bool Console::PrintMenuPrint()
{
    cout<<"0. Exit\n";
    cout<<"1.Print a day\n";
    cout<<"2.Print a week\n";
    cout<<"3.Print a month\n";
    return DeterminatePrintChoice();
}

void EnterTime(int& h, int& min, char&dt)
{
    cout<<"Enter a time HH:MM\n";
    cin>>h>>dt>>min;
    while( h>23 || h<0 || min>59 || min <0  || dt!=':')
    {
        cout<<"Please, try again\n" << endl;
        cin.clear();
        cin.ignore();
        EnterTime(h, min, dt);
    }
    
}

void EnterDate(int& d, int& m, int& y, char&t1, char&t2)
{
    cout<<"Enter a date DD.MM.YYYY\n";
    cin>>d>>t1>>m>>t2>>y;
    while(d>31 || d<0 || m>12 || m<0 || t1!='.' || t2!='.')
    {
        cout<<"Please, try again\n";
        cin.clear();
        cin.ignore();
        EnterDate(d, m, y, t1, t2);
    }
    
}

void GetLine(string & value)
{
    while (value.empty())
    {
        getline(cin, value);
    }
}

void Console::AddPlans()
{
    int d=0, m=0, y=0, h=0, min=0, h_e=0, min_e=0;
    char t1, t2, dt1, dt2, rep;
    string name, place, content;
    existed must_repeat=Once;
    cout<<"Plan can have different repetition.\nIt can be: 0.Once, 1.Every day, 2.Every week, 3.Every month, 4.Every year\n";
    cout<<"Enter a number of repetition\n";
    cin>>rep;
    cin.clear();
    cin.ignore(INTMAX_MAX, '\n');
    while (rep!='0' && rep!='1' && rep!='2' && rep!='3' && rep!='4')
    {
        cout<<"Wrong number, please try again\n";
        cin>>rep;
    }
    switch(rep)
    {
        case '0':
            must_repeat=Once;
            break;
        case '1':
            must_repeat=Every_day;
            break;
        case '2':
            must_repeat=Every_week;
            break;
        case '3':
            must_repeat=Every_month;
            break;
        case '4':
            must_repeat=Every_year;
            break;
    }
    cout<<"After, enter, please, name\n";
    GetLine(name);
    EnterDate(d, m, y, t1, t2);
    EnterTime(h, min, dt1);
    char place_exists='0', content_exists='0', time_end_exists='0';
    cout<<"Would you like to add place? (1.yes or 0.no)\n";
    cin >> place_exists;
    while(place_exists != '1' && place_exists != '0')
    {
        cin.clear();
        cin.ignore();
        cout<<"Please, try again\n";
        cin >> place_exists;
    }
    if(place_exists == '1')
    {
        cout<<"Please add place\n";
        GetLine(place);
    }
    cout<<"Would you like to add content? (1.yes or 0.no)\n";
    cin >> content_exists;
    while(content_exists != '1' && content_exists != '0')
    {
        cin.clear();
        cin.ignore();
        cout<<"Please, try again\n";
        cin >> content_exists;
    }
    if(content_exists == '1')
    {
        cout<<"Please add content\n";
        GetLine(content);
    }
    if(place_exists == '1' && content_exists == '0')
    {
        cout << "Would you like to add end of plan? (1.yes or 0.no)\n";
        cin >> time_end_exists;
        while(time_end_exists != '1' && time_end_exists != '0')
        {
            cin.clear();
            cin.ignore();
            cout<<"Please, try again\n";
            cin >> time_end_exists;
        }
        if (time_end_exists == '1')
        {
            cout << "Please add time end of plan (HH:MM)\n";
            cin >> h_e >> dt2 >> min_e;
            while(h_e>23 || h_e<0 || dt2!=':' || min_e>59 || min_e<0)
            {
                cout<<"Please, try again\n";
                cin.clear();
                cin.ignore();
                cin >> h_e >> dt2 >> min_e;
            }
        }
    }
    bool if_added;
    if(time_end_exists == '1')
        if_added=my_first_calender.AddPlan(d, m, y, h, min, h_e, min_e, name, place, must_repeat);
    else if(place_exists == '1')
        if_added=my_first_calender.AddPlan(d, m, y, h, min, name, place, content, must_repeat);
    else
        if_added=my_first_calender.AddPlan(d, m, y, h, min, name, content, must_repeat);

    if(if_added)
    cout<<"Plan is added\n";
}

void Console::DeletePlan()
{

    string name;
    cout<<"To delete a plan, please enter name of the plan\n";
    GetLine(name);
    if(my_first_calender.DeletePlan(name)==0)
    {
        cout<<"Please, try again\n";
        DeletePlan();
    }
}

void Console::ShiftPlan()
{
    char t1, t2, dt;
    string name;
    int d, m, y, h, min;
    cout<<"To shift a plan, please enter name of the plan\n";
    GetLine(name);
    EnterDate(d, m, y, t1, t2);
    EnterTime(h, min, dt);
    auto returns= my_first_calender.ShiftPlan(name, d, m, y, h, min);
    if(returns==-1)
    {
        cout<<"The name doesn't exist. Please, try again\n";
        ChangePlan();
    }
    else if (returns==0)
    {
        cout<<"Nothing is planned:)\n";
    }
}

void Console::SearchFreeDate()
{
    ostringstream oss;
    my_first_calender.FindFreeDay(oss);
    cout<<oss.str();
}

void Console::ChangePlan()
{
    string name;
    cout<<"To change a plan, please enter name of the plan\n";
    GetLine(name);
    auto returns=my_first_calender.ChangePlan(name);
    if(returns==-1)
    {
        cout<<"The name doesn't exist. Please, try again\n";
        ChangePlan();
    }
    else if (returns==0)
    {
        cout<<"Nothing is planned:)\n";
    }
}

void Console::Export()
{
    string file_name;
    cout<<"Enter, please, full name of file\n";
    GetLine(file_name);
    my_first_calender.Export(file_name);
}

void Console::Import()
{
    string file_name;
    cout<<"Enter, please, full name of file\n";
    GetLine(file_name);
    my_first_calender.Import(file_name);
}

void Console::PrintDay()const
{
    char t1, t2;
    int d, m, y;
    ostringstream oss;
    EnterDate(d, m, y, t1, t2);
    my_first_calender.PrintDay(d, m, y, oss);
    cout<<oss.str();
}

void Console::PrintWeek()const
{
    ostringstream oss;
    char t1, t2;
    int d, m, y;
    EnterDate(d, m, y, t1, t2);
    my_first_calender.PrintWeek(d, m, y, oss);
    cout<<oss.str();
}

void Console::PrintMonth()const
{
    ostringstream oss;
    int m, y;
    cout<<"Enter a year\n";
    cin>>y;
    cin.ignore(INTMAX_MAX, '\n');
    cout<<"Enter a month\n";
    cin>>m;
    cin.ignore(INTMAX_MAX, '\n');
    if(m>31 || m<0)
    {
        cout<<"Please, try again";
        PrintMonth();
    }
    my_first_calender.PrintMonth(m, y, oss);
    cout<<oss.str();
}

bool Console::DeterminatePrintChoice()
{
    string choice;
    GetLine(choice);
    if(choice=="0")
        return false;
    else if(choice=="1")
        PrintDay();
    else if(choice=="2")
        PrintWeek();
    else if(choice=="3")
        PrintMonth();
    else{
        cout << "Sorry, wrong number\nTry again\n";
        DeterminatePrintChoice();
    }
    return true;
}


bool Console::DeterminateMainChoice()
{
    string choice;
    GetLine(choice);
        if(choice=="0")
            return false;
        else if(choice=="1")
            PrintMenuPrint();
        else if(choice=="2")
            AddPlans();
        else if(choice=="3")
            DeletePlan();
        else if(choice=="4")
            ShiftPlan();
        else if(choice=="5")
            ChangePlan();
        else if(choice=="6")
            SearchFreeDate();
        else if(choice=="7")
            Import();
        else if(choice=="8")
            Export();
        else{
            cout<<"Sorry, wrong number\nTry again\n";
            DeterminateMainChoice();}
    return true;
}





