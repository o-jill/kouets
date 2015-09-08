#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "projectfile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *e);

private slots:
    void on_toolButton_clicked();
    void on_lineEdit_cmdline_textChanged(const QString &arg1);
    void on_actionOpen_triggered();
    void onReadyReadStdOut();
    void onReadyReadStdErr();
    void onProcessFinished(int code);
    void on_tabWidget_tabCloseRequested(int index);
    void on_actionAdd_triggered();
    void onTimerUpdate();
    void on_actionSave_triggered();
    void on_checkBox_ActivateProcessedTab_clicked(bool checked);
    void on_checkBox_LineWrap_clicked(bool checked);

    void on_actionRun_triggered();

    void on_actionPause_triggered();

private:
    int OpenProjectFile(const QString &path);
    void SwitchTimer(int bon);

private:
    Ui::MainWindow *ui;
    QTimer *ptimer_update_;
    int brunning_;
    bool initated_;
    QProcess *process_;
    QTextEdit *pte_;
    QTreeWidgetItem *pitem_;
    ProjectFile prj_;
    int curfile_;
    QString result_;
    QString remainingtext_;
    int nerrors_;
};

#endif // MAINWINDOW_H
