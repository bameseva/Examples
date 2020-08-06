#ifndef SEMESTRALNI_PRACE_CALENDAR_H
#define SEMESTRALNI_PRACE_CALENDAR_H

#include <memory>
#include <map>
#include "Abstract_Plans.h"
#include "AbstractPlansTime-consuming.h"

using namespace std;

class Calendar
{
public:
    /**
     * Prints plans for one day (d, m, y) to ostream os
     * @param d - day of the date to be printed
     * @param m - month of the date to be printed
     * @param y - year of the date to be printed
     * @param os - ostream where plan be printed
     */
    void PrintDay(int d, int m, int y, ostream & os)const;

    /**
    * Prints plans for one week to ostream os
    * @param d - day of the date to be printed
    * @param m - month of the date to be printed
    * @param y - year of the date to be printed
    * @param os - ostream where plan be printed
    */
    void PrintWeek(int d, int m, int y, ostream & os)const;

    /**
    * Prints plans for one month to ostream os
    * @param d - day of the date to be printed
    * @param m - month of the date to be printed
    * @param y - year of the date to be printed
    * @param os - ostream where plan be printed
    */
    void PrintMonth(int m, int y, ostream & os)const;

    /**
     * Adds an activity to a specified date, name
     * @param d - day of the activity
     * @param m - month of the activity
     * @param y - year of the activity
     * @param h - hour of the activity
     * @param min - minute of the activity
     * @param name - name of the plan
     * @param place - place of the plan
     * @param content - content (description) of  the plan
     * @param must_repet - whether and how often the activity should repeat
     * @return false if a plan with this date already exists or if the input is malformed
     */
    bool AddPlan(int d, int m, int y, int h, int min, const string & name, const string & place, const string & content ="", existed must_repet=Once);

    /**
     * Adds an time range activity to a specified date, name
     * @param d - day of the activity
     * @param m - month of the activity
     * @param y - year of the activity
     * @param h_b - hour start of an activity
     * @param min_b - minute start of an activity
     * @param h_e - hour end of an activity
     * @param min_e - minute end of an acitivty
     * @param name - name of the plan
     * @param place - place of the plan
     * @param must_repet - whether and how often the activity should repeat
     * @return false if a plan with this date already exists or if the input is malformed
     * */
    bool AddPlan(int d, int m, int y, int h_b, int min_b, int h_e, int min_e, const string & name, const string & place, existed must_repet=Once);

    /**
    * Adds an activity to a specified date
    * @param d - day of the activity
    * @param m - month of the activity
    * @param y - year of the activity
    * @param h - hour of the activity
    * @param min - minute of the activity
    * @param name - name of the plan
    * @param content - content (description) of  the plan
    * @param must_repet - whether and how often the activity should repeat
    * @return false if a plan with this date already exists or if the input is malformed
    */
    bool AddPlan(int d, int m, int y, int h, int min, const string & name, const string & content, existed must_repet=Once);

    /**
     * Deletes a plan by name
     * @param name - name of the plan
     * @return -1 if no plans exist, 0 if the specific plan does not exist, 1 otherwise
     */
    int DeletePlan(const string & name);

    /**
     * Changes the date of the plan
     * @param name
     * @param d - new day for the plan
     * @param m - new month for the plan
     * @param y - new year for the plan
     * @param h - new hour for the plan
     * @param min -  new minute for the plan
     * @return -1 if no plans exist, 0 if the specific plan does not exist, 1 otherwise
     */
    int ShiftPlan(const string & name, int d, int m, int y, int h, int min);

    //! Finds the nearest free day
    void FindFreeDay(ostream & os);

    //! Checks whether a plan with a given name exists
    bool IfExistsName(const string & name);

    /**
     * Allows to change the plan completely (including name, content, date etc.)
     * @param name - name of the plan to be changed
     * @return 0 if no plans exist, -1 if plan does not exist, 1 otherwise
     */
    int ChangePlan(const string & name);

    /**
     * Imports plans from a given file
     * @param fileName - name of the file to import to
     * @return false if file is bad (not exist, permission denied), true otherwise
     */
    bool Import(const string & fileName);

    /**
     * Exports plans from a given file
     * @param fileName - name of the file to export to
     */
    void Export(const string & file_name);


private:

    void ChangeName(const string & name, const string & new_name);

    void AddToNeedMap(const Date&, shared_ptr<AbstractPlans> , existed must_repeat);

    void DeleteFromNeedMap(const Date & plans_date, const string & name, existed must_repeat);

    bool PrintFromOneDay(int d, int m, int y, ostream & os, bool full)const;

    bool PrintFromEveryDay(ostream & os, bool full)const;

    bool PrintFromEveryWeek(int d, int m, int y, ostream & os, bool full)const;

    bool PrintFromEveryMonth(int d, ostream & os, bool full)const;

    bool PrintFromEveryYear(int d, int m, int y, ostream & os, bool full)const;

    void ChangeDateNeedMap(Date date, multimap<Date, shared_ptr<AbstractPlans>>::iterator iterator);

    bool ParseToAdd(const string & line);

    void GetPlanCountFromOnce(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const;

    void GetPlanCountFromDay(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const;

    void GetPlanCountFromWeek(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const;

    void GetPlanCountFromMonth(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const;

    void GetPlanCountFromYear(int & count_task, int & count_event, int & count_activity, const Date & date_begin)const;

    map<string, shared_ptr<AbstractPlans>> sorted_by_name;

    multimap<Date, shared_ptr<AbstractPlans>> all_plans;

    multimap<Date, shared_ptr<AbstractPlans>> one_day_plans;

    multimap<Date, shared_ptr<AbstractPlans>> every_day_plans;

    multimap<string, shared_ptr<AbstractPlans>> every_week_plans;

    multimap<int, shared_ptr<AbstractPlans>> every_month_plans;

    multimap<pair<int, string>, shared_ptr<AbstractPlans>> every_year_plans;

};

#endif //SEMESTRALNI_PRACE_CALENDAR_H
