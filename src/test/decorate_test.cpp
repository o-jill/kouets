#include "decorate_test.h"

#include <QtCore>

#include "../decorate.h"

void TestDecorate::initTestCase()
{
    QDir::setCurrent(qApp->applicationDirPath());
}

void TestDecorate::testNone()
{
#if 0  // abstract class is not able to be instanciated.
    DecorateBase db;

    QVERIFY(db.Name() == "");
    QVERIFY(db.ErrorNum() == -1);
#endif
    DecorateNone dn;

    QVERIFY(dn.Name() == "none");
    QVERIFY(dn.ErrorNum() == -1);

    QString inputtest = "( > < ) <<&&& ===[abc]";
    QString result = dn.decorate(inputtest);
    QString correct_answer = "( &gt; &lt; ) &lt;&lt;&&& ===[abc]<BR>";
    QCOMPARE(result, correct_answer);

    QStringList sl;
    sl << "( > < ) <<" << "&&& ===[abc]";
    result = dn.Decorate(&sl);
    QString correct_answer2 = "( &gt; &lt; ) &lt;&lt;<BR>";
    QCOMPARE(result, correct_answer2);
    sl << "";
    result = dn.Decorate(&sl);
    QString correct_answer3 = "( &gt; &lt; ) &lt;&lt;<BR>&&& ===[abc]<BR>";
    QCOMPARE(result, correct_answer3);

    QString str1("( > < ) <<\n" "&&& ===[abc]");
    QTextStream ts1(&str1);
    result = dn.Decorate(&ts1);
    QString correct_answer4 = "( &gt; &lt; ) &lt;&lt;<BR>&&& ===[abc]<BR>";
    QCOMPARE(result, correct_answer4);

    QString str2("( > < ) <<\n" "&&& ===[abc]\n");
    QTextStream ts2(&str2);
    result = dn.Decorate(&ts2);
    QString correct_answer5 = "( &gt; &lt; ) &lt;&lt;<BR>&&& ===[abc]<BR>";
    QCOMPARE(result, correct_answer5);
}

void TestDecorate::testGCppVs7()
{
    DecorateGCppVs7 dgcv7;

    QVERIFY(dgcv7.Name() == "cpplint.py_VS7");
    QVERIFY(dgcv7.ErrorNum() == -1);
}

void TestDecorate::testDMgr()
{
    DecorationManager dm;
    QVERIFY(dm.find("arienai_namae_desu") == NULL);
    QVERIFY(dm.find("none") != NULL);
    QVERIFY(dm.find("cpplint.py_VS7") != NULL);
    DecorateBase*pdb;
    QVERIFY((pdb = dm.find("none")) != NULL);
    QVERIFY(pdb->Name() == "none");
    QVERIFY(pdb->ErrorNum() == -1);
    QVERIFY((pdb = dm.find("cpplint.py_VS7")) != NULL);
    QVERIFY(pdb->Name() == "cpplint.py_VS7");
    QVERIFY(pdb->ErrorNum() == -1);
}

#if 0   // sandbox

#endif  // sandbox
