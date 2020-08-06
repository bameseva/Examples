#ifndef SEMESTRALNI_PRACE_CONSOLE_H
#define SEMESTRALNI_PRACE_CONSOLE_H

#include "Calendar.h"

class Console
{
public:

    //! Entrypoint for the application, interacts with the user until exit
   void StartAplication();

private:

    Calendar my_first_calender;

    static void PrintMainMenu();
    /*print list of possible actions, that user can do with calendar*/

    //void PrintPlan()const; //do to menu, in which user can make a choice, how h wants print calendar

    bool PrintMenuPrint();
    /*print all possibility how to print calendar*/

    //void PrintChangeMenu()const;//print all possibility how to change calendar

    void AddPlans();
    /*make a plan
     *ask user about name, date and other details
     *and add plan by the details
     * */

    void DeletePlan();
    /*remove a plan
     *ask user about name, date and other details
     *and delete plan by the details
     * */

    void ShiftPlan();
    /*move a plan
    *ask user about name, date and other details
    *and shift plan by the details
    * */

    void SearchFreeDate();
    /*looking for a free date*/

    void Export();
    /*export to file all calendar*/

    void Import();
    /*import calendar from file*/

    bool DeterminateMainChoice();
    /*determinate user's choice from main menu*/

    bool DeterminatePrintChoice();
    /*determinate user's choice from main menu*/

    void PrintDay()const;
    /*print certain day by the date, that user enter*/

    void ChangePlan();
    /*change day by name, if name doesn't exist ask again*/

    void PrintWeek()const;

    void PrintMonth()const;
};

#endif //SEMESTRALNI_PRACE_CONSOLE_H
