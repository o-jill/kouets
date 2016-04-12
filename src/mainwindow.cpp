#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>

#ifdef WINDOWS
#include <windows.h>
#endif

#include "kouetsapp.h"
#include "projectfile.h"
#include "kouetshash.h"
#include "decorate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), nrunning_(RUN_INIT),
    process_(NULL), pte_(NULL), pitem_(NULL), curfile_(0), nerrors_(-1),
    initfinished_(false)
{
    ui->setupUi(this);

    KouetsApp*app = reinterpret_cast<KouetsApp*>(qApp);

    ui->lineEdit_pathprogram->setText(app->GetProgramPath());
    ui->lineEdit_cmdline->setText(app->GetCmdLine());
    ui->checkBox_ActivateProcessedTab->setChecked(
            app->IsActivateProcessedTab());
    ui->checkBox_LineWrap->setChecked(app->LineWrap());

    ptimer_update_ = new QTimer(this);
    ptimer_update_->setInterval(1000);
    connect(ptimer_update_, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));

    process_ = new QProcess(this);
    connect(process_, SIGNAL(finished(int)),
            this, SLOT(onProcessFinished(int)));
    connect(process_, SIGNAL(readyReadStandardOutput()),
            this, SLOT(onReadyReadStdOut()));
    connect(process_, SIGNAL(readyReadStandardError()),
            this, SLOT(onReadyReadStdErr()));
    connect(process_, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(onProcessError(QProcess::ProcessError)));
    process_->setProcessChannelMode(QProcess::MergedChannels);

    pprgs_ = new QProgressBar(ui->statusBar);
    ui->statusBar->addPermanentWidget(pprgs_);
    pprgs_->setRange(0, 1);
    pprgs_->setAlignment(Qt::AlignCenter);
    pprgs_->setFormat("%v / %m");
    pprgs_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QFont f = pprgs_->font();
    f.setPointSize(f.pointSize()/2);
    pprgs_->setFont(f);

    for (int i = 0 ; i < decomgr_.Size() ; ++i) {
        ui->comboBox->addItem(decomgr_.Name(i));
    }

    QString deconame = app->GetDecoration();
    if (deconame.length() != 0) {
        int idx = ui->comboBox->findText(deconame, Qt::MatchFixedString);
        ui->comboBox->setCurrentIndex(idx);
    }

    SetWindowTitle("");

    QString path = app->FileName2Open();
    if (path.length() != 0) {
        QFileInfo fi(path);
        // if project file
        if (fi.suffix() == "kouets") {
            OpenProjectFile(fi.absoluteFilePath());
        }
    }

    initfinished_ = true;
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
    if (ptimer_update_->isActive())
        ptimer_update_->stop();
    //
    QString path = QFileDialog::getOpenFileName(this, "choose a fileto open");

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
        QString s = prj_.atFilename(i);
        item->setText(TREE_COLUMN_NAME, s);
        item->setText(TREE_COLUMN_STATE, "ready");
        item->setText(TREE_COLUMN_ERROR, "not yet");
        item->setText(TREE_COLUMN_PATH, prj_.atPath(i));
        ui->treeWidget->addTopLevelItem(item);
    }
    curfile_ = 0;
    UpdateProgressBarRangeMax();
    UpdateProgressBarPos();
    SetWindowTitle(path);
    SwitchTimer(TRUE);

    return 1;
}

void MainWindow::onReadyReadStdOut()
{
    if (pte_ == NULL)
        return;

    QString output = process_->readAllStandardOutput();

    if (output.length() <= 0) {
    } else {
        QStringList sl = output.split("\n");
        sl[0] = remainingtext_+sl[0];
        remainingtext_ = sl[sl.size()-1];
        if (pdeco_ == NULL) {
            DecorateNone dec;
            result_ += dec.Decorate(&sl);
            if (nerrors_ < 0)
                nerrors_ = dec.ErrorNum();
        } else {
            result_ += pdeco_->Decorate(&sl);
            if (nerrors_ < 0)
                nerrors_ = pdeco_->ErrorNum();
        }
        pte_->setHtml(result_);

        // qDebug() << result_;
    }
    // qDebug() << output;
}

void MainWindow::onReadyReadStdErr()
{
    // qDebug() << output;
}

void MainWindow::onProcessFinished(int code)
{
    if (pte_ == NULL) {
        UpdateProgressBarRangeMax();
        return;
    }

    QString output = remainingtext_ + process_->readAllStandardError();

    if (output.length() <= 0) {
        // nerrors = 0;
    } else {
        DecorateGCppVs7 dec;
        QStringList sl = output.split("\n");
        result_ += dec.Decorate(&sl);
        if (nerrors_ < 0)
            nerrors_ = dec.ErrorNum();

        pte_->setHtml(result_);

        // qDebug() << result_;

        result_.clear();
    }

    UpdateProgressBarRangeMax();
    UpdateProgressBarPos();
    ++curfile_;
    if (curfile_ == prj_.size()) {
        curfile_ = 0;
        if (IsRunOnce()) {
            SwitchTimer(FALSE);
        }
    }

    if (pitem_) {
        pitem_->setText(TREE_COLUMN_STATE, "done");
        if (nerrors_ < 0) {
            pitem_->setText(TREE_COLUMN_ERROR, "error...");
        } else {
            if (pitem_) {
                pitem_->setText(TREE_COLUMN_ERROR, QString("%1").arg(nerrors_));
            }
        }
    }

    if (IsRunable()) {
        ptimer_update_->start();
    }
}

void MainWindow::onProcessError(QProcess::ProcessError err)
{
    qDebug() << "onProcessError:" << err << process_->errorString();
#ifdef WINDOWS
    qDebug() << "GetLastErrorError:" << GetLastError();
#endif
    ptimer_update_->stop();
    UpdateProgressBarRangeMax();

    if (pitem_) {
        pitem_->setText(TREE_COLUMN_STATE, "process error");
    }
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

// add files.
void MainWindow::on_actionAdd_triggered()
{
    if (IsRunning()) {
        ptimer_update_->stop();
    }
    //
    QStringList path =
                QFileDialog::getOpenFileNames(this, "choose files to add");

    if (path.length() == 0) {
    } else {
        QStringList::iterator it;
        for (it = path.begin() ; it != path.end() ; ++it) {
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(TREE_COLUMN_NAME, *it);
            item->setText(TREE_COLUMN_STATE, "ready");
            item->setText(TREE_COLUMN_ERROR, "not yet");
            item->setText(TREE_COLUMN_PATH, *it);
            ui->treeWidget->addTopLevelItem(item);
            prj_.Add(*it);
        }
        UpdateProgressBarRangeMax();
    }
    if (IsRunable()) {
        ptimer_update_->start();
    }
}

void MainWindow::onTimerUpdate()
{
    ptimer_update_->stop();

    KouetsApp *app = reinterpret_cast<KouetsApp*>(qApp);

    UpdateProgressBarPos();
    if (!prj_.isUpdated(curfile_)) {
        ++curfile_;
        if (curfile_ == prj_.size()) {
            curfile_ = 0;
        }
        if (IsRunOnce() && curfile_ == 0) {
            SwitchTimer(FALSE);
        } else {
            ptimer_update_->start();
        }
        return;
    }
    SetProgressBarMarquee();
    QString path = prj_.atPath(curfile_);
    QString fname = prj_.atFilename(curfile_);
    QString apppath;
    QString cmdline;
    QString deconame;

    if (prj_.isUseDefaultAppPath()) {
        apppath = "\"" + app->GetProgramPath() + "\"";
    } else {
        apppath = "\"" + prj_.AppPath() + "\"";
    }

    if (prj_.isUseDefaultCmdLine()) {
        cmdline = app->GetCmdLine();
    } else {
        cmdline = prj_.CmdLine();
    }
    cmdline += " \"";
    cmdline += path;
    cmdline += "\"";

    if (prj_.isUseDefaultParser()) {
        deconame = app->GetDecoration();
    } else {
        deconame = prj_.Parser();
    }

    // qDebug() << "program:" << apppath;
    // qDebug() << "cmdline:" << cmdline;
#ifdef WINDOWS
    process_->setNativeArguments(cmdline);
#else
    QStringList args = cmdline.split(" ");
#endif
//    process_->start(apppath);

    QFileInfo fi(path);

    int count = ui->treeWidget->topLevelItemCount();
    pitem_ = NULL;
    for (int idx = 0 ; idx < count ; ++idx) {
        QTreeWidgetItem *pi = ui->treeWidget->topLevelItem(idx);
        QString path = pi->text(TREE_COLUMN_PATH);
        path.replace(QChar('\\'), QChar('/'));
        if (path == fi.absoluteFilePath()) {
            pitem_ = pi;
            pi->setText(TREE_COLUMN_STATE, "running");
            break;
        }
    }

    if (pitem_ == NULL) {
        return;
    }

    if (!fi.exists()) {
        // the file does not exist.
        pitem_->setText(TREE_COLUMN_STATE, "not exists");
        pitem_->setText(TREE_COLUMN_UPDATED, "yyyy/MM/dd hh:mm:ss");
        if (IsRunOnce() && curfile_ == 0) {
            SwitchTimer(FALSE);
        } else {
            ptimer_update_->start();
        }
        return;
    }

    QString tabname = fname;
    int idx = FindTab(fname);
    if (idx < 0) {
        pte_ = new QTextEdit(this);
        pte_->setReadOnly(true);
        if (app->LineWrap()) {
        } else {
            pte_->setWordWrapMode(QTextOption::NoWrap);
        }
        ui->tabWidget->addTab(pte_, tabname);
    } else {
        pte_ = reinterpret_cast<QTextEdit*>(ui->tabWidget->widget(idx));
    }
    pte_->clear();
    if (app->IsActivateProcessedTab()) {
        ui->tabWidget->setCurrentWidget(pte_);
    }
    pitem_->setText(TREE_COLUMN_UPDATED,
                prj_.lastUpdated(curfile_).toString("yyyy/MM/dd hh:mm:ss"));

    pdeco_ = decomgr_.find(deconame);
    nerrors_ = -1;
    result_.clear();
#ifdef WINDOWS
    process_->start(apppath);
#else
    process_->start(apppath, args);

#endif
}

void MainWindow::on_actionSave_triggered()
{
    if (prj_.size() == 0)
        return;

    int timeron = ptimer_update_->isActive();
    if (timeron) {
        ptimer_update_->stop();
    }

    QString path = QFileDialog::getSaveFileName(
                this, "choose a file", QString(),
                "kouets(*.kouets);;All files(*.*)");

    if (path.length() == 0) {
    } else {
        if (prj_.size() > 0) {
            prj_.Save(path);
            SetWindowTitle(path);
        }
    }

    if (IsRunable()) {
        ptimer_update_->start();
    }
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
            OpenProjectFile(flist.at(0).toLocalFile());
        } else {
            // add a dropped file.
            for (it = flist.begin() ; it != flist.end() ; ++it) {
                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(TREE_COLUMN_NAME, it->toLocalFile());
                item->setText(TREE_COLUMN_STATE, "ready");
                item->setText(TREE_COLUMN_ERROR, "not yet");
                item->setText(TREE_COLUMN_PATH, it->toLocalFile());
                ui->treeWidget->addTopLevelItem(item);
                prj_.Add(it->toLocalFile());
            }
            UpdateProgressBarRangeMax();
            if (IsRunable()) {
                SwitchTimer(TRUE);
            }
        }
    } else {
        // add dropped files.
        for (it = flist.begin() ; it != flist.end() ; ++it) {
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(TREE_COLUMN_NAME, it->toLocalFile());
            item->setText(TREE_COLUMN_STATE, "ready");
            item->setText(TREE_COLUMN_ERROR, "not yet");
            item->setText(TREE_COLUMN_PATH, it->toLocalFile());
            ui->treeWidget->addTopLevelItem(item);
            prj_.Add(it->toLocalFile());
        }
        UpdateProgressBarRangeMax();
        if (IsRunable()) {
            SwitchTimer(TRUE);
        }
    }
}

void MainWindow::on_checkBox_ActivateProcessedTab_clicked(bool checked)
{
    KouetsApp *app = reinterpret_cast<KouetsApp*>(qApp);
    app->SetActivateProcessedTab(checked);
    app->SaveIni();
}

void MainWindow::on_checkBox_LineWrap_clicked(bool checked)
{
    KouetsApp *app = reinterpret_cast<KouetsApp*>(qApp);
    app->SetLineWrap(checked);
    app->SaveIni();
}

void MainWindow::SwitchTimer(int bon)
{
    ui->actionRun->setEnabled(bon == 0);
    ui->actionPause->setEnabled(bon != 0);
    nrunning_ = (bon != 0) ? RUN_RUNNING : RUN_STOP;
    if (bon != 0) {
        ptimer_update_->start();
    } else {
        ptimer_update_->stop();
    }
}

void MainWindow::on_actionRun_triggered()
{
    SwitchTimer(TRUE);
}

void  MainWindow::on_actionRunOnce_triggered()
{
    if (nrunning_ == RUN_RUNONCE)
        return;

    ui->actionRun->setEnabled(true);
    ui->actionPause->setEnabled(true);
    nrunning_ = RUN_RUNONCE;
    curfile_ = 0;
    ptimer_update_->start();
}

void MainWindow::on_actionPause_triggered()
{
    SwitchTimer(FALSE);
    UpdateProgressBarRangeMax();
}

void  MainWindow::on_actionLog_triggered()
{
    if (prj_.size() == 0 || ui->tabWidget->count() <= 2)
        return;

    int timeron = ptimer_update_->isActive();
    if (timeron) {
        ptimer_update_->stop();
    }

    int curtab = ui->tabWidget->currentIndex();
    QString tabname = ui->tabWidget->tabText(curtab);
    if (tabname.startsWith(QChar(':'))) {
        if (IsRunable()) {
            ptimer_update_->start();
        }
        return;
    }

    QDateTime now = QDateTime::currentDateTime();
    QString path = QString("kouets%1%2%3%4%5%6")
            .arg(now.date().year(), 4, 10, QChar('0'))
            .arg(now.date().month(), 2, 10, QChar('0'))
            .arg(now.date().day(), 2, 10, QChar('0'))
            .arg(now.time().hour(), 2, 10, QChar('0'))
            .arg(now.time().minute(), 2, 10, QChar('0'))
            .arg(now.time().second(), 2, 10, QChar('0'));

    path = QFileDialog::getSaveFileName(
                this, "choose a file to save", path,
                "plain text log(*.txt);;html log(*.html);;All files(*.*)");

    if (path.length() == 0) {
    } else {
        curtab = ui->tabWidget->currentIndex();
        tabname = ui->tabWidget->tabText(curtab);
        QTextEdit*pte =
            reinterpret_cast<QTextEdit*>(ui->tabWidget->currentWidget());
        if (pte) {
            QFile file(path);
            if (file.open(QIODevice::Text|QIODevice::WriteOnly)) {
                QFileInfo fi(path);
                QString buf;
                if (fi.suffix().compare("html", Qt::CaseInsensitive) == 0) {
                    buf = pte->toHtml();
                } else {
                    buf = pte->toPlainText();
                }
                file.write(buf.toUtf8());
            }
        }
    }

    if (IsRunable()) {
        ptimer_update_->start();
    }
}

void MainWindow::SetProgressBarPos(int pos)
{
    pprgs_->setValue(pos);
}

/**
 * @brief MainWindow::SetProgressBarRangeMax
 * @param max 0:marquee, other:maximum
 */
void MainWindow::SetProgressBarRangeMax(int max)
{
    pprgs_->setRange(0, max);
}

void MainWindow::UpdateProgressBarPos()
{
    pprgs_->setValue(curfile_);
}

void MainWindow::UpdateProgressBarRangeMax()
{
    if (prj_.size() <= 0)
        pprgs_->setRange(0, 1);
    else
        pprgs_->setRange(0, prj_.size());
}

void MainWindow::SetProgressBarMarquee()
{
    pprgs_->setRange(0, 0);
}

/**
 * [MainWindow::SetWindowTitle description]
 * @param prjfname project file name.
 */
void MainWindow::SetWindowTitle(const QString &prjfname)
{
    QString str = QString("Kouets [%1 %2]").arg(branchname).arg(commithash);
    if (prjfname.length() > 0) {
        str = prjfname + " - " + str;
    }
    setWindowTitle(str);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (IsRunning()) {
        ptimer_update_->stop();
    }
    if (process_) {
        if (process_->state() == QProcess::Running) {
            process_->terminate();
        }
    }
}

void MainWindow::on_treeWidget_itemDoubleClicked(
        QTreeWidgetItem *item, int column)
{
    // activate tab
    QString path = item->text(0);
    int idx = FindTab(path);
    if (idx > 0) {
        ui->tabWidget->setCurrentIndex(idx);
    } else {
        prj_.resetUpdated(path);
        item->setText(TREE_COLUMN_STATE, tr("will be checked"));
        item->setText(TREE_COLUMN_UPDATED, "-/-/- -:-:-");
    }
}

int MainWindow::FindTab(const QString& path)
{
    //
    QString s = path;
    int count = ui->tabWidget->count();
    int idx;
    for (idx = 0 ; idx < count ; ++idx) {
        QString tabname = ui->tabWidget->tabText(idx);
        if (s.compare(tabname, Qt::CaseInsensitive) == 0)
            break;
    }
    if (idx == count) {
        return -1;
    } else {
        return idx;
    }
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (initfinished_ == false)
        return;

    KouetsApp*app = reinterpret_cast<KouetsApp*>(qApp);
    app->SetDecoration(arg1);
    app->SaveIni();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == 'W' && e->modifiers() == Qt::ControlModifier) {  // Ctrl-W
        int idxtab = ui->tabWidget->currentIndex();
        QString str = ui->tabWidget->tabText(idxtab);
        if (str.startsWith(":"))
            return;
        ui->tabWidget->removeTab(idxtab);
    }
}
