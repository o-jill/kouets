/**
 * @file kouetsapp.h
 * @brief application class
 *
 */
#ifndef __KOUETSAPP_H__
#define __KOUETSAPP_H__

#include <QApplication>
#include <QString>

#ifdef WINDOWS
#include <windows.h>
#endif

#define TRUE  true
#define FALSE false

class KouetsApp : public QApplication
{
public:
    KouetsApp(int &argc, char**argv);
    ~KouetsApp();

    int LoadIni();
    int SaveIni();

    int CheckRunning();

    // setter

    /// set ini file path
    void SetIniPath(const QString &path) {iniPath_ = path;}

    void SetProgramPath(QString path) {
        if (programPath_ == path)
            return;
        programPath_ = path;
        updated_ = true;
    }
    void SetCmdLine(QString cmdline) {
        if (cmdLine_ == cmdline)
            return;
        cmdLine_ = cmdline;
        updated_ = true;
    }
    void SetDecoration(QString deco) {
        if (decoration_ == deco)
            return;
        decoration_ = deco;
        updated_ = true;
    }
    void SetActivateProcessedTab(int b) {
        if (b == bactivateprocessedtab_)
            return;
        bactivateprocessedtab_ = b;
        updated_ = true;
    }
    void SetLineWrap(int b) {
        if (b == blinewrap_)
            return;
        blinewrap_ = b;
        updated_ = true;
    }

    int ParseCmdLine(char *str);

    // getter

    QString GetIniPath() {return iniPath_;}  //!< @return INI file path
    QString GetAppDataPath() {return appDataPath_;}  //!< @return %APPDATA% path
    QString GetTempPath() {return tempPath_;}  //!< @return %TEMP% path

    QString GetProgramPath() {return programPath_;}
    QString GetCmdLine() {return cmdLine_;}
    QString GetDecoration() {return decoration_;}
    int IsActivateProcessedTab() {return bactivateprocessedtab_;}
    int LineWrap() {return blinewrap_;}

    QString FileName2Open() {return filename2Open_;}
private:
    void prepareAppDataPath();
#if QT_VERSION >= 0x050000
    static void  myMessageHandler5(
        QtMsgType type, const QMessageLogContext &context, const QString &msg);
#else
    static void myMessageHandler4(QtMsgType type, const char *msg);
#endif

protected:
    // bool winEventFilter(MSG *message, long *result);

private:
    QString iniPath_;
    QString logPath_;
    QString appDataPath_;
    QString tempPath_;

    QString programPath_;
    QString cmdLine_;
    QString decoration_;
    int bactivateprocessedtab_;
    int blinewrap_;
    int updated_;

    QString filename2Open_;
};


#endif  // __KOUETSAPP_H__
