#include "projectfile_test.h"

#include <QtCore>

#include "../projectfile.h"

void TestProjectFile::test()
{
    ProjectFile pf;
    QVERIFY(pf.AppPath() == "");
    QVERIFY(pf.isUseDefaultAppPath() == ProjectFile::True);
    QVERIFY(pf.CmdLine() == "");
    QVERIFY(pf.isUseDefaultCmdLine() == ProjectFile::True);
    QVERIFY(pf.Parser() == "");
    QVERIFY(pf.isUseDefaultParser() == ProjectFile::True);

    QVERIFY(pf.size() == 0);
    if (pf.size() < 1) {
        // out of bound access is not allowed!!
        QWARN("this test requires some elements.");
    } else {
        QCOMPARE(pf.atFilename(0), QString());
        QCOMPARE(pf.atName(0), QString());
        QCOMPARE(pf.atPath(0), QString());
        QCOMPARE(pf.lastUpdated(0), QDateTime());
        QCOMPARE(pf.isUpdated(0), -1);

        QCOMPARE(pf.atFilename(10), QString());
        QCOMPARE(pf.atName(10), QString());
        QCOMPARE(pf.atPath(10), QString());
        QCOMPARE(pf.lastUpdated(10), QDateTime());
        QCOMPARE(pf.isUpdated(10), -1);

        FileConfig&fc = pf.at(0);
        QVERIFY(fc.Filename() == "");
        QVERIFY(fc.AbsPath() == "");
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == false);
        QVERIFY(fc.IsDefaultCmdLine() == false);
        QVERIFY(fc.IsDefaultParser() == false);
    }

    QVERIFY(pf.Remove("path") == -1);

    pf.Add("path");
    QVERIFY(pf.AppPath() == "");
    QVERIFY(pf.isUseDefaultAppPath() == ProjectFile::True);
    QVERIFY(pf.CmdLine() == "");
    QVERIFY(pf.isUseDefaultCmdLine() == ProjectFile::True);
    QVERIFY(pf.Parser() == "");
    QVERIFY(pf.isUseDefaultParser() == ProjectFile::True);

    QVERIFY(pf.size() == 1);
    if (pf.size() < 1) {
        // out of bound access is not allowed!!
        QWARN("this test requires some elements.");
    } else {
        QCOMPARE(pf.atFilename(0), QString("path"));
        QCOMPARE(pf.atName(0), QString("path"));
        QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(pf.atPath(0), QString("D:/programing/QT/kouets/src/test/path"));
        QCOMPARE(pf.lastUpdated(0), QDateTime());
        QCOMPARE(pf.isUpdated(0), -1);

        if (pf.size() < 11) {
            QWARN("this test requires more than 10 elements.");
        } else {
            // out of bound access is not allowed!!
            QCOMPARE(pf.atFilename(10), QString());
            QCOMPARE(pf.atName(10), QString());
            QCOMPARE(pf.atPath(10), QString());
            QCOMPARE(pf.lastUpdated(10), QDateTime());
            QCOMPARE(pf.isUpdated(10), -1);
        }

        FileConfig&fc = pf.at(0);  //   ???
        QVERIFY(fc.Filename() == "path");
        QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(fc.AbsPath(), QString("D:/programing/QT/kouets/src/test/path"));
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == false);
        QVERIFY(fc.IsDefaultCmdLine() == false);
        QVERIFY(fc.IsDefaultParser() == false);
    }

    pf.Add("test.pro");
    QVERIFY(pf.AppPath() == "");
    QVERIFY(pf.isUseDefaultAppPath() == ProjectFile::True);
    QVERIFY(pf.CmdLine() == "");
    QVERIFY(pf.isUseDefaultCmdLine() == ProjectFile::True);
    QVERIFY(pf.Parser() == "");
    QVERIFY(pf.isUseDefaultParser() == ProjectFile::True);

    QVERIFY(pf.size() == 2);
    if (pf.size() < 2) {
        // out of bound access is not allowed!!
        QWARN("this test requires some elements.");
    } else {
        QCOMPARE(pf.atFilename(0), QString("path"));
        QCOMPARE(pf.atName(0), QString("path"));
        QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(pf.atPath(0), QString("D:/programing/QT/kouets/src/test/path"));
        QCOMPARE(pf.lastUpdated(0), QDateTime());
        QCOMPARE(pf.isUpdated(0), -1);

        QCOMPARE(pf.atFilename(1), QString("test.pro"));
        QCOMPARE(pf.atName(1), QString("test.pro"));
        QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(pf.atPath(1), QString("D:/programing/QT/kouets/src/test/test.pro"));
        QCOMPARE(pf.lastUpdated(1), QDateTime());
        QCOMPARE(pf.isUpdated(1), 1);

        if (pf.size() < 11) {
            QWARN("this test requires more than 10 elements.");
        } else {
            // out of bound access is not allowed!!
            QCOMPARE(pf.atFilename(10), QString());
            QCOMPARE(pf.atName(10), QString());
            QCOMPARE(pf.atPath(10), QString());
            QCOMPARE(pf.lastUpdated(10), QDateTime());
            QCOMPARE(pf.isUpdated(10), -1);
        }

        FileConfig&fc = pf.at(1);
        QVERIFY(fc.Filename() == "test.pro");
        QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QVERIFY(fc.AbsPath() == "D:/programing/QT/kouets/src/test/test.pro");
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == false);
        QVERIFY(fc.IsDefaultCmdLine() == false);
        QVERIFY(fc.IsDefaultParser() == false);
    }

    QVERIFY(pf.Find("path") == 0);
    QVERIFY(pf.Find("test.pro") == 1);
    QVERIFY(pf.Find("asdfg") == -1);

    QVERIFY(pf.Remove("path") == 1);
    QVERIFY(pf.size() == 1);
    if (pf.size() < 1) {
        // out of bound access is not allowed!!
        QWARN("this test requires some elements.");
    } else {
        QCOMPARE(pf.atFilename(0), QString("test.pro"));
        QCOMPARE(pf.atName(0), QString("test.pro"));
        QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(pf.atPath(0), QString("D:/programing/QT/kouets/src/test/test.pro"));
        QFileInfo fi("test.pro");
        QCOMPARE(pf.lastUpdated(0), fi.lastModified());
        QCOMPARE(pf.isUpdated(0), 0);

        if (pf.size() < 11) {
            QWARN("this test requires more than 10 elements.");
        } else {
            // out of bound access is not allowed!!
            QCOMPARE(pf.atFilename(10), QString());
            QCOMPARE(pf.atName(10), QString());
            QCOMPARE(pf.atPath(10), QString());
            QCOMPARE(pf.lastUpdated(10), QDateTime());
            QCOMPARE(pf.isUpdated(10), -1);
        }

        FileConfig&fc = pf.at(0);
        QVERIFY(fc.Filename() == "test.pro");
        QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(fc.AbsPath(), QString("D:/programing/QT/kouets/src/test/test.pro"));
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == false);
        QVERIFY(fc.IsDefaultCmdLine() == false);
        QVERIFY(fc.IsDefaultParser() == false);
    }
}

#if 0  // sandbox
    int Open(const QString &path);
    int Save(const QString &path);
    int SavePlainText(const QString &path);
    int SaveXML(const QString &path);
    void Copy(QVector<FileConfig> *fc);
    int Find(const QString&path);
    void resetUpdated(int idx);
    void resetUpdated(const QString&path);
#endif  // sandbox
