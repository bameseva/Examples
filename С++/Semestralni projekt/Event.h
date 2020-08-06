#ifndef SEMESTRALNI_PRACE_EVENT_H
#define SEMESTRALNI_PRACE_EVENT_H

#include "AbstractPlansTime-consuming.h"

using namespace std;

class Event: public AbstractTimeConsuming
{
public:
    Event(const string & name, const Date & plan_date, existed time_of_repet, const string & place, int h_t, int min_t)
            :AbstractTimeConsuming(name, plan_date, time_of_repet, place ), end(Time(h_t, min_t)) {}

    Event(const Event&) = default;

    Event& operator=(const Event& other) = default;

    //! Prints Event in full detail to ostream
    void  FullPrint(ostream & os) const override;

    //! Prints shortened version of Event (name, time) to ostream
    void  ShortPrint(ostream & os) const override;

    //! Changes the name, place, time and content of the Event
    void ChangePlan() override ;


    shared_ptr<AbstractPlans> ClonePlan() const override;

    shared_ptr<AbstractTimeConsuming> CloneTCPlan() const override;

    const Time& GetEndTime()const;

    void SetTimeEnd();

    int GetCount()const override {return 3;}

    /**
    * Exports existing plans to a file
    * @param plan - output parameter, it will contain all existing plans ready to be written to a file
    */
    void Export(string & plan)override;

private:
    Time end;
};
#endif //SEMESTRALNI_PRACE_EVENT_H
