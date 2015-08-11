/** 
 * @file kouetsapp.cpp
 * @brief ここに説明を書く
 * 
 */

#include "kouetsapp.h"

#include <QtCore>

#include <Windows.h>



/**
 * コンストラクタ
 */
KouetsApp::KouetsApp(int &argc, char**argv)
    :QApplication(argc, argv)
{
    prepareAppDataPath();

    CoInitialize(0);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    wchar_t temp[MAX_PATH] = L"";
    ExpandEnvironmentStringsW(L"%TEMP%\\", temp, _countof(temp));

    tempPath_ = QString::fromWCharArray(temp);

    iniPath_ = appDataPath_+ "Kouets.ini";

    logPath_ = QString(qApp->applicationDirPath() + "/qdebug_%1.log").arg(
                             QDate::currentDate().toString(Qt::ISODate));

#ifdef _DEBUG
    qInstallMsgHandler(KouetsApp::myMessageHandler);
#endif
    qDebug() << "Launch on " << QDateTime::currentDateTime();
}

/**
 * デストラクタ
 */
KouetsApp::~KouetsApp()
{
    //SaveIni();

    CoUninitialize();
}

/**
 * 設定ファイルの読み込み
 *
 * @return 設定ファイルが読み込めたかどうか。
 */
int KouetsApp::LoadIni()
{
    int ret = 0;

    QSettings stg(GetIniPath(), QSettings::IniFormat);

    // read something

    return ret;
}

int KouetsApp::SaveIni()
{
    int ret = 0;

    QSettings stg(GetIniPath(), QSettings::IniFormat);

    // write something

    return ret;
}


/**
 * 設定などを保存するフォルダパスの生成
 *
 * @note %APPDATA%/Kouets/
 *       --> C:\Users\nob-aoki\AppData\Roaming[WinVista/7/8]
 *       --> C:\Documents and Settings\nob-aoki\Application Data[Win2k/XP]
 */
void KouetsApp::prepareAppDataPath()
{
    wchar_t appdatapath[0x1000];
    ExpandEnvironmentStringsW(L"%APPDATA%\\Kouets\\",
                              appdatapath, _countof(appdatapath));

    appDataPath_ = QString::fromWCharArray(appdatapath);
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
 * qDebug()の出力をファイルにする。
 *
 * @param type 引数'type'の説明
 * @param *msg 引数'*msg'の説明
 *
 * @note ファイル名は"debug_YYYY-MM-DD.log"
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
    KouetsApp*pApp = ((KouetsApp*)qApp);
    if (pApp != NULL) {
        QString fn = QString(pApp->logPath_);
        QFile outFile(fn);
        outFile.open(QIODevice::Text|QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << endl;
    }

    // put to QtCreator.
    OutputDebugStringA(msg);
    OutputDebugStringA("\n");

    if (type == QtFatalMsg) {
        abort();
    }
}
