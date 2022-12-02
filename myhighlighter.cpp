#include "myhighlighter.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;


    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPattern1, keywordPattern2, keywordPattern3; //关键词集合,关键都以正则表达式表示 下面的可以改为我们想要的关键词
    keywordPattern1 << "\\bchar\\b" << "\\bvoid\\b" << "\\bint\\b" << "\\bdouble\\b" << "\\bfloat\\b" << "\\bbool\\b" << "\\bstring\\b" << "\\bvector\\b" << "\\bmap\\b" << "\\bset\\b" <<
                       "\\barraylist\\b" << "\\bstruct\\b" << "\\bclass\\b";
    keywordPattern2 << "\\bif\\b" << "\\belse\\b" << "\\bwhile\\b" << "\\bfor\\b" << "\\bforeach\\b" << "\\bbreak\\b" << "\\breturn\\b" << "\\bcontinue\\b" << "\\bswitch\\b" << "\\bcase\\b" <<
                       "\\bgoto\\b";
    keywordPattern3 << "\\bdefine\\b" << "\\bconst\\b" << "\\bprivate\\b" << "\\bpublic\\b" << "\\bprotected\\b" << "\\binclude\\b" << "\\bstatic\\b" << "\\busing\\b";
    keywordFormat.setForeground(QColor(255,189,255));//设定关键词的高亮样式
    foreach (const QString &pattern, keywordPattern1) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
//! [0]

//! [1]
    keywordFormat.setForeground(QColor(255,255,193));//设定关键词的高亮样式
    foreach (const QString &pattern, keywordPattern2) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    keywordFormat.setForeground(Qt::lightGray);//设定关键词的高亮样式
    foreach (const QString &pattern, keywordPattern3) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
//! [1]

//! [2]
    classFormat.setFontWeight(QFont::Bold);//添加Qt的类到高亮规则中
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);
//! [2]

//! [3]

//! [3]

//! [4]
    quotationFormat.setForeground(QColor(176,255,176));//字符串
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
//! [4]

//! [5]
    functionFormat.setFontItalic(true);//函数
    functionFormat.setForeground(QColor(189,255,255));
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(QColor(255,145,145));//单行注释
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(255,145,145));//多行注释，只设定了样式，具体匹配在highlightBlock中设置
//! [5]

//! [6]
    commentStartExpression = QRegularExpression("/\\*");//多行注释的匹配正则表达式
    commentEndExpression = QRegularExpression("\\*/");


}
//! [6]

//! [7]
void Highlighter::highlightBlock(const QString &text)//应用高亮规则，也用于区块的高亮，比如多行注释
{
    foreach (const HighlightingRule &rule, highlightingRules) {//文本采用高亮规则
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
//! [7] //! [8]
    setCurrentBlockState(0); //以下是多行注释的匹配
//! [8]

//! [9]
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

//! [9] //! [10]
    while (startIndex >= 0) {
//! [10] //! [11]
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
//! [11]




//keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
//                << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
//                << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
//                << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
//                << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
//                << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
//                << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
//                << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
//                << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
//                << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b";
