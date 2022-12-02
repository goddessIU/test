#include "mainwindow.h"
#include <QKeyEvent>
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextCursor>
#include <QMessageBox>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEditMain->setHidden(true);

    ui->horizontalGroupBox_4->hide();
    ui->horizontalGroupBox_3->hide();
    Windows = 0;


//    model=new QFileSystemModel(this); //QFileSystemModel提供单独线程，推荐使用
//    model->setRootPath(""); //设置根目录
//    ui->treeViewProject->setModel(model); //设置数据模型

    File_Initial();
    Multiple_Window();
    Bracket_Match();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actioncha_triggered()//查找的行为产生
{
    FindFrom = 0;
    ui->horizontalGroupBox_4->show();
    ui->horizontalGroupBox_3->show();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->horizontalGroupBox_4->hide();
    ui->horizontalGroupBox_3->hide();
}
void MainWindow::keyPressEvent( QKeyEvent *k )
{
    if(k->key() == Qt::Key_Control)
    {
      if(k->key() == Qt::Key_F)
      {
          ui->horizontalGroupBox_4->show();
          ui->horizontalGroupBox_3->show();
      }
    }
}

void MainWindow::on_actionClass_triggered()
{
    if(ui->groupBox_4->isHidden()){
        ui->groupBox_4->show();
        ui->pushButton_12->show();
    }else{
        ui->groupBox_4->hide();
        ui->pushButton_12->hide();
    }

}

void MainWindow::on_pushButton_6_clicked()
{
    QString a = ui->lineEdit_2->text();//要查找的字符串
    QString b = ui->textEditMain->codeTextEdit->toPlainText();//代码文本
    QTextDocument *document = ui->textEditMain->codeTextEdit->document();
    QTextCharFormat format(cursors.charFormat());
    format.setForeground(Qt::white);
    format.setBackground(QColor(45, 44, 48));
    cursors.mergeCharFormat(format);
    int c = b.indexOf(a,FindFrom);//从findefrom这个位置开始查找
    if(c ==-1){
        if(FindFrom == 0) QMessageBox::warning(this,tr("警告"),tr("查找不到相关内容"),QMessageBox::Ok);
        else {
            FindFrom = 0;
            c = b.indexOf(a,FindFrom);
        }
    }
    FindFrom = c + 1;
    format.setForeground(Qt::black);//查找时的高亮
    format.setBackground(Qt::yellow);
    cursors = document->find(a,cursors,QTextDocument::FindWholeWords);
    cursors.mergeCharFormat(format);
    //cursors.setPosition(c, QTextCursor::MoveAnchor);
    //cursors.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, a.length());
    ui->textEditMain->codeTextEdit->setFocus();
    ui->textEditMain->codeTextEdit->setTextCursor(cursors);//将文本光标移动到想要的位置
}

void MainWindow::on_pushButton_7_clicked()
{
    QString a = ui->lineEdit_2->text();
    QString d = ui->lineEdit_3->text();
    QString b = ui->textEditMain->codeTextEdit->toPlainText();
    QTextCharFormat format(cursors.charFormat());
    format.setForeground(Qt::white);
    format.setBackground(QColor(45, 44, 48));
    cursors.mergeCharFormat(format);
    b = b.replace(FindFrom - 1,a.length(), d);
    ui->textEditMain->codeTextEdit->clear();
    ui->textEditMain->codeTextEdit->setText(b);
    //QTextCursor cursors = ui->textEditMain->codeTextEdit->textCursor();
    cursors.setPosition(FindFrom , QTextCursor::MoveAnchor);
    ui->textEditMain->codeTextEdit->setTextCursor(cursors);//将文本光标移动到想要的位置
}




void MainWindow::on_actionOutput_triggered()
{
    if(ui->tabWidget_2->isHidden()){
     ui->tabWidget_2->show();
    }else{
    ui->tabWidget_2->hide();
    }

}

void MainWindow::on_actionFile_triggered()
{
    if(ui->groupBox->isHidden()){
        ui->groupBox->show();
        ui->groupBox_5->show();
        ui->tabWidget_3->show();
        ui->tabWidget->show();
        ui->projectName->show();
    }else{
        ui->groupBox->hide();
        ui->groupBox_5->hide();
        ui->tabWidget_3->hide();
        ui->tabWidget->hide();
        ui->projectName->hide();
    }
}

void MainWindow::on_actionStatus_triggered()
{
    if(ui->toolBar->isHidden()){
       ui->toolBar->show();
    }else{
        ui->toolBar->hide();
    }
}
void MainWindow::on_actionEditor_triggered()
{

}

void MainWindow::on_actionComplierAndRun_triggered()
{
    Complier();
    if(Compliered)
        Run();
}

void MainWindow::on_actionStop_triggered()
{
    Complier();
}



void MainWindow::on_pushButtonClear_clicked()
{
    ui->textBrowser_output->clear();
}

void MainWindow::on_actionExit_2_triggered()
{
    QApplication* app;
    app->quit();

}

void MainWindow::on_action_triggered()
{
    this->setStyleSheet("background:rgb(45, 44, 48);color:rgb(255, 255, 255);margin:0;padding:0;Spacing:0;");
    ui->pushButton_12->setStyleSheet("border:none;background:rgb(51, 51, 51);margin:0;padding:1px;");
    ui->projectName->setStyleSheet("border:none;background:rgb(51, 51, 51);margin:0;padding:1px;");
}

void MainWindow::on_action_2_triggered()
{
    this->setStyleSheet("background:rgb(252, 255, 235);color:rgb(42, 42, 42);margin:0;padding:0;Spacing:0;");
    ui->pushButton_12->setStyleSheet("border:none;background:rgb(51, 51, 51);color:rgb(255, 255, 255);margin:0;padding:1px;");
    ui->projectName->setStyleSheet("border:none;background:rgb(51, 51, 51);color:rgb(255, 255, 255);margin:0;padding:1px;");
}
