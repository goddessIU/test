#include "debugedit.h"
#include<QMouseEvent>
#include<QDebug>
#include<QTextCursor>

QList<QTextEdit::ExtraSelection> extra_selections;
int debugEdit::findline()
{
    return debugline;
}
void debugEdit::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::LeftButton) {
        QTextCursor temcursor=this->textCursor();
        for ( int i=0; i<extra_selections.size(); i++ )
        {
            if ( extra_selections.at(i).cursor == temcursor)
            extra_selections.removeAt(i);
        }
        this->setExtraSelections(extra_selections);




    }
    if(e->button() == Qt::RightButton) {
        QTextEdit::ExtraSelection line;
        line.cursor = this->textCursor();
//        line.cursor.clearSelection();
        line.format.setBackground(QColor(Qt::red));
        line.format.setProperty(QTextFormat::FullWidthSelection, true);
        extra_selections.append(line);
        this->setExtraSelections(extra_selections);




    }

}
void debugEdit::mousePress()
{


}
