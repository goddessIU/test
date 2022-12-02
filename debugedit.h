#ifndef DEBUGEDIT_H
#define DEBUGEDIT_H
#include<QTextEdit>

class debugEdit : public QTextEdit
{
public:
    int findline();
    void mousePress();
public:
    int debugline=0;


protected:
    void mousePressEvent(QMouseEvent *e);

};

#endif // DEBUGEDIT_H
