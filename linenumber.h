#ifndef LINENUMBER_H
#define LINENUMBER_H

#include<QTextEdit>

class LineNumber:public QTextEdit
{
public:
    int findtextline();
    void mousePress();
private:
    int line=0;

protected:
    void mousePressEvent(QMouseEvent *e);
};

#endif // LINENUMBER_H
