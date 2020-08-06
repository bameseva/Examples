#include "Content.h"

Content::Content(const Content & other)
{
    text=other.GetContent();
}

Content & Content::operator=(const Content & other)
{
    text=other.GetContent();
    return *this;
}

void Content::SetContent(const string & new_text){text=new_text;}

const string & Content::GetContent() const{ return text;}
