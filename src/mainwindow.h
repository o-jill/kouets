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
    void onProcessFinished(int code);
    void on_tabWidget_tabCloseRequested(int index);
    void on_actionAdd_triggered();
    void onTimerUpdate();
    void on_actionSave_triggered();

    void on_checkBox_ActivateProcessedTab_clicked(bool checked);

private:
    int OpenProjectFile(const QString &path);

private:
    Ui::MainWindow *ui;
    QTimer *ptimer_update_;
    bool initated_;
    QProcess *process_;
    QTextEdit *pte_;
    QTreeWidgetItem *pitem_;
    ProjectFile prj_;
    int curfile_;
};

#endif // MAINWINDOW_H
