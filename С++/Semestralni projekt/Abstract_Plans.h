#ifndef SEMESTRALNI_PRACE_ABSTRACT_PLANS_H
#define SEMESTRALNI_PRACE_ABSTRACT_PLANS_H

#include "Date.h"
#include "Time.h"
#include<string>
#include<ostream>
#include <memory>

using namespace std;

enum existed{Once, Every_day, Every_week, Every_month, Every_year};


class AbstractPlans
{
protected:
    string name;
    Date plan_date;
    existed repetition;
public:
    AbstractPlans(const string & name, const Date & plan_date, existed time_of_repeat)
    :name(name), plan_date(plan_date), repetition(time_of_repeat){}

    ~AbstractPlans() = default;

    virtual shared_ptr<AbstractPlans> ClonePlan() const = 0;

    //! Gets a plan in full detail to ostream
    virtual void  FullPrint(ostream & os) const = 0;

    //!Gets only name, date and time of plan to ostream
    virtual void  ShortPrint(ostream & os) const = 0;

    //! Changes the name, place, time and content
    virtual void ChangePlan() = 0 ;

    void SetName(const string & new_name){name=new_name;}

    void SetDate(const Date & new_date)
    {
        plan_date.SetDate(new_date.GetDay(), new_date.GetMonth(), new_date.GetYear(),  new_date.GetHour(), new_date.GetMinute());
    }

    const existed & GetRepetition()const { return repetition;}

    int GetIntRepetition()const { return repetition;}

    const string& GetName()const { return name;}

    const Date& GetDate()const { return plan_date;}

    /**
    * Exports existing plans to a file
    * @param plan - output parameter, it will contain all existing plans ready to be written to a file
    */
    virtual void Export(string & plan)=0;

    virtual int GetCount()const=0;
};

#endif //SEMESTRALNI_PRACE_ABSTRACT_PLANS_H
