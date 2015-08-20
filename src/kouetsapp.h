/**
 * @file kouetsapp.h
 * @brief �����ɐ���������
 *
 */
#ifndef __KOUETSAPP_H__
#define __KOUETSAPP_H__

#include <QApplication>
#include <QString>

#include <windows.h>

class KouetsApp : public QApplication
{
public:
    KouetsApp(int &argc, char**argv);
    ~KouetsApp();

    int LoadIni();
    int SaveIni();

    int CheckRunning();

    // setter

    /// INI�t�@�C���̃p�X��ݒ肷��
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

    QString GetIniPath() {return iniPath_;}  //!< @return INI�t�@�C���̃p�X
    QString GetAppDataPath() {return appDataPath_;}  //!< @return %APPDATA%�̃p�X
    QString GetTempPath() {return tempPath_;}  //!< @return %TEMP%�̃p�X

    QString GetProgramPath() {return programPath_;}
    QString GetCmdLine() {return cmdLine_;}
    int IsActivateProcessedTab() {return bactivateprocessedtab_;}
    int LineWrap() {return blinewrap_;}

    QString FileName2Open() {return filename2Open_;}
private:
    void prepareAppDataPath();
    static void myMessageHandler(QtMsgType type, const char *msg);

protected:
    // bool winEventFilter(MSG *message, long *result);

private:
    QString iniPath_;
    QString logPath_;
    QString appDataPath_;
    QString tempPath_;

    QString programPath_;
    QString cmdLine_;
    int bactivateprocessedtab_;
    int blinewrap_;
    int updated_;

    QString filename2Open_;
};


#endif  // __KOUETSAPP_H__
