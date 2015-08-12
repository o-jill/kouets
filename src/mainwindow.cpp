#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>


#include "kouetsapp.h"
#include "projectfile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), pte_(NULL), process_(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    KouetsApp*app = reinterpret_cast<KouetsApp*>(qApp);

    ui->lineEdit_pathprogram->setText(app->GetProgramPath());
    ui->lineEdit_cmdline->setText(app->GetCmdLine());

    process_ = new QProcess(this);
    connect(process_, SIGNAL(finished(int)), this, SLOT(onProcessFinished(int)));

    // QStringList column;
    // column << fname << status << errors;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// select program file
void MainWindow::on_toolButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "choose a program");

    if (path.length() != 0) {
        ui->lineEdit_pathprogram->setText(path);
        KouetsApp*app = reinterpret_cast<KouetsApp*>(qApp);
        app->SetProgramPath(path);
        app->SaveIni();
    }
}

void MainWindow::on_lineEdit_cmdline_textChanged(const QString &arg1)
{
    KouetsApp*app = reinterpret_cast<KouetsApp*>(qApp);
    app->SetCmdLine(arg1);
    app->SaveIni();
}

void MainWindow::on_actionOpen_triggered()
{
    //
    QString path = QFileDialog::getOpenFileName(this, "choose a file");

    if (path.length() == 0)
        return;
#if 0
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, path);
    item->setText(1, "ready");
    item->setText(2, "not yet");
    ui->treeWidget->insertTopLevelItem(0, item);
#endif
#if 1
    KouetsApp *app = reinterpret_cast<KouetsApp*>(qApp);

    QString cmdline = app->GetCmdLine();
    cmdline += " ";
    cmdline += path;

    qDebug() << "program:" << app->GetProgramPath();
    qDebug() << "path:" << cmdline;

    process_->setNativeArguments(cmdline);
    process_->start(app->GetProgramPath());

    QFileInfo fi(path);
    QString tabname = fi.fileName();
    int count = ui->tabWidget->count();
    int idx;
    for (idx = 0 ; idx < count ; ++idx) {
        QString s = ui->tabWidget->tabText(idx);
        if(s.compare(tabname, Qt::CaseInsensitive) == 0)
            break;
    }
    if (idx == count) {
        pte_ = new QTextEdit(this);
        ui->tabWidget->addTab(pte_, tabname);
    } else {
        pte_ = reinterpret_cast<QTextEdit*>(ui->tabWidget->widget(idx));
    }
    ui->tabWidget->setCurrentWidget(pte_);
#endif
}

void MainWindow::onProcessFinished(int code)
{
    QString output = process_->readAllStandardError();
    QTextStream ts(&output);
    QString result;
    for (;;) {
        QString line = ts.readLine();
        if (line.length() == 0)
            break;
        qDebug() << line;
        QRegExp reg1("(.+)\\(([0-9]+)\\):(.+) (\\[.+\\]) (\\[[0-9]+\\])");
        QRegExp reg2("(\\D+): (\\d+)");
        if (reg1.indexIn(line) >= 0) {
            result += "<B>" + reg1.cap(1) + "</B>";
            result += "(<FONT COLOR='RED'>" + reg1.cap(2) + "</FONT>):";
            result += "<STRONG>"+reg1.cap(3)+"</STRONG> ";
            result += "<FONT COLOR='BLUE'>" + reg1.cap(4) + "</FONT> ";
            result += "<FONT COLOR='GREEN'>" + reg1.cap(5) + "</FONT>";
            result += "<BR>";
        } else if (reg2.indexIn(line) >= 0) {
            result += reg2.cap(1);
            result += ": <FONT COLOR='RED'>" + reg2.cap(2) + "</FONT>";
        } else {
            result += line;
            result += "<BR>";
        }
    }
    pte_->setText(result);

    // qDebug() << result;
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QString str = ui->tabWidget->tabText(index);
    if (str.startsWith(":"))
        return;
    QTextEdit*pte = reinterpret_cast<QTextEdit*>(ui->tabWidget->widget(index));
    ui->tabWidget->removeTab(index);
    delete pte;
}
