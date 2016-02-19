/**
 * @file kouetsapp.cpp
 * @brief application class
 *
 */

#include "kouetsapp.h"

#ifdef _WINDOWS
#include <Windows.h>
#else
#include <iostream>
#endif

#include <QtCore>


/**
 * constructor
 */
KouetsApp::KouetsApp(int &argc, char**argv)
    :QApplication(argc, argv), updated_(0), bactivateprocessedtab_(1),
    blinewrap_(0)
{
    prepareAppDataPath();

    // CoInitialize(0);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    for (int i = 1 ; i < argc ; ++i) {
        int ret = ParseCmdLine(argv[i]);
    }
#ifdef _WINDOWS
    wchar_t temp[MAX_PATH] = L"";
    ExpandEnvironmentStringsW(L"%TEMP%\\", temp, _countof(temp));

    tempPath_ = QString::fromWCharArray(temp);
#else
    tempPath_ = "/tmp/";  // this should use environment valiable
#endif
    iniPath_ = appDataPath_+ "Kouets.ini";

    logPath_ = QString(qApp->applicationDirPath() + "/qdebug_%1.log").arg(
                             QDate::currentDate().toString(Qt::ISODate));

#ifdef _DEBUG
    qInstallMsgHandler(KouetsApp::myMessageHandler);
#endif
    qDebug() << "Launch on " << QDateTime::currentDateTime();
}

/**
 * destructor
 */
KouetsApp::~KouetsApp()
{
    // CoUninitialize();
}

/**
 * load ini file.
 *
 * @return zero if loaded.
 */
int KouetsApp::LoadIni()
{
    int ret = 0;

    QSettings stg(GetIniPath(), QSettings::IniFormat);

    QVariant var = stg.value("program");
    programPath_ = var.toString();
    var = stg.value("commandline");
    cmdLine_ = var.toString();
    var = stg.value("decoration");
    decoration_ = var.toString();
    var = stg.value("activatetab", 1);
    bactivateprocessedtab_ = var.toInt();
    var = stg.value("linewrap", 1);
    blinewrap_ = var.toInt();

    updated_ = false;

    return ret;
}

int KouetsApp::SaveIni()
{
    if (!updated_)
        return 0;

    int ret = 0;

    QSettings stg(GetIniPath(), QSettings::IniFormat);

    stg.setValue("program", programPath_);
    stg.setValue("commandline", cmdLine_);
    stg.setValue("decoration", decoration_);
    stg.setValue("activatetab", bactivateprocessedtab_);
    stg.setValue("linewrap", blinewrap_);

    updated_ = false;

    return ret;
}


/**
 * generate path where to store settings.
 *
 * @note %APPDATA%/Kouets/
 *       --> C:\Users\nob-aoki\AppData\Roaming[WinVista/7/8]
 *       --> C:\Documents and Settings\nob-aoki\Application Data[Win2k/XP]
 *
 * @note ~/.Kouets/    on Linux & MacOS
 */
void KouetsApp::prepareAppDataPath()
{
#ifdef _WINDOWS
    wchar_t appdatapath[0x1000];
    ExpandEnvironmentStringsW(L"%APPDATA%\\Kouets\\",
                              appdatapath, _countof(appdatapath));
    appDataPath_ = QString::fromWCharArray(appdatapath);
#else
    appDataPath_ = "~/.Kouets/";
#endif
}

#if 0
bool hatApp::winEventFilter(MSG *message, long *result)
{
    if (message->message == WM_DEVICECHANGE) {
        qDebug() << "WM_DEVICECHANGE:" << message->wParam;
    }

    return false;
}
#endif

/**
 * put text by qDebug() to a log file and debug-console.
 *
 * @param type message type.
 * @param msg  debug message.
 *
 * @note file name is "debug_YYYY-MM-DD.log".
 */
void KouetsApp::myMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        // txt = QString("Debug: %1").arg(msg);
        txt = msg;
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
    }

    // put to a log file.
    KouetsApp *pApp = reinterpret_cast<KouetsApp*>(qApp);
    if (pApp != NULL) {
        QString fn = QString(pApp->logPath_);
        QFile outFile(fn);
        outFile.open(QIODevice::Text|QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << endl;
    }

    // put to QtCreator.
#ifdef _WINDOWS
    OutputDebugStringA(msg);
    OutputDebugStringA("\n");
#else
    std::cerr << msg << std::endl;
#endif

    if (type == QtFatalMsg) {
        abort();
    }
}

int KouetsApp::ParseCmdLine(char *str)
{
    if (str[0] == '-' || str[0] == '/') {
        // a
    } else {
        filename2Open_ = str;
    }
    return 0;
}
