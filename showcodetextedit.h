#ifndef SHOWJSONTEXTEDIT_H
#define SHOWJSONTEXTEDIT_H

#include <QTextEdit>
#include<linenumber.h>

class showCodeTextEdit : public QWidget
{
    Q_OBJECT
public:
    showCodeTextEdit(QWidget *parent = nullptr);

public:
    class debugEdit * debugedit;
    class LineNumber * lineNumberWidget;
    QTextEdit * codeTextEdit;
    int lastLine;//上一次的行

    void onTextChange();
    void onCursorPositionChanged();
    void mousepressed();
};

#endif // SHOWJSONTEXTEDIT_H
