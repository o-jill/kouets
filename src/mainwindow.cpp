#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>


#include "kouetsapp.h"
#include "projectfile.h"
#include "kouetshash.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), pte_(NULL), process_(NULL),
    curfile_(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    KouetsApp*app = reinterpret_cast<KouetsApp*>(qApp);

    ui->lineEdit_pathprogram->setText(app->GetProgramPath());
    ui->lineEdit_cmdline->setText(app->GetCmdLine());

    ptimer_update_ = new QTimer(this);
    ptimer_update_->setInterval(1000);
    connect(ptimer_update_, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));

    process_ = new QProcess(this);
    connect(process_, SIGNAL(finished(int)),
            this, SLOT(onProcessFinished(int)));

    setWindowTitle(QString("Kouets [%1 %2]").arg(branchname).arg(commithash));
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

    OpenProjectFile(path);
}

int MainWindow::OpenProjectFile(const QString &path)
{
    if (prj_.Open(path) <= 0) {
        return 0;
    }

    ui->treeWidget->clear();

    pte_ = NULL;
    int count = ui->tabWidget->count();
    for (int idx = count ; idx ; ) {
        --idx;
        QString str = ui->tabWidget->tabText(idx);
        if (str.startsWith(":"))
            continue;
        QTextEdit*pte =
                reinterpret_cast<QTextEdit*>(ui->tabWidget->widget(idx));
        ui->tabWidget->removeTab(idx);
        delete pte;
    }

    for (int i = 0 ; i < prj_.size() ; ++i) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        QString s = prj_.at(i);
        item->setText(0, s);
        item->setText(1, "ready");
        item->setText(2, "not yet");
        ui->treeWidget->addTopLevelItem(item);
    }
    curfile_ = 0;

    ptimer_update_->start();
    return 1;
}

void MainWindow::onProcessFinished(int code)
{
    if (pte_ == NULL)
        return;

    QString output = process_->readAllStandardError();
    QTextStream ts(&output);
    QString result;
    int nerrors = -1;
    for (;;) {
        QString line = ts.readLine();
        if (line.length() == 0)
            break;
        result += Decorate(line, nerrors);
    }
    pte_->setText(result);
    ++curfile_;
    if (curfile_ == prj_.size())
        curfile_ = 0;

    // qDebug() << result;

    if (pitem_) {
        pitem_->setText(1, "done");
        if (nerrors < 0) {
            pitem_->setText(2, "error...");
        } else {
            if (pitem_) {
                pitem_->setText(2, QString("%1").arg(nerrors));
            }
        }
    }

    ptimer_update_->start();
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

// ファイルの追加。
void MainWindow::on_actionAdd_triggered()
{
    //
    QStringList path = QFileDialog::getOpenFileNames(this, "choose a file");

    if (path.length() == 0)
        return;

    for (QStringList::iterator it = path.begin() ; it != path.end() ; ++it) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, *it);
        item->setText(1, "ready");
        item->setText(2, "not yet");
        ui->treeWidget->addTopLevelItem(item);
        prj_.Add(*it);
    }
    if (ptimer_update_->isActive()) {
        //
    } else {
        ptimer_update_->start();
    }
}

/**
 * @brief Decorate text from google C++ lint program(cpplint.py)
 * @param str input text line.
 * @param nerr number of errors, if str contains the number. otherwise not changed.
 * @return decorated string
 */
QString MainWindow::Decorate(QString &str, int &nerr)
{
    QString result;
    QRegExp reg1("(.+)\\(([0-9]+)\\):(.+) (\\[.+\\]) (\\[[0-9]+\\])");
    QRegExp reg2("(\\D+): (\\d+)");

    str.replace("<", "&lt;");
    str.replace(">", "&gt;");
    // str.replace("&", "&amp;");  // it may not be needed...

    if (reg1.indexIn(str) >= 0) {
        result += "<B>" + reg1.cap(1) + "</B>";
        result += "(<FONT COLOR='RED'>" + reg1.cap(2) + "</FONT>):";
        result += "<STRONG>"+reg1.cap(3)+"</STRONG> ";
        result += "<FONT COLOR='BLUE'>" + reg1.cap(4) + "</FONT> ";
        result += "<FONT COLOR='GREEN'>" + reg1.cap(5) + "</FONT>";
        result += "<BR>";
    } else if (reg2.indexIn(str) >= 0) {
        result += reg2.cap(1);
        result += ": <FONT COLOR='RED'>" + reg2.cap(2) + "</FONT>";
        nerr = reg2.cap(2).toInt();
    } else {
        result += str;
        result += "<BR>";
    }
#if 0
    qDebug() << str;
    qDebug() << result;
#endif
    return result;
}

void MainWindow::onTimerUpdate()
{
    ptimer_update_->stop();

    KouetsApp *app = reinterpret_cast<KouetsApp*>(qApp);

    if (!prj_.isUpdated(curfile_)) {
        ++curfile_;
        if (curfile_ == prj_.size())
            curfile_ = 0;
        ptimer_update_->start();
        return;
    }
    QString path = prj_.at(curfile_);

    QString cmdline = app->GetCmdLine();
    cmdline += " ";
    cmdline += path;

    // qDebug() << "program:" << app->GetProgramPath();
    // qDebug() << "path:" << cmdline;

    process_->setNativeArguments(cmdline);
    process_->start(app->GetProgramPath());

    QFileInfo fi(path);
    QString tabname = fi.fileName();
    int count = ui->tabWidget->count();
    int idx;
    for (idx = 0 ; idx < count ; ++idx) {
        QString s = ui->tabWidget->tabText(idx);
        if (s.compare(tabname, Qt::CaseInsensitive) == 0)
            break;
    }
    if (idx == count) {
        pte_ = new QTextEdit(this);
        pte_->setReadOnly(true);
        ui->tabWidget->addTab(pte_, tabname);
    } else {
        pte_ = reinterpret_cast<QTextEdit*>(ui->tabWidget->widget(idx));
    }
    ui->tabWidget->setCurrentWidget(pte_);
    count = ui->treeWidget->topLevelItemCount();
    pitem_ = NULL;
    for (idx = 0 ; idx < count ; ++idx) {
        QTreeWidgetItem *pi = ui->treeWidget->topLevelItem(idx);
        if (pi->text(0) == fi.absoluteFilePath()) {
            pitem_ = pi;
            pi->setText(1, "running");
            break;
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (ptimer_update_->isActive()) {
        ptimer_update_->stop();
    }

    QString path = QFileDialog::getSaveFileName(
                this, "choose a file", QString(),
                "kouets(*.kouets);;All files(*.*)");

    if (path.length() == 0)
        return;

    if (prj_.size() > 0)
        prj_.Save(path);

    ptimer_update_->start();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasFormat("text/uri-list")) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QList<QUrl> flist = e->mimeData()->urls();
    if (flist.size() == 0) {
        return;
    }

    QList<QUrl>::iterator it;
    if (flist.size() == 1) {
        QFileInfo fi(flist.at(0).toLocalFile());
        // if project file
        if (fi.suffix() == "kouets") {
            // a
            OpenProjectFile(flist.at(0).toLocalFile());
        } else {
            // add a dropped file.
            for (it = flist.begin() ; it != flist.end() ; ++it) {
                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, it->toLocalFile());
                item->setText(1, "ready");
                item->setText(2, "not yet");
                ui->treeWidget->addTopLevelItem(item);
                prj_.Add(it->toLocalFile());
            }
            if (ptimer_update_->isActive()) {
                //
            } else {
                ptimer_update_->start();
            }
        }
    } else {
        // add dropped files.
        for (it = flist.begin() ; it != flist.end() ; ++it) {
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(0, it->toLocalFile());
            item->setText(1, "ready");
            item->setText(2, "not yet");
            ui->treeWidget->addTopLevelItem(item);
            prj_.Add(it->toLocalFile());
        }
        if (ptimer_update_->isActive()) {
            //
        } else {
            ptimer_update_->start();
        }
    }
}
