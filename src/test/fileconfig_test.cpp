#include "fileconfig_test.h"

#include <QtCore>

#include "../fileconfig.h"

void TestFileConfig::initTestCase()
{
    QDir::setCurrent(qApp->applicationDirPath());
}

void TestFileConfig::test()
{
    FileConfig fc;

    QVERIFY(fc.Filename() == "");
    QVERIFY(fc.AbsPath() == "");
    QVERIFY(fc.AppPath() == "");
    QVERIFY(fc.CmdLine() == "");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.HasAppPath() == false);
    QVERIFY(fc.HasCmdLine() == false);
    QVERIFY(fc.HasParser() == false);

    QDir dir;
    dir.cd("..");
    QDir::setCurrent(dir.absolutePath());

    fc.SetFilename("fileconfig_test.h");
    QVERIFY(fc.Filename() == "fileconfig_test.h");
    QCOMPARE(fc.AbsPath(), QDir::currentPath()+"/fileconfig_test.h");
    QVERIFY(fc.AppPath() == "");
    QVERIFY(fc.CmdLine() == "");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.HasAppPath() == false);
    QVERIFY(fc.HasCmdLine() == false);
    QVERIFY(fc.HasParser() == false);

    fc.SetAppPath("path/to/executable/file");
    QVERIFY(fc.Filename() == "fileconfig_test.h");
    QCOMPARE(fc.AbsPath(), QDir::currentPath()+"/fileconfig_test.h");
    QVERIFY(fc.AppPath() == "path/to/executable/file");
    QVERIFY(fc.CmdLine() == "");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.HasAppPath() == true);
    QVERIFY(fc.HasCmdLine() == false);
    QVERIFY(fc.HasParser() == false);

    fc.SetCmdLine("arg1 arg2");
    QVERIFY(fc.Filename() == "fileconfig_test.h");
    QCOMPARE(fc.AbsPath(), QDir::currentPath()+"/fileconfig_test.h");
    QVERIFY(fc.AppPath() == "path/to/executable/file");
    QVERIFY(fc.CmdLine() == "arg1 arg2");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.HasAppPath() == true);
    QVERIFY(fc.HasCmdLine() == true);
    QVERIFY(fc.HasParser() == false);

    fc.SetParser("cpplint.py_VS7");
    QVERIFY(fc.Filename() == "fileconfig_test.h");
    QCOMPARE(fc.AbsPath(), QDir::currentPath()+"/fileconfig_test.h");
    QVERIFY(fc.AppPath() == "path/to/executable/file");
    QVERIFY(fc.CmdLine() == "arg1 arg2");
    QVERIFY(fc.Parser() == "cpplint.py_VS7");
    QVERIFY(fc.HasAppPath() == true);
    QVERIFY(fc.HasCmdLine() == true);
    QVERIFY(fc.HasParser() == true);

    FileConfig fc2 = fc;
    QVERIFY(fc2.Filename() == "fileconfig_test.h");
    QCOMPARE(fc2.AbsPath(), QDir::currentPath()+"/fileconfig_test.h");
    QCOMPARE(fc2.AppPath(), QString("path/to/executable/file"));
    QVERIFY(fc2.CmdLine() == "arg1 arg2");
    QVERIFY(fc2.Parser() == "cpplint.py_VS7");
    QVERIFY(fc2.HasAppPath() == true);
    QVERIFY(fc2.HasCmdLine() == true);
    QVERIFY(fc2.HasParser() == true);

    fc.Init();
    QVERIFY(fc.Filename() == "");
    QVERIFY(fc.AbsPath() == "");
    QVERIFY(fc.AppPath() == "");
    QVERIFY(fc.CmdLine() == "");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.HasAppPath() == false);
    QVERIFY(fc.HasCmdLine() == false);
    QVERIFY(fc.HasParser() == false);
}

/* -- -- -- -- sandbox

    FileConfig(const FileConfig &rhs);

    void Init();

    void SetFilename_(const QString &str);
    void SetAbsPath_(const QString &str);

-- -- -- -- */
