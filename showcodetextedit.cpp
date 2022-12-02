#include "showCodeTextEdit.h"
#include <QTextEdit>
#include <QHBoxLayout>
#include <QDebug>
#include <QScrollBar>
#include <QFontMetrics>
#include "myhighlighter.h"
#include"linenumber.h"
#include "debugedit.h"

showCodeTextEdit::showCodeTextEdit(QWidget *parent)
    :QWidget(parent)
{
    debugedit = new debugEdit;
    debugedit->verticalScrollBar()->hide();//隐藏滚轮轴
    debugedit->horizontalScrollBar()->hide();
    debugedit->setFocusPolicy(Qt::NoFocus);
    debugedit->setContextMenuPolicy(Qt::NoContextMenu);

    lineNumberWidget = new LineNumber;
    lineNumberWidget->verticalScrollBar()->hide();//隐藏滚轮轴
    lineNumberWidget->horizontalScrollBar()->hide();
    lineNumberWidget->insertPlainText(QStringLiteral("1\n"));
    lineNumberWidget->setFocusPolicy(Qt::NoFocus);
    lineNumberWidget->setContextMenuPolicy(Qt::NoContextMenu);

    codeTextEdit = new QTextEdit;
    codeTextEdit->setAcceptRichText(false);//禁用富文本
    codeTextEdit->setLineWrapMode(QTextEdit::NoWrap);
    Highlighter *h = new Highlighter(codeTextEdit->document());
    connect(codeTextEdit->verticalScrollBar(),&QScrollBar::valueChanged,[&](int value)
    {
        lineNumberWidget->verticalScrollBar()->setValue(value);
    });
    connect(codeTextEdit->verticalScrollBar(),&QScrollBar::valueChanged,[&](int value)
    {
        debugedit->verticalScrollBar()->setValue(value);
    });
    //保证代码行与行号一致

    QHBoxLayout * hb = new QHBoxLayout;
    hb->setMargin(0);
    hb->setSpacing(0);
    hb->addWidget(debugedit);
    hb->addWidget(lineNumberWidget);
    hb->addWidget(codeTextEdit);
    this->setLayout(hb);
    connect(codeTextEdit,&QTextEdit::textChanged,this,&showCodeTextEdit::onTextChange);
    connect(codeTextEdit,&QTextEdit::cursorPositionChanged,this,&showCodeTextEdit::onCursorPositionChanged);

    QFont font;
    font.setPixelSize(16);
    debugedit->setFont(font);
    lineNumberWidget->setFont(font);
    codeTextEdit->setFont(font);
    debugedit->setFixedWidth(40);
    lineNumberWidget->setFixedWidth(lineNumberWidget->font().pixelSize() + 15);
    lastLine = 1;

}

void showCodeTextEdit::onTextChange()
{
    int codeTextEditRow = codeTextEdit->document()->lineCount();
    if(codeTextEditRow == lastLine)
        return;

    lineNumberWidget->blockSignals(true);
    codeTextEdit->blockSignals(true);

    int countOfRow = 0;
    int temp = codeTextEditRow;
    while(temp != 0)
    {
        temp = temp/10;
        ++countOfRow;
    }
    lineNumberWidget->setFixedWidth(lineNumberWidget->font().pixelSize() * countOfRow + 15);//保证行号显示完整

    lineNumberWidget->clear();
    QString str,strs;
    ++codeTextEditRow;
    for (int i = 1; i < codeTextEditRow; ++i)
    {
        str.append(QString("%1\n").arg(i));
        strs.append(QString(" \n"));
    }
    lineNumberWidget->setPlainText(str);
    debugedit->setPlainText(strs);
    lastLine = codeTextEdit->document()->lineCount();

    lineNumberWidget->verticalScrollBar()->setValue(codeTextEdit->verticalScrollBar()->value());
    lineNumberWidget->blockSignals(false);
    codeTextEdit->blockSignals(false);
}

void showCodeTextEdit::onCursorPositionChanged()
{
    lineNumberWidget->blockSignals(true);
    codeTextEdit->blockSignals(true);
    lineNumberWidget->verticalScrollBar()->setValue(codeTextEdit->verticalScrollBar()->value());
    lineNumberWidget->blockSignals(false);
    codeTextEdit->blockSignals(false);
}
void showCodeTextEdit::mousepressed()
{

}
