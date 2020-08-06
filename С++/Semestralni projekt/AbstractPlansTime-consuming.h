#ifndef SEMESTRALNI_PRACE_ABSTRACTPLANSTIME_CONSUMING_H
#define SEMESTRALNI_PRACE_ABSTRACTPLANSTIME_CONSUMING_H

#include <iostream>
#include "Abstract_Plans.h"

class AbstractTimeConsuming: public AbstractPlans
{
protected:
    string place;

public:

    AbstractTimeConsuming(const string & name, const Date & plan_date, existed time_of_repet, const string & place):AbstractPlans(name, plan_date, time_of_repet), place(place){}

    ~AbstractTimeConsuming() = default;

    virtual shared_ptr<AbstractTimeConsuming> CloneTCPlan() const = 0;

    const string & GetPlace(){ return place;}

    void SetPlace()
    {
        string new_place;
        while (new_place.empty())
        {
            getline(cin, new_place);
        }
        place=new_place;
    }
};
#endif //SEMESTRALNI_PRACE_ABSTRACTPLANSTIME_CONSUMING_H
