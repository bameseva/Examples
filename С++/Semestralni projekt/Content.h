#ifndef SEMESTRALNI_PRACE_CONTENT_H
#define SEMESTRALNI_PRACE_CONTENT_H

#include <string>
using namespace std;

class Content
{
private:
    string text;
public:
    explicit Content(const string & text=" "):text(text){}

    Content(const Content & other);

    Content & operator=(const Content & other);

    void SetContent(const string & new_text);

    const string & GetContent() const;
};

#endif //SEMESTRALNI_PRACE_CONTENT_H
