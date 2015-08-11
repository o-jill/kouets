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

    /// INI�t�@�C���̃p�X��ݒ肷��
    void SetIniPath(const QString &path) {iniPath_ = path;}

    QString GetIniPath() {return iniPath_;}  //!< @return INI�t�@�C���̃p�X
    QString GetAppDataPath() {return appDataPath_;}  //!< @return %APPDATA%�̃p�X
    QString GetTempPath() {return tempPath_;}  //!< @return %TEMP%�̃p�X

private:
    void prepareAppDataPath();
    static void myMessageHandler(QtMsgType type, const char *msg);

protected:
    //bool winEventFilter(MSG *message, long *result);

private:
    QString iniPath_;
    QString logPath_;
    QString appDataPath_;
    QString tempPath_;
};


#endif  // __KOUETSAPP_H__
