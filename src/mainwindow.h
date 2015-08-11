#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButton_clicked();
    void on_lineEdit_cmdline_textChanged(const QString &arg1);
    void on_actionOpen_triggered();
    void onProcessFinished(int code);

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    QTimer *ptimer_update_;
    bool initated_;
    QProcess *process_;
    QTextEdit *pte_;
};

#endif // MAINWINDOW_H
