#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QKeyEvent>
#include <QFileSystemModel>
#include<QProcess>
#include <QTextCursor>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void File_Initial();
    void Multiple_Window();
    void Bracket_Match();
    void SetType_Auto();
    void keyPressEvent(QKeyEvent  *event);
    ~MainWindow();
    bool eventFilter(QObject *obj, QEvent *e);
    void findline();//根据鼠标点击位置确定行数
    void findoverline(int begin);//括号匹配计算结束行数
    void foldorunfold(int begin,int over,QString name);
    void Allunfold();
    void Refold();
    int fold[200][3];
    QStringList folded[200];
    int tempfolds[200];
    int FindFrom;//查找用
    void showProjectName();
    QProcess process;
    QTextCursor cursors;
private:
    Ui::MainWindow *ui;
    bool Creating, Opening, Compliered;
    QString Remain_File_Address;
    QString Remain_File_Content;
    QFileSystemModel *model;
    int Windows;
private slots:
    void CreateFile();
    void OpenFile();
    void SaveFile();
    void SaveAs();
    void ExitFile();//关闭当前文件（作为一个附属函数用）
    void Exit();//关闭窗口
    void OpenHelp();
    void Complier();
    void Run();
    void on_actioncha_triggered();
    void on_pushButton_5_clicked();
    void on_actionClass_triggered();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButtonFold_clicked();
    void on_actionOutput_triggered();
    void on_actionFile_triggered();
    void on_actionStatus_triggered();
    void on_actionEditor_triggered();
    void on_actionComplierAndRun_triggered();
    void on_actionStop_triggered();
//    void on_actionClassView_triggered();
    void on_pushButtonClear_clicked();
    void on_actionExit_2_triggered();
    void on_action_triggered();
    void on_action_2_triggered();
};

#endif // MAINWINDOW_H
