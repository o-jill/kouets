/**
 * this file is for testing ProjectXML class.
 */
#include "projectxml_test.h"

#include <QtCore>

#include "../projectxml.h"
#include "../fileconfig.h"

void TestProjectXML::initTestCase()
{
    QDir::setCurrent(qApp->applicationDirPath());
}

void TestProjectXML::test()
{
    ProjectXML xml;
    QVERIFY(xml.AppPath() == "");
    QVERIFY(xml.IsDefaultAppPath() == ProjectXML::True);
    QVERIFY(xml.CmdLine() == "");
    QVERIFY(xml.IsDefaultCmdLine() == ProjectXML::True);
    QVERIFY(xml.Parser() == "");
    QVERIFY(xml.IsDefaultParser() == ProjectXML::True);
    QVERIFY(xml.Files() != NULL);

    QVERIFY(xml.ItemSize() == 0);
    if (xml.ItemSize() < 1) {
        // out of bound access is not allowed!!
        QWARN("this test requires some elements.");
    } else {
        FileConfig&fc = xml.at(0);
        QVERIFY(fc.Filename() == "");
        QVERIFY(fc.AbsPath() == "");
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == FileConfig::True);
        QVERIFY(fc.IsDefaultCmdLine() == FileConfig::True);
        QVERIFY(fc.IsDefaultParser() == FileConfig::True);
    }

    QVERIFY(xml.readFile("file_not_exist") == false);
    QVERIFY(xml.AppPath() == "");
    QVERIFY(xml.IsDefaultAppPath() == ProjectXML::True);
    QVERIFY(xml.CmdLine() == "");
    QVERIFY(xml.IsDefaultCmdLine() == ProjectXML::True);
    QVERIFY(xml.Parser() == "");
    QVERIFY(xml.IsDefaultParser() == ProjectXML::True);
    QVERIFY(xml.Files() != NULL);
    QVERIFY(xml.ItemSize() == 0);

    QDir dir;
    dir.cd("..");
    QDir::setCurrent(dir.absolutePath());

    QVERIFY(xml.readFile("xml.kouets") == true);
    QVERIFY(xml.AppPath() == "C:/path/to/default.exe");
    QVERIFY(xml.IsDefaultAppPath() == ProjectXML::False);
    QVERIFY(xml.CmdLine() == "--some --default --command --option");
    QVERIFY(xml.IsDefaultCmdLine() == ProjectXML::False);
    QVERIFY(xml.Parser() == "");
    QVERIFY(xml.IsDefaultParser() == ProjectXML::True);
    QVERIFY(xml.Files() != NULL);
    QCOMPARE(xml.ItemSize(), 2);

    if (xml.ItemSize() < 2) {
        // out of bound access is not allowed!!
        QFAIL("# of element was changed unintentionally!!");
    } else {
        FileConfig&fc = xml.at(0);
        QVERIFY(fc.Filename() == "kouetsapp.cpp");
        QCOMPARE(fc.AbsPath(), QDir::currentPath()+"/kouetsapp.cpp");
        QVERIFY(fc.AppPath() == "");
        QVERIFY(fc.CmdLine() == "");
        QVERIFY(fc.Parser() == "");
        QVERIFY(fc.IsDefaultAppPath() == FileConfig::False);
        QVERIFY(fc.IsDefaultCmdLine() == FileConfig::True);
        QVERIFY(fc.IsDefaultParser() == FileConfig::True);

        FileConfig&fc2 = xml.at(1);
        QVERIFY(fc2.Filename() == "kouetsapp.h");
        QCOMPARE(fc2.AbsPath(), QDir::currentPath()+"/kouetsapp.h");
        QVERIFY(fc2.AppPath() == "C:/speci/alpat/htoan/application.exe");
        QVERIFY(fc2.CmdLine() == "--some --command --option");
        QVERIFY(fc2.Parser() == "cpplint.py_VS7");
        QVERIFY(fc2.IsDefaultAppPath() == FileConfig::False);
        QVERIFY(fc2.IsDefaultCmdLine() == FileConfig::False);
        QVERIFY(fc2.IsDefaultParser() == FileConfig::False);
    }
}

#if 0  // -- -- sandbox -- --
    bool readFile(const QString &fileName);
    bool readFile(QFile *pfile);
    bool IsDefaultAppPath() {return bapppath_;}
    bool IsDefaultCmdLine() {return bcmdline_;}
    bool IsDefaultParser() {return bparser_;}
    QString AppPath() {return apppath_;}
    QString CmdLine() {return cmdline_;}
    QString Parser() {return parser_;}
    int ItemSize() {return items_.size();}
    FileConfig at(int n) {return items_[n];}
    QVector<FileConfig>* Files() {return &items_;}

    void dump();
#endif  // -- -- sandbox -- --
