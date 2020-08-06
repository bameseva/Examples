#include <string>
#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include <vector>
#include "Calendar.h"
#include "Activity.h"
#include "Event.h"
#include "Task.h"

bool EqualDate(const Date & date, const Date & other)
{
    return date.GetYear()==other.GetYear() && date.GetMonth()==other.GetMonth() && date.GetDay()==other.GetDay();
}

void EmptyPrint(ostream & os)
{
    os<<"Nothing is planned yet:)"<<endl;
}

void EmptyNamePrint(const string & name)
{
    cout<<"Sorry, that name already exists - "<<name<<endl;
}

bool Calendar::PrintFromOneDay(int d, int m, int y, ostream & os, bool full)const
{
    if (full)os<<"\033[4mOnly today\033[0m"<<endl;
    if(one_day_plans.empty())return false;
    Date date_begin(d, m, y, 0, 0);
    auto one_day_sekvence_begin=one_day_plans.begin();
    while(!EqualDate(one_day_sekvence_begin->first, date_begin))
    {
        one_day_sekvence_begin++;
        if(one_day_sekvence_begin==one_day_plans.end()) return false;
    }
    while( one_day_sekvence_begin->second->GetDate().GetDay()<d+1 )
    {
        if (full)one_day_sekvence_begin->second->FullPrint(os);
        else one_day_sekvence_begin->second->ShortPrint(os);
        one_day_sekvence_begin++;
        if(one_day_sekvence_begin==one_day_plans.end()) break;
    }
    return true;
}

bool Calendar::PrintFromEveryDay(ostream & os, bool full)const
{
    if (full)os<<"\033[4mEveryday\033[0m"<<endl;
    if(every_day_plans.empty())return false;
    auto every_day_sekvence_begin=every_day_plans.begin();
    while(every_day_sekvence_begin!=every_day_plans.end())
    {
        if (full)every_day_sekvence_begin->second->FullPrint(os);
        else every_day_sekvence_begin->second->ShortPrint(os);
        every_day_sekvence_begin++;
    }
    return true;
}

bool Calendar::PrintFromEveryWeek(int d, int m, int y, ostream & os, bool full)const
{
    if (full)os<<"\033[4mEvery Week\033[0m"<<endl;
    if(every_week_plans.empty())return false;
    const Date certain_day(d, m, y, 0, 0);
    string name_of_certain_day=certain_day.GetNameDay();
    auto every_week_sekvence=every_week_plans.lower_bound(name_of_certain_day);
    if(every_week_sekvence==every_week_plans.end()) return false;
    while(every_week_sekvence->first==name_of_certain_day)
    {
        if (full)every_week_sekvence->second->FullPrint(os);
        else every_week_sekvence->second->ShortPrint(os);
        every_week_sekvence++;
        if(every_week_sekvence==every_week_plans.end())break;
    }

    return true;
}

bool Calendar::PrintFromEveryYear(int d, int m, int y, ostream & os, bool full)const
{
    if (full)os<<"\033[4mEvery Year\033[0m"<<endl;
    if(every_year_plans.empty())return false;
    const Date certain_day(d, m, y, 0, 0);
    const auto key = make_pair(certain_day.GetDay(), certain_day.GetNameMonth());
    auto every_year_sekvence=every_year_plans.lower_bound(key);
    if(every_year_sekvence==every_year_plans.end()) return false;
    while(every_year_sekvence->first==key)
    {
        if (full)every_year_sekvence->second->FullPrint(os);
        else every_year_sekvence->second->ShortPrint(os);
        every_year_sekvence++;
        if(every_year_sekvence==every_year_plans.end())break;
    }
    return true;
}

bool Calendar::PrintFromEveryMonth(int d, ostream & os, bool full)const
{
    if(every_month_plans.empty())return false;
    auto every_month_sekvence = every_month_plans.lower_bound(d);
    if(every_month_sekvence==every_month_plans.end()) return false;
    while(every_month_sekvence->first==d)
    {
        if (full)every_month_sekvence->second->FullPrint(os);
        else every_month_sekvence->second->ShortPrint(os);
        every_month_sekvence++;
        if(every_month_sekvence==every_month_plans.end())break;
    }
    return true;
}

void Calendar::PrintDay(int d, int m, int y, ostream & os)const
{
    string intensive="\033[1m";
    string norm="\033[0m";
    Date date_begin(d, m, y, 0, 0);
    os<<intensive <<date_begin.GetDay()<<" "<<date_begin.GetNameMonth()<<" "<<date_begin.GetYear()<<endl;
    os<<date_begin.GetNameDay()<<norm<<endl;
    if(!all_plans.empty())
    {
        PrintFromOneDay(d, m, y, os, true);
        PrintFromEveryDay(os, true);
        PrintFromEveryWeek(d, m, y,os, true);
        PrintFromEveryMonth(d, os, true);
        PrintFromEveryYear(d, m, y, os, true);
    }
    else
    {
        EmptyPrint(os);
    }
}


void Calendar::PrintWeek(int d, int m, int y, ostream & os)const
{
    if(!all_plans.empty())
    {
        Date date_begin(d, m, y, 0, 0);
        string end = date_begin.GetNameDay();
        bool flag_day = false;
        string intensive = "\033[1m";
        string norm = "\033[0m";
        string start_line = "+-------------+";
            string name_day = date_begin.GetNameDay();
            while (name_day != end || !flag_day) {
                auto day = date_begin.GetDay();
                auto month = date_begin.GetMonth();
                auto name_month = date_begin.GetNameMonth();
                auto year = date_begin.GetYear();
                os << start_line << endl;
                os << "| " << intensive << day << " " << name_month << " " << year << " |\n";
                os << "|  " << intensive << name_day << norm;
                for (size_t i = 1; i < 12 - name_day.length(); i++)
                    os << " ";
                os << "|\n";
                PrintFromOneDay(day, month, year, os, false);
                PrintFromEveryDay(os, false);
                PrintFromEveryWeek(day, month, year, os, false);
                PrintFromEveryMonth(day, os, false);
                PrintFromEveryYear(day, month, year, os, false);

                    date_begin.SetDate(date_begin.GetDay()+1, date_begin.GetMonth(), date_begin.GetYear(), 0, 0);
                name_day = date_begin.GetNameDay();
                if (name_day != end) flag_day = true;
            }
            os << start_line << endl;
    } else
    {
        EmptyPrint(os);
    }
}

void Calendar::GetPlanCountFromOnce(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const
{
    if (!one_day_plans.empty())
    {
        int end = date_begin.GetMonth();
        auto begin_sekvence = one_day_plans.lower_bound(date_begin);
        if (begin_sekvence->second->GetDate().GetMonth() != end) begin_sekvence++;
        while (begin_sekvence->second->GetDate().GetMonth() == end)
        {
            switch (begin_sekvence->second->GetCount())
            {
                case 1:
                    count_task++;
                    break;
                case 2:
                    count_activity++;
                    break;
                case 3:
                    count_event++;
                    break;
            }
            begin_sekvence++;
            if (begin_sekvence == one_day_plans.end()) break;
        }
    }
}

void Calendar::GetPlanCountFromDay(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const
{
    if (!every_day_plans.empty())
    {
        int end = date_begin.GetMonth();
        auto begin_sekvence = every_day_plans.lower_bound(date_begin);
        if (begin_sekvence->second->GetDate().GetMonth() != end) begin_sekvence++;
        while (begin_sekvence->second->GetDate().GetMonth() == end)
        {
            switch (begin_sekvence->second->GetCount())
            {
                case 1:
                    count_task++;
                    break;
                case 2:
                    count_activity++;
                    break;
                case 3:
                    count_event++;
                    break;
            }
            begin_sekvence++;
            if (begin_sekvence == every_day_plans.end()) break;
        }
    }
}

void Calendar::GetPlanCountFromWeek(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const
{
    if (!every_week_plans.empty())
    {
        int end = date_begin.GetMonth();
        auto begin_sekvence = every_week_plans.lower_bound(date_begin.GetNameDay());
        if (begin_sekvence->second->GetDate().GetMonth() != end) begin_sekvence++;
        while (begin_sekvence->second->GetDate().GetMonth() == end)
        {
            switch (begin_sekvence->second->GetCount())
            {
                case 1:
                    count_task++;
                    break;
                case 2:
                    count_activity++;
                    break;
                case 3:
                    count_event++;
                    break;
            }
            begin_sekvence++;
            if (begin_sekvence == every_week_plans.end()) break;
        }
    }
}

void Calendar::GetPlanCountFromMonth(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const
{
    if (!every_month_plans.empty())
    {
        int end = date_begin.GetMonth();
        auto begin_sekvence = every_month_plans.lower_bound(date_begin.GetDay());
        if (begin_sekvence->second->GetDate().GetMonth() != end) begin_sekvence++;
        while (begin_sekvence->second->GetDate().GetMonth() == end)
        {
            switch (begin_sekvence->second->GetCount())
            {
                case 1:
                    count_task++;
                    break;
                case 2:
                    count_activity++;
                    break;
                case 3:
                    count_event++;
                    break;
            }
            begin_sekvence++;
            if (begin_sekvence == every_month_plans.end()) break;
        }
    }
}

void Calendar::GetPlanCountFromYear(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const
{
    if (!every_year_plans.empty())
    {
        int end = date_begin.GetMonth();
        auto begin_sekvence = every_year_plans.lower_bound(make_pair(date_begin.GetDay(), date_begin.GetNameMonth()));
        if (begin_sekvence->second->GetDate().GetMonth() != end) begin_sekvence++;
        while (begin_sekvence->second->GetDate().GetMonth() == end)
        {
            switch (begin_sekvence->second->GetCount())
            {
                case 1:
                    count_task++;
                    break;
                case 2:
                    count_activity++;
                    break;
                case 3:
                    count_event++;
                    break;
            }
            begin_sekvence++;
            if (begin_sekvence == every_year_plans.end()) break;
        }
    }
}

void Calendar::PrintMonth(int m, int y, ostream & os)const
{
    int count_task = 0;
    int count_event = 0;
    int count_activity = 0;
    string intensive = "\033[1m";
    string norm = "\033[0m";
    Date date_begin(1, m, y, 0, 0);
    if(!all_plans.empty())
    {
        GetPlanCountFromOnce(count_task, count_event, count_activity, date_begin);
        GetPlanCountFromDay(count_task, count_event, count_activity, date_begin);
        GetPlanCountFromWeek(count_task, count_event, count_activity, date_begin);
        GetPlanCountFromMonth(count_task, count_event, count_activity, date_begin);
        GetPlanCountFromYear(count_task, count_event, count_activity, date_begin);

        os << "\t" << intensive << date_begin.GetNameMonth() << " " << y << endl;
        os << "\033[33mTasks:\t\t" << count_task << endl;
        os << "\033[34mEvents:\t\t" << count_event << endl;
        os << "\033[35mActivities:\t" << count_activity << "\033[0m" << endl;
    }
    else{
        os << "\t" << intensive << date_begin.GetNameMonth() << " " << y << endl;
        os << "\033[33mTasks:\t\t" << count_task << endl;
        os << "\033[34mEvents:\t\t" << count_event << endl;
        os << "\033[35mActivities:\t" << count_activity << "\033[0m" << endl;
    }
}

void Calendar::AddToNeedMap(const Date & plans_date, shared_ptr<AbstractPlans> plan, existed must_repeat)
{
    switch(must_repeat)
    {
        case Once:
            one_day_plans.emplace(make_pair(plans_date, plan));
            break;
        case Every_day:
            every_day_plans.emplace(make_pair(plans_date, plan));
            break;
        case Every_year:
            every_year_plans.emplace(make_pair(make_pair(plans_date.GetDay(), plans_date.GetNameMonth()), plan));
            break;
        case Every_week:
            every_week_plans.emplace(make_pair(plans_date.GetNameDay(), plan));
            break;
        case Every_month:
            every_month_plans.emplace(make_pair(plans_date.GetDay(), plan));
            break;
    }
}

void Calendar::DeleteFromNeedMap(const Date & plans_date, const string & name, existed must_repeat)
{
    switch(must_repeat)
    {
        case Once:
            if(one_day_plans.find(plans_date)!=one_day_plans.end() && one_day_plans.find(plans_date)->second->GetName()==name)
                one_day_plans.erase(one_day_plans.find(plans_date));
            break;
        case Every_day:
            if(every_day_plans.find(plans_date)!=every_day_plans.end() && every_day_plans.find(plans_date)->second->GetName()==name)
                every_day_plans.erase(every_day_plans.find(plans_date));
            break;
        case Every_year:
            if(every_year_plans.find(make_pair(plans_date.GetDay(), plans_date.GetNameMonth()))!=every_year_plans.end() &&  every_year_plans.find(make_pair(plans_date.GetDay(), plans_date.GetNameMonth()))->second->GetName()==name)
                every_year_plans.erase(every_year_plans.find(make_pair(plans_date.GetDay(), plans_date.GetNameMonth())));
            break;
        case Every_week:
            if(every_week_plans.find(plans_date.GetNameDay())!=every_week_plans.end() && every_week_plans.find(plans_date.GetNameDay())->second->GetName()==name)
                every_week_plans.erase(every_week_plans.find(plans_date.GetNameDay()));
            break;
        case Every_month:
            auto to_delete_every_month=every_month_plans.find(plans_date.GetDay());
            if(to_delete_every_month->second->GetName()==name)
                every_month_plans.erase(to_delete_every_month);
            break;
    }
}

bool Calendar::IfExistsName(const string & name)
{
    auto begin_sequence=sorted_by_name.find(name);
    return(begin_sequence!=sorted_by_name.end());
}

bool Calendar::AddPlan(int d, int m, int y, int h, int min, const string & name, const string & place, const string & content, existed must_repet)
{
    Date plans_date(d, m, y, h, min);
    shared_ptr<AbstractTimeConsuming> time_plan(new Activity(name, plans_date, must_repet, place, content));
    shared_ptr<AbstractPlans> plan(time_plan);
    if(IfExistsName(name))
    {
        EmptyNamePrint(name);
        return false;
    }
    all_plans.emplace(pair<Date, shared_ptr<AbstractPlans>>(plans_date, plan));
    sorted_by_name.emplace(pair<string, shared_ptr<AbstractPlans>>(name, plan));
    AddToNeedMap(plans_date, plan, must_repet);
    return true;
}

bool Calendar::AddPlan(int d, int m, int y, int h_b, int min_b, int h_e, int min_e, const string & name, const string & place, existed must_repet)
{
    Date plans_date(d, m, y, h_b, min_b);
    shared_ptr<AbstractTimeConsuming> time_plan(new Event(name, plans_date, must_repet, place, h_e, min_e));
    shared_ptr<AbstractPlans> plan(time_plan);
    if(IfExistsName(name))
    {
        EmptyNamePrint(name);
        return false;
    }
    all_plans.emplace(pair<Date, shared_ptr<AbstractPlans>>(plans_date, plan));
    sorted_by_name.emplace(pair<string, shared_ptr<AbstractPlans>>(name, plan));
    AddToNeedMap(plans_date, plan, must_repet);
    return true;
}

bool Calendar::AddPlan(int d, int m, int y, int h, int min, const string & name, const string & content, existed must_repet)
{
    Date plans_date(d, m, y, h, min);
    shared_ptr<AbstractPlans> plan(new Task(name, plans_date, must_repet, content));
    if(IfExistsName(name))
    {
        EmptyNamePrint(name);
        return false;
    }
    all_plans.emplace(pair<Date, shared_ptr<AbstractPlans>>(plans_date, plan));
    sorted_by_name.emplace(pair<string, shared_ptr<AbstractPlans>>(name, plan));
    AddToNeedMap(plans_date, plan, must_repet);
    return true;
}

int Calendar::DeletePlan(const string & name)
{
    if(sorted_by_name.empty())
    {
        ostringstream os;
        EmptyPrint(os);
        cout<<os.str();
        return -1;
    }
    if(!IfExistsName(name))
    {
        EmptyNamePrint(name);
        return 0;
    }
    auto candidate_to_remove_name=sorted_by_name.find(name)->second;
    Date plans_date(candidate_to_remove_name->GetDate());
    auto candidate_to_remove=all_plans.lower_bound(plans_date);
    all_plans.erase(candidate_to_remove);
    sorted_by_name.erase(name);
    DeleteFromNeedMap(plans_date, name, candidate_to_remove->second->GetRepetition());
    return 1;
}

void Calendar::ChangeDateNeedMap(Date new_date, multimap<Date, shared_ptr<AbstractPlans>>::iterator old_iterator)
{
    multimap<Date, shared_ptr<AbstractPlans>>::iterator one_day;
    multimap<Date, shared_ptr<AbstractPlans>>::iterator every_day;
    multimap<string, shared_ptr<AbstractPlans>>::iterator every_week;
    multimap<int, shared_ptr<AbstractPlans>>::iterator every_month;
    multimap<pair<int, string>, shared_ptr<AbstractPlans>>::iterator every_year;
    Date end;
    switch(old_iterator->second->GetRepetition())
    {
        case Once:
            one_day=one_day_plans.find(old_iterator->first);
            end=one_day->second->GetDate();
            while(one_day->second->GetName()!=one_day->second->GetName() && EqualDate(end, one_day->first))
                one_day--;
            one_day_plans.emplace(make_pair(new_date, one_day->second));
            one_day_plans.erase(one_day);
            break;
        case Every_day:
            every_day=every_day_plans.find(old_iterator->first);
            end=every_day->second->GetDate();
            while(every_day->second->GetName()!=every_day->second->GetName() && EqualDate(end, every_day->first))
                every_day--;
            every_day_plans.emplace(make_pair(new_date, every_day->second));
            every_day_plans.erase(every_day);
            break;
        case Every_year:
            every_year=every_year_plans.find(make_pair(old_iterator->second->GetDate().GetDay(), old_iterator->second->GetDate().GetNameMonth()));
            end=every_year->second->GetDate();
            while(every_year->second->GetName()!=every_year->second->GetName() && EqualDate(end, every_year->second->GetDate()))
                every_year--;
            every_year_plans.emplace(make_pair(make_pair(new_date.GetDay(), new_date.GetNameMonth()), every_year->second));
            every_year_plans.erase(every_year);
            break;
        case Every_week:
            every_week=every_week_plans.find(old_iterator->first.GetNameDay());
            end=every_week->second->GetDate();
            while(every_week->second->GetName()!=every_week->second->GetName() && EqualDate(end, every_week->second->GetDate()))
                every_week--;
            every_week_plans.emplace(make_pair(new_date.GetNameDay(), every_week->second));
            every_week_plans.erase(every_week);
            break;
        case Every_month:
            every_month=every_month_plans.find(old_iterator->first.GetDay());
            end=every_month->second->GetDate();
            while(every_month->second->GetName()!=every_month->second->GetName() && EqualDate(end, every_month->second->GetDate()))
                every_month--;
            every_month_plans.emplace(make_pair(new_date.GetDay(), every_month->second));
            every_month_plans.erase(every_month);
            break;
    }
}

int Calendar::ShiftPlan(const string & name, int d, int m, int y, int h, int min)
{
    if(sorted_by_name.empty())return 0;
    if(!IfExistsName(name))
    {
        EmptyNamePrint(name);
        return -1;
    }
    Date plans_date_t(d, m, y, h, min);
    auto candidate_to_shift=sorted_by_name.find(name)->second;
    auto candidate_to_shift_by_date=all_plans.find(candidate_to_shift->GetDate());
    Date end(candidate_to_shift->GetDate().GetDay(), candidate_to_shift->GetDate().GetMonth(), candidate_to_shift->GetDate().GetYear(), candidate_to_shift->GetDate().GetHour(), candidate_to_shift->GetDate().GetMinute());
    while(candidate_to_shift_by_date->second->GetName()!=name && EqualDate(end, candidate_to_shift->GetDate()))
        candidate_to_shift_by_date--;
    all_plans.insert(pair<Date, shared_ptr<AbstractPlans>>(plans_date_t, candidate_to_shift));
    ChangeDateNeedMap(plans_date_t, candidate_to_shift_by_date);
    all_plans.erase(candidate_to_shift_by_date);
    candidate_to_shift->SetDate(plans_date_t);
    return 1;
}

void Calendar::FindFreeDay(ostream & os)
{
    time_t time_ptr;
    multimap<Date,shared_ptr<AbstractPlans>>::iterator possible_free_date;
    tm* tm_local;
    time_ptr = time(NULL);
    tm_local = localtime(&time_ptr);
    int counter=0;
    Date next_day= Date(tm_local->tm_mday+1, tm_local->tm_mon+1, tm_local->tm_year+1900, 23, 59 );
    Date local=Date(tm_local->tm_mday, tm_local->tm_mon+1, tm_local->tm_year+1900, 23, 59);
    while (true)
    {
        possible_free_date=one_day_plans.upper_bound(local);
        if(possible_free_date==one_day_plans.end()) break;
        else
        {
            if(possible_free_date->first.GetYear()>next_day.GetYear()
            && possible_free_date->first.GetMonth()>next_day.GetMonth()
            && possible_free_date->first.GetDay()>next_day.GetDay())break;
            else
            {
               local.SetDate(tm_local->tm_mday+counter, tm_local->tm_mon+1, tm_local->tm_year+1900, tm_local->tm_hour, tm_local->tm_min );
               counter++;
            }
       }
   }
   if(counter==0)
       os<<next_day<<endl;
   else os<<local<<endl;
}

void Calendar::ChangeName(const string & name, const string & new_name)
{
    auto change_by_name=sorted_by_name.find(name);
    auto change_by_date=all_plans.find(change_by_name->second->GetDate());
    change_by_date->second->SetName(new_name);
    sorted_by_name.emplace(pair<string, shared_ptr<AbstractPlans>>(new_name, change_by_name->second));
    sorted_by_name.erase(name);
}

int Calendar::ChangePlan(const string & name)
{
    if(sorted_by_name.empty()) return 0;
    if(!IfExistsName(name))
    {
        EmptyNamePrint(name);
        return -1;
    }
    auto plan_to_change=sorted_by_name.find(name);
    char choice;
    string new_name;
    cout<<"What you want to change?\n";
    cout<<"0.Exit\n1.Name\n2.Details\n";
    cin>>choice;
    cin.clear();
    cin.ignore(INTMAX_MAX, '\n');
    switch(choice)
    {
        case '0':
            break;
        case '1':
            cout<<"Enter a new name, please\n"<<endl;
            while(new_name.empty())
            {
                getline(cin, new_name);
            }
            ChangeName(name, new_name);
            break;
        case '2':
            plan_to_change->second->ChangePlan();
            break;
        default:
            cout<<"Wrong number, please try again\n"<<endl;
            ChangePlan(name);
    }
    return 1;
}

bool Calendar::ParseToAdd(const string & line)
{
    string name, place, content;
    string  d, m, y, h, min, h_e, min_e;
    char rep='-';
    existed must_repeat;
    int flag=0;
    for(char i : line)
    {
        if((i>'9' || i<'0') && flag==0 && rep=='-') return false;
        if(i=='\n')
        {
            flag++;
            continue;
        }
        switch(flag)
        {
            case 0:
                rep=i;
                break;
            case 1:
                d.push_back(i);
                break;
            case 2:
                m.push_back(i);
                break;
            case 3:
                y.push_back(i);
                break;
            case 4:
                h.push_back(i);
                break;
            case 5:
                min.push_back(i);
                break;
            case 6:
                name.push_back(i);
                break;
            case 7:
                place.push_back(i);
                break;
            case 8:
                content.push_back(i);
                break;
            case 9:
                h_e.push_back(i);
                break;
            case 10:
                min_e.push_back(i);
                break;
            default:
                return false;
        }
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
        default:
            return false;
    }
    if(!h_e.empty())
        return AddPlan(stoi( d ), stoi( m ), stoi( y ), stoi( h ), stoi( min ), stoi( h_e ), stoi( min_e ), name, place, must_repeat);
    else if(!place.empty())
        return AddPlan(stoi( d ), stoi( m ), stoi( y ), stoi( h ), stoi( min ),  name, place, content, must_repeat);
    else
        return AddPlan(stoi( d ), stoi( m ), stoi( y ), stoi( h ), stoi( min ),  name, content, must_repeat);
}

bool Calendar::Import(const string & file_name)
{
    ifstream file(file_name);
    vector<string> lines;
    string line;
    if (file.fail() || !file.is_open() || file.bad()) {
        cout << "Invalid file" << endl;
        file.close();
        return false;
    }
    char ends='~';
    while( getline(file, line, ends))
    {
        lines.push_back(line);
    }
    for (string & x: lines)
    if(!ParseToAdd(x))
    {
        cout << "Invalid file" << endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}

void Calendar::Export(const string & file_name)
{
    std::ofstream file(file_name);
    auto begin=all_plans.begin();
    string plan;
    while(begin!=all_plans.end())
    {
        begin->second->Export(plan);
        file<<plan;
        plan.clear();
        begin++;
    }
    file.close();
}


