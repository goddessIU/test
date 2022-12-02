#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>//消息对话框
#include <QFile>
#include <QFileDevice>
#include <QTextStream>
#include <QtEvents>
#include <QMenu>
#include <QAction>
#include <QTextEdit>
#include<QProcess>
#include<QTextCodec>
#include<QFileInfo>
#include<QDir>
#include<QDebug>
#include<QTextCursor>
#include<QTextBlock>


void MainWindow::File_Initial(){
    Opening = false;
    Creating = false;
    Remain_File_Address = "";
    Remain_File_Content = "";
    Compliered=false;
    if(!Opening && !Creating) ui->textEditMain->codeTextEdit->clear();

    connect(ui->actionNewFile,SIGNAL(triggered()),this,SLOT(CreateFile()));
    connect(ui->actionOpenFile,SIGNAL(triggered()),this,SLOT(OpenFile()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(SaveFile()));
    connect(ui->actionSaveAs,SIGNAL(triggered()),this,SLOT(SaveAs()));
    connect(ui->actionExit_2,SIGNAL(triggered()),this,SLOT(Exit()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(OpenHelp()));
    connect(ui->actionComplier,SIGNAL(triggered()),this,SLOT(Complier()));
    connect(ui->actionRun,SIGNAL(triggered()),this,SLOT(Run()));
    for(int i=0;i<200;i++)
        {
            fold[i][0]=0;
            fold[i][1]=0;
            fold[i][2]=0;
            tempfolds[i]=0;
        }
}


void MainWindow::Multiple_Window(){

}



void MainWindow::Bracket_Match(){
   ui->textEditMain->codeTextEdit->installEventFilter(this);
}

void MainWindow::SetType_Auto(){
    QTextCursor cur=ui->textEditMain->codeTextEdit->textCursor();
    int curPosition=cur.position();
    cur.movePosition(QTextCursor::Start,QTextCursor::KeepAnchor);
    QString curSelectedData=cur.selectedText();
    QByteArray ba=curSelectedData.toLatin1();
    char c;//从curSelectedData中提取出的字符
    int n=0;//判断字符串中括号个数
    for(int i=0;i<curPosition;i++){
        c=ba[i];
        if(c=='('||c=='{'||c=='[')
            n++;
        if(c==')'||c=='}'||c==']')
            n--;
    }
    cur.setPosition(curPosition,QTextCursor::MoveAnchor);
    cur.movePosition(QTextCursor::EndOfLine,QTextCursor::MoveAnchor);
    int lineEnd=cur.position();
    cur.setPosition(curPosition,QTextCursor::MoveAnchor);
    if(lineEnd>curPosition){
        cur.setPosition(curPosition+1,QTextCursor::KeepAnchor);
        QString specialType=cur.selectedText();
        QByteArray ba1=specialType.toLatin1();
        cur.setPosition(curPosition,QTextCursor::MoveAnchor);
        if(ba1[0]==')'||ba1[0]=='}'||ba1[0]==']'){
            cur.insertBlock();//换行
            ui->textEditMain->codeTextEdit->setAlignment(Qt::AlignLeft);
            for(int i=0;i<3*(n-1);i++){//在行开头输入对应空格
                cur.insertText(" ");
            }
        }else{
            cur.insertBlock();//换行
            ui->textEditMain->codeTextEdit->setAlignment(Qt::AlignLeft);//删除当前行的空格
            for(int i=0;i<3*n;i++){//在行开头输入对应空格
                cur.insertText(" ");
            }
        }
    }else{
        cur.insertBlock();
        ui->textEditMain->codeTextEdit->setAlignment(Qt::AlignLeft);
        for(int i=0;i<3*n;i++){
            cur.insertText(" ");
        }
    }
}


bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == ui->textEditMain->codeTextEdit)      //判断过滤事件是否是发送窗口事件
    {
        if(ev->type() == QEvent::KeyPress)   //过滤键盘事件
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev); //强行转换为按键事件
            int keyValue = keyEvent->key();       //获取按键值
            QTextCursor tc = ui->textEditMain->codeTextEdit->textCursor();
            if(keyValue==Qt::Key_ParenLeft){
                tc.insertText("()");
                ui->textEditMain->codeTextEdit->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
            }
            else if (keyValue==Qt::Key_BracketLeft){
                tc.insertText("[]");
                ui->textEditMain->codeTextEdit->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
            }
            else if (keyValue ==Qt::Key_BraceLeft){
                tc.insertText("{");
                SetType_Auto();
                SetType_Auto();
                tc.insertText("}");
                tc.movePosition(QTextCursor::StartOfLine);
                for(int i=0;i<3;i++) tc.deleteChar();
                ui->textEditMain->codeTextEdit->moveCursor(QTextCursor::Up,QTextCursor::MoveAnchor);
                ui->textEditMain->codeTextEdit->moveCursor(QTextCursor::EndOfLine,QTextCursor::MoveAnchor);
            }
            else if (keyValue ==Qt::Key_Left)
                ui->textEditMain->codeTextEdit->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
            else if(keyValue==Qt::Key_Return)//自动排版，enter为开始标志
                SetType_Auto();
            else return 0;
        }
    }
}


void MainWindow::CreateFile(){
    ExitFile();//先把旧的文件关了
    Creating = true;
    ui->textEditMain->codeTextEdit->clear();
    ui->textEditMain->setHidden(false);     //显示文本框
}

void MainWindow::OpenFile()
{
    ExitFile();
    ui->textEditMain->setHidden(false);     //显示文本框
    QString address;
    address = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("C source files(*.c;*.cpp);;All files(*.*);;Text File (*.txt)"));//第一个参数为父类
    if(address == "")    return;//直接关闭了文件浏览对话框
    else Remain_File_Address = address;//保存文件名
    QFile file(address);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
        return;
    }
    else
    {
        if(!file.isReadable())
        {
            QMessageBox::warning(this,tr("错误"),tr("该文件不可读"));
            return;
        }
        else
        {
            Opening = true;
            QTextStream textStream(&file);//读取文件，使用QTextStream
            while(!textStream.atEnd())
            {
                ui->textEditMain->codeTextEdit->setPlainText(textStream.readAll());
            }
            Remain_File_Content = ui->textEditMain->codeTextEdit->toPlainText();
            file.close();
            ui->textEditMain->codeTextEdit->show();
            showProjectName();
        }
    }
}

void MainWindow::SaveFile(){//不仅要判断是新文件还是旧的，还要判断是否希望保存
    if(!Opening && !Creating)  return;//如果没有能保存的
    Allunfold();
    QString content = ui->textEditMain->codeTextEdit->toPlainText();
    if(content== "")
    {
        QMessageBox::warning(this,tr("警告"),tr("内容不能为空!"),QMessageBox::Ok);
        return;
    }
    if(Creating){
        QString address = QFileDialog::getSaveFileName(this,tr("Save As"),"/test",tr("All files(*.*);;Text File (*.txt);;C source files(*.c;*.cpp)"));
        if(address == "")  return;
        else Remain_File_Address = address;//保存文件名
        QFile file(address);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,tr("错误"),tr("打开文件失败"),QMessageBox::Ok);
            return;
        }
        else
        {
            Remain_File_Content = content;
            QTextStream textStream(&file);
            textStream<<content;
            file.close();
            Creating = false;
            Opening = true;//就转变为打开的文件了
            Refold();
        }
    }
    else if(Opening){
        QFile file(Remain_File_Address);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,tr("警告"),tr("打开文件失败"));
            return;
        }
        else
        {
            Remain_File_Content = content;
            QTextStream textStream(&file);
            textStream<<content;
            file.close();
            Refold();
        }
    }
    else;
}


void MainWindow::SaveAs(){
    if(!Opening && !Creating)  return;
    QString content = ui->textEditMain->codeTextEdit->toPlainText();
    if(content== "")
    {
        QMessageBox::warning(this,tr("警告"),tr("内容不能为空!"),QMessageBox::Ok);
        return;
    }
    QString address = QFileDialog::getSaveFileName(this,tr("Save As"),"/test",tr("All files(*.*);;Text File (*.txt);;C source files(*.c;*.cpp)"));
    if(address == "")  return;
    else Remain_File_Address = address;//保存文件名
    QFile file(address);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("错误"),tr("打开文件失败"),QMessageBox::Ok);
        return;
    }
    else
    {
        Remain_File_Content = content;
        Allunfold();
        QTextStream textStream(&file);
        textStream<<content;
        file.close();
        if(Creating){
            Creating = false;
            Opening = true;
        }
        Refold();
    }
}


void MainWindow::Exit(){
    ExitFile();//先把文件关了
    exit(0);
}



void MainWindow::ExitFile(){
    if(!Opening && !Creating) ui->textEditMain->codeTextEdit->clear();//未打开文件
    else{
        QString content = ui->textEditMain->codeTextEdit->toPlainText();
        if((Creating && content!="")||(content!=Remain_File_Content && Opening)){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,tr("警告"),tr("是否要先保存"));
            if(reply == QMessageBox::StandardButton::Yes)
            SaveFile();//未保存文件
        }
    }
    Remain_File_Address = "";
    Remain_File_Content = "";
    Opening = false;
    Creating = false;
}

void MainWindow::OpenHelp() {
     QMessageBox::information(this,"帮助","team 关东的ide，可以按照现有的dev c++等的使用方式来使用");
}

void MainWindow::Complier(){

    if(!Opening && !Creating)  {
        QMessageBox::warning(this,tr("错误"),tr("请先打开或创建文件"),QMessageBox::Ok);
        return;
    }
//    if (!Saved){
//            QMessageBox::warning(this,tr("错误"),tr("请先保存"),QMessageBox::Ok);
//            SaveFile();//未保存文件
//        }
//    else
//    {
        SaveFile();
        Allunfold();
    QFileInfo info(Remain_File_Address);
    QString cppinfo=info.absolutePath()+"/"+info.baseName()+'.'+info.suffix();
    QString command="g++ -o "+info.absolutePath()+"/"+info.baseName()+" "+cppinfo;
    process.setProgram(info.absolutePath());
    process.open(QIODevice::ReadWrite);
    //编译输出
    process.start(command);
    process.waitForFinished();
    ui->textBrowser_output->clear();
    QByteArray qbaError  = process.readAllStandardError();
    if(qbaError!=""){
        char* srcError = qbaError.data();
        ui->textBrowser_output->insertPlainText(srcError);
        ui->textBrowser_output->moveCursor(QTextCursor::End);
        QMessageBox::warning(this,tr("错误"),tr("编译出错"),QMessageBox::Ok);
        Compliered=false;
        return;
    }

    Compliered=true;
    QString exedir=QDir::currentPath();
    QString todir=info.absolutePath();
    QFile exefile(QDir::currentPath()+"/"+info.baseName()+".exe");
    exefile.copy(exedir,todir);
    Refold();
//    }
}

void MainWindow::Run(){
    if(!Opening && !Creating)  {
        QMessageBox::warning(this,tr("错误"),tr("请先打开或创建文件"),QMessageBox::Ok);
        return;
    }
    if(!Compliered)
    {
        QMessageBox::warning(this,tr("错误"),tr("请先编译"),QMessageBox::Ok);
        Complier();//未编译文件
    }
//    QProcess process;
    QFileInfo info(Remain_File_Address);
    process.setProgram(info.absolutePath());
    process.open(QIODevice::ReadWrite);
    QString todo=ui->textEditin->toPlainText();
    QString exeinfo=info.absolutePath()+"/"+info.baseName()+".exe";
    process.start(exeinfo);
    process.write(todo.toUtf8());
    if(!process.waitForFinished(3000))
    {QMessageBox::warning(this,tr("错误"),tr("请正确输入"),QMessageBox::Ok);
        return;
    }
    process.waitForReadyRead();
    //   QByteArray qba  = process.readAll();

    //    //解决中文乱码问题
    //    QTextCodec* pTextCodec = QTextCodec::codecForName("System");
    //    if(pTextCodec != nullptr);
    //    QString str = pTextCodec->toUnicode(qba);

    //   qDebug("%s\n",str.toStdString().c_str());
        ui->textBrowser_output->clear();
        QByteArray qba[50];//输出行数最多为50行
        int qbaNumber=0;
        while((qba[qbaNumber]=process.readLine())!="")
        {
            QTextCodec* pTextCodec = QTextCodec::codecForName("System");
            if(pTextCodec != nullptr);
            QString str = pTextCodec->toUnicode(qba[qbaNumber]);
            QByteArray ba1 = str.toLatin1();
            char* src1 = ba1.data();
            ui->textBrowser_output->insertPlainText(src1);
            ui->textBrowser_output->moveCursor(QTextCursor::End);
            qbaNumber++;

        }
}

void MainWindow::on_pushButtonFold_clicked()
{
    ui->textEditMain->lineNumberWidget->mousePress();
    findline();
}

void MainWindow::findline()
{
    findoverline(ui->textEditMain->lineNumberWidget->findtextline());
}

void MainWindow::findoverline(int begin)
{
    int over;
    QString name="void";
    QString tempstr;
    int j=0;
    tempstr=ui->textEditMain->codeTextEdit->document()->findBlockByLineNumber(begin).text();
    for(j=0;j<tempstr.size();j++)
    {
        if(tempstr.at(j)=="{")
            break;
    }
    name=tempstr.mid(0,j);
    int i=begin,end,barnum=0;
    do
    {
        tempstr=ui->textEditMain->codeTextEdit->document()->findBlockByLineNumber(i).text();
        for(j=0;j<tempstr.size();j++)
        {
            if(tempstr.at(j)=="{")
                barnum++;
            else if(tempstr.at(j)=="}")
                barnum--;
        }
        i++;
    }while(!(barnum==0||i==end));
    if(barnum==0)
    {
        over=i-1;
        foldorunfold(begin,over,name);
    }
    else
        QMessageBox::warning(this,tr("错误"),tr("括号不匹配"),QMessageBox::Ok);
}

void MainWindow::foldorunfold(int begin,int over,QString name)
{
    int realbegin=begin;
    int realover=over;
    //把begin和end由显示行数变成实际行数
    int flag=1;//用于判断是否还有没计算到的折叠导致界面行数计算得到的实际行数不同
    int tempfold[200]={0};//存储增加折叠情况
    while(flag==1)
    {
        flag=0;
        for(int k=0;k<200;k++)//判断显示行数
        {
            if(fold[k][0]<realbegin&&fold[k][2]==1&&tempfold[k]==0)
            {
                realbegin+=fold[k][1]-fold[k][0];
                realover+=fold[k][1]-fold[k][0];
                tempfold[k]=1;//加过一次之后就不加了
            }
        }
        for(int k=0;k<200;k++)
        {
            if(fold[k][0]<realbegin&&fold[k][2]==1&&tempfold[k]==0)
            {
                flag=1;//如果存在加完之后变成超过的的话就再次循环
            }
        }
    }
    //判断是否折叠
    if(realbegin==realover)//已经折叠进行展开
    {
        //指定行数begin加入内容
        for(int i=0;i<200;i++)
        {
            //找到这个折叠的存储位置
            if(realbegin==fold[i][0]&&realover+folded[i].size()==fold[i][1]+1&&fold[i][2]==1)
            {
                for(int j=begin;j<over+folded[i].size();j++)
                {
                    QTextCursor tempcursor = ui->textEditMain->codeTextEdit->textCursor();
                    tempcursor.movePosition(QTextCursor::Start);
                    tempcursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, j);
                    tempcursor.select(QTextCursor::LineUnderCursor);
                    QString toadd=folded[i].at(j-begin);
                    if(j<over+folded[i].size()-1)
                        tempcursor.insertText(toadd+"\n");
                    else
                        tempcursor.insertText(toadd);
                }
                fold[i][2]=0;//存储未折叠情况
                return;
            }
        }
        return;
    }
    //未折叠进行折叠
    int j=0;
    for(int i=0;i<200;i++)//寻找空位
    {
        if(fold[i][0]==0&&fold[i][1]==0)
        {
            j=i;
            break;
        }
    }
    //存储实际行数
    fold[j][0]=realbegin;
    fold[j][1]=realover;
    fold[j][2]=1;
    for(int i=begin;i<over+1;over--)
    {
        //存储折叠内容
        folded[j].append(ui->textEditMain->codeTextEdit->document()->findBlockByLineNumber(i).text());
        //在折叠的位置添加折叠后的结果
        QTextCursor tempcursor = ui->textEditMain->codeTextEdit->textCursor();
        if(i==begin)
        {
            tempcursor.movePosition(QTextCursor::Start);
            tempcursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, begin);
            tempcursor.select(QTextCursor::LineUnderCursor);
            QString toadd=name+"{....}";
            tempcursor.insertText(toadd);
            i++;
            over++;
        }
        else
        {
            //删除界面显示内容
            tempcursor.movePosition(QTextCursor::Start);
            tempcursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, i);
            tempcursor.select(QTextCursor::LineUnderCursor);
            tempcursor.removeSelectedText();
            tempcursor.deleteChar();
        }
    }
}

void MainWindow::Allunfold()
{
    int i=0;
    for(i=0;i<200;i++)
    {
        tempfolds[i]=fold[i][2];
    }
    for(i=0;i<200;i++)
    {
        int num=0,j=0;
        if(fold[i][2]==1)
        {
            for(j=0;j<200;j++)
            {
                if(fold[j][0]<fold[i][0]&&fold[j][2]==1)
                    num+=folded[j].size();
            }
            QString tempstr;
            tempstr=ui->textEditMain->codeTextEdit->document()->findBlockByLineNumber(fold[i][0]-num).text();
            for(j=0;j<tempstr.size();j++)
            {
                if(tempstr.at(j)=="{")
                    break;
            }
            QString name;
            name=tempstr.mid(0,j);
            foldorunfold(fold[j][0]-num,fold[j][0]-num,name);
        }
    }
}

void MainWindow::Refold()
{
    int i=0;
    for(i=0;i<200;i++)
    {
        int num=0,j=0;
        if(tempfolds[i]==1)
        {
            for(j=0;j<200;j++)
            {
                if(fold[j][0]<fold[i][0]&&fold[j][2]==1)
                    num+=folded[j].size();
            }
            QString tempstr;
            tempstr=ui->textEditMain->codeTextEdit->document()->findBlockByLineNumber(fold[i][0]-num).text();
            for(j=0;j<tempstr.size();j++)
            {
                if(tempstr.at(j)=="{")
                    break;
            }
            QString name;
            name=tempstr.mid(0,j);
            foldorunfold(fold[j][0]-num,fold[j][1]-num,name);
        }
    }
}
void MainWindow::showProjectName()
{
    QFileInfo info(Remain_File_Address);
    QString cppinfoname=info.baseName()+'.'+info.suffix();

    ui->labelProjectName->setText(cppinfoname);
//    ui->labelProjectName->setText(info.absolutePath());

    model=new QFileSystemModel(this); //QFileSystemModel提供单独线程，推荐使用
    model->setRootPath(info.absolutePath()); //设置根目录

    //设置过滤器
    QStringList filter;
    filter<<"*.cpp"<<"*.c";
    model->setNameFilters(filter);

    //隐藏没有通过过滤器的文件，false为隐藏
    model->setNameFilterDisables(false);

    ui->treeViewProject->setModel(model); //设置数据模型
    ui->treeViewProject->setRootIndex(model->index(info.absolutePath()));
}
