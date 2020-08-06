#ifndef SEMESTRALNI_PRACE_ACTIVITY_H
#define SEMESTRALNI_PRACE_ACTIVITY_H

#include "Content.h"
#include "AbstractPlansTime-consuming.h"

using namespace std;

class Activity: public AbstractTimeConsuming
{
public:
    Activity(const string & name, const  Date & plan_date,  existed time_of_repet, const string & place, const string & content)
            :AbstractTimeConsuming(name, plan_date, time_of_repet, place), adds(content) {}

    Activity(const Activity&) = default;

    Activity& operator=(const Activity& other) = default;

    //! Prints Activity in full detail to ostream
    void  FullPrint(ostream & os) const override;

    //! Prints shortened version of Activity (name, time) to ostream
    void  ShortPrint(ostream & os) const override;

    //! Changes the name, place, content of the Activity
    void ChangePlan() override ;

    shared_ptr<AbstractPlans> ClonePlan() const override;

    shared_ptr<AbstractTimeConsuming> CloneTCPlan() const override;

    const Content& GetContent()const;

    int GetCount()const override{return 2;}

    void SetContent();

    /**
     * Exports existing plans to a file
     * @param plan - output parameter, it will contain all existing plans ready to be written to a file
     */
    void Export(string & plan)override;

private:
    Content adds;
};
#endif //SEMESTRALNI_PRACE_ACTIVITY_H
