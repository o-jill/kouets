/**
 * this file is for testing ProjectFile class.
 */
#include "projectfile_test.h"

#include <QtCore>

#include "../projectfile.h"
#include "../fileconfig.h"

void TestProjectFile::initTestCase()
{
    QDir::setCurrent(qApp->applicationDirPath());
}

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
        QVERIFY(fc.IsDefaultAppPath() == FileConfig::True);
        QVERIFY(fc.IsDefaultCmdLine() == FileConfig::True);
        QVERIFY(fc.IsDefaultParser() == FileConfig::True);
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
        // QEXPECT_FAIL("", "this test depends on your environment", Continue);
        // qDebug() << QDir::currentPath()+"/path";
        QCOMPARE(pf.atPath(0), QDir::currentPath()+"/path");
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
        // QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(fc.AbsPath(), QDir::currentPath()+"/path");
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == FileConfig::True);
        QVERIFY(fc.IsDefaultCmdLine() == FileConfig::True);
        QVERIFY(fc.IsDefaultParser() == FileConfig::True);
    }

    QDir dir;
    dir.cd("..");
    QDir::setCurrent(dir.absolutePath());

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
        // QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(pf.atPath(0), QDir::currentPath()+"/debug/path");
        QCOMPARE(pf.lastUpdated(0), QDateTime());
        QCOMPARE(pf.isUpdated(0), -1);

        QCOMPARE(pf.atFilename(1), QString("test.pro"));
        QCOMPARE(pf.atName(1), QString("test.pro"));
        // QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(pf.atPath(1), QDir::currentPath()+"/test.pro");
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
        // QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(fc.AbsPath(), QDir::currentPath()+"/test.pro");
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == FileConfig::True);
        QVERIFY(fc.IsDefaultCmdLine() == FileConfig::True);
        QVERIFY(fc.IsDefaultParser() == FileConfig::True);
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
        // QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(pf.atPath(0), QDir::currentPath()+"/test.pro");
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
        // QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(fc.AbsPath(), QDir::currentPath()+"/test.pro");
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == FileConfig::True);
        QVERIFY(fc.IsDefaultCmdLine() == FileConfig::True);
        QVERIFY(fc.IsDefaultParser() == FileConfig::True);
    }
}

void TestProjectFile::test2()
{
    ProjectFile pf;

    QVERIFY(pf.Open("file_not_exist") == -1);
    QVERIFY(pf.size() == 0);
    QVERIFY(pf.AppPath() == "");
    QVERIFY(pf.isUseDefaultAppPath() == ProjectFile::True);
    QVERIFY(pf.CmdLine() == "");
    QVERIFY(pf.isUseDefaultCmdLine() == ProjectFile::True);
    QVERIFY(pf.Parser() == "");
    QVERIFY(pf.isUseDefaultParser() == ProjectFile::True);

    QCOMPARE(pf.Open("projectfile_test.cpp"), -2);
    QVERIFY(pf.size() == 0);
    QVERIFY(pf.AppPath() == "");
    QVERIFY(pf.isUseDefaultAppPath() == ProjectFile::True);
    QVERIFY(pf.CmdLine() == "");
    QVERIFY(pf.isUseDefaultCmdLine() == ProjectFile::True);
    QVERIFY(pf.Parser() == "");
    QVERIFY(pf.isUseDefaultParser() == ProjectFile::True);
}

void TestProjectFile::test3()
{
    ProjectFile pf;

    QDir dir;
    dir.cd("..");
    QDir::setCurrent(dir.absolutePath());
    qDebug() << "currentPath:" << QDir::currentPath();

    QVERIFY(pf.Open(QDir::currentPath()+"../kouets.kouets") == 12);
    QVERIFY(pf.size() == 12);

    if (pf.size() < 1) {
        // out of bound access is not allowed!!
        QFAIL("# of element was changed unintentionally!!");
    } else {
        QCOMPARE(pf.atFilename(0), QString("kouetsapp.cpp"));
        QCOMPARE(pf.atName(0), QString("kouetsapp.cpp"));
        // QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(pf.atPath(0), QDir::currentPath()+"/kouetsapp.cpp");
        QCOMPARE(pf.lastUpdated(0), QDateTime());
        QCOMPARE(pf.isUpdated(0), 1);
        //        QFileInfo fi("../kouetsapp.cpp");
        QFileInfo fi(pf.atPath(0));
        QCOMPARE(pf.lastUpdated(0), fi.lastModified());
    }

    if (pf.size() < 6) {
        // out of bound access is not allowed!!
        QFAIL("# of element was changed unintentionally!!");
    } else {
        FileConfig&fc = pf.at(5);
        QVERIFY(fc.Filename() == "projectfile.cpp");
        // QEXPECT_FAIL("", "this test depends on your environment", Continue);
        QCOMPARE(fc.AbsPath(), QDir::currentPath()+"/projectfile.cpp");
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == FileConfig::True);
        QVERIFY(fc.IsDefaultCmdLine() == FileConfig::True);
        QVERIFY(fc.IsDefaultParser() == FileConfig::True);
    }

    if (pf.size() < 11) {
        // out of bound access is not allowed!!
        QFAIL("# of element was changed unintentionally!!");
    } else {
        QCOMPARE(pf.atFilename(10), QString("projectxml.cpp"));
        QCOMPARE(pf.atName(10), QString("projectxml.cpp"));
        QCOMPARE(pf.atPath(10), QDir::currentPath()+"/projectxml.cpp");
        QCOMPARE(pf.lastUpdated(10), QDateTime());
        QCOMPARE(pf.isUpdated(10), 1);
    }
}

#if 0  // sandbox
    // @return -2:unknown format, -1:open error, 0:no content, more than zero:number of files.
    int Open(const QString &path);
    int Save(const QString &path);
    int SavePlainText(const QString &path);
    int SaveXML(const QString &path);
    void Copy(QVector<FileConfig> *fc);
    int Find(const QString&path);
    void resetUpdated(int idx);
    void resetUpdated(const QString&path);
#endif  // sandbox
