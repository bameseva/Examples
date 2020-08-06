#ifndef SEMESTRALNI_PRACE_TASK_H
#define SEMESTRALNI_PRACE_TASK_H

#include "Abstract_Plans.h"
#include "Content.h"

using namespace std;
class Task: public AbstractPlans
{
public:
    Task(const string & name, const Date& plan_date, existed time_of_repet, const string & content)
    :AbstractPlans(name, plan_date, time_of_repet), tasks(content) {}

    Task(const Task&) = default;

    Task& operator=(const Task& other) = default;

    //! Prints Task in full detail to ostream
    void  FullPrint(ostream & os) const override;

    //! Prints shortened version of Task (name, time) to ostream
    void  ShortPrint(ostream & os) const override;

    shared_ptr<AbstractPlans> ClonePlan() const override;

    //! Changes the name, place, time and content of the Task
    void ChangePlan() override;

    const Content& GetContent()const;

    void SetContent();

    int GetCount()const override{return 1;}

    /**
    * Exports existing plans to a file
    * @param plan - output parameter, it will contain all existing plans ready to be written to a file
    */
    void Export(string & plan)override;

private:
    Content tasks;
};
#endif //SEMESTRALNI_PRACE_TASK_H
