#include"linenumber.h"
#include<QMouseEvent>
#include<QDebug>
#include<QTextCursor>

int LineNumber::findtextline()
{
    return line;
}

void LineNumber::mousePressEvent(QMouseEvent *e)
{
    ;
}

void LineNumber::mousePress()
{

        QTextCursor cursor=this->textCursor();
        cursor=this->textCursor();
        line=cursor.blockNumber();

}
