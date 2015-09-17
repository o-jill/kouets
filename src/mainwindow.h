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
    enum {
        TREE_COLUMN_PATH = 0,
        TREE_COLUMN_STATE,
        TREE_COLUMN_ERROR,
        TREE_COLUMN_UPDATED,
        TREE_COLUMN_MAX
        // TREE_COLUMN_,
    };
    enum {
        RUN_INIT = -1,
        RUN_STOP = 0,
        RUN_RUNNING = 1,
        RUN_RUNONCE = 2,
        RUN_MAX
    };
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool IsRunning() {
        return (nrunning_ == RUN_RUNNING || nrunning_ == RUN_RUNONCE);
    }
    bool IsRunable() {
        return ((prj_.size() > 0)
                && (nrunning_ == RUN_RUNNING || nrunning_ == RUN_INIT
                    || nrunning_ == RUN_RUNONCE));
    }
    bool IsRunOnce() {return nrunning_ == RUN_RUNONCE;}
protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void closeEvent(QCloseEvent *e);

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
    void on_actionRunOnce_triggered();
    void on_actionPause_triggered();
    void on_actionLog_triggered();
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    int OpenProjectFile(const QString &path);
    void SwitchTimer(int bon);
    void SetProgressBarPos(int pos);
    void SetProgressBarRangeMax(int max);
    void UpdateProgressBarPos();
    void UpdateProgressBarRangeMax();
    void SetProgressBarMarquee();
    void SetWindowTitle(const QString& str);
    int FindTab(const QString& str);

private:
    Ui::MainWindow *ui;
    QTimer *ptimer_update_;
    int nrunning_;
    QProgressBar *pprgs_;
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

#endif  // MAINWINDOW_H
