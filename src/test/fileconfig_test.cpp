#include "fileconfig_test.h"

#include "../fileconfig.h"

void TestFileConfig::test()
{
    FileConfig fc;

    QVERIFY(fc.Filename() == "");
    QVERIFY(fc.AbsPath() == "");
    QVERIFY(fc.AppPath() == "");
    QVERIFY(fc.CmdLine() == "");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.IsDefaultAppPath() == false);
    QVERIFY(fc.IsDefaultCmdLine() == false);
    QVERIFY(fc.IsDefaultParser() == false);

    fc.SetFilename("fileconfig_test.h");
    QVERIFY(fc.Filename() == "fileconfig_test.h");
    QEXPECT_FAIL("", "this will be fail because of environment dependency.", Continue);
    QCOMPARE(fc.AbsPath(), QString("D:/programing/QT/kouets/src/test/fileconfig_test.h"));
    QVERIFY(fc.AppPath() == "");
    QVERIFY(fc.CmdLine() == "");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.IsDefaultAppPath() == false);
    QVERIFY(fc.IsDefaultCmdLine() == false);
    QVERIFY(fc.IsDefaultParser() == false);

    fc.SetAppPath("path/to/executable/file");
    QVERIFY(fc.Filename() == "fileconfig_test.h");
    QEXPECT_FAIL("", "this will be fail because of environment dependency.", Continue);
    QCOMPARE(fc.AbsPath(), QString("D:/programing/QT/kouets/src/test/fileconfig_test.h"));
    QVERIFY(fc.AppPath() == "path/to/executable/file");
    QVERIFY(fc.CmdLine() == "");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.IsDefaultAppPath() == true);
    QVERIFY(fc.IsDefaultCmdLine() == false);
    QVERIFY(fc.IsDefaultParser() == false);

    fc.SetCmdLine("arg1 arg2");
    QVERIFY(fc.Filename() == "fileconfig_test.h");
    QEXPECT_FAIL("", "this will be fail because of environment dependency.", Continue);
    QCOMPARE(fc.AbsPath(), QString("D:/programing/QT/kouets/src/test/fileconfig_test.h"));
    QVERIFY(fc.AppPath() == "path/to/executable/file");
    QVERIFY(fc.CmdLine() == "arg1 arg2");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.IsDefaultAppPath() == true);
    QVERIFY(fc.IsDefaultCmdLine() == true);
    QVERIFY(fc.IsDefaultParser() == false);

    fc.SetParser("cpplint.py_VS7");
    QVERIFY(fc.Filename() == "fileconfig_test.h");
    QEXPECT_FAIL("", "this will be fail because of environment dependency.", Continue);
    QCOMPARE(fc.AbsPath(), QString("D:/programing/QT/kouets/src/test/fileconfig_test.h"));
    QVERIFY(fc.AppPath() == "path/to/executable/file");
    QVERIFY(fc.CmdLine() == "arg1 arg2");
    QVERIFY(fc.Parser() == "cpplint.py_VS7");
    QVERIFY(fc.IsDefaultAppPath() == true);
    QVERIFY(fc.IsDefaultCmdLine() == true);
    QVERIFY(fc.IsDefaultParser() == true);

    FileConfig fc2 = fc;
    QVERIFY(fc2.Filename() == "fileconfig_test.h");
    QEXPECT_FAIL("", "this will be fail because of environment dependency.", Continue);
    QCOMPARE(fc2.AbsPath(), QString("D:/programing/QT/kouets/src/test/fileconfig_test.h"));
    QVERIFY(fc2.AppPath() == "path/to/executable/file");
    QVERIFY(fc2.CmdLine() == "arg1 arg2");
    QVERIFY(fc2.Parser() == "cpplint.py_VS7");
    QVERIFY(fc2.IsDefaultAppPath() == true);
    QVERIFY(fc2.IsDefaultCmdLine() == true);
    QVERIFY(fc2.IsDefaultParser() == true);

    fc.Init();
    QVERIFY(fc.Filename() == "");
    QVERIFY(fc.AbsPath() == "");
    QVERIFY(fc.AppPath() == "");
    QVERIFY(fc.CmdLine() == "");
    QVERIFY(fc.Parser() == "");
    QVERIFY(fc.IsDefaultAppPath() == false);
    QVERIFY(fc.IsDefaultCmdLine() == false);
    QVERIFY(fc.IsDefaultParser() == false);
}

/* -- -- -- -- sandbox

    FileConfig(const FileConfig &rhs);

    void Init();

    void SetFilename_(const QString &str);
    void SetAbsPath_(const QString &str);

-- -- -- -- */
