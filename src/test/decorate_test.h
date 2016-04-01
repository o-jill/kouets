#ifndef __DECORATE_TEST_H__
# define __DECORATE_TEST_H__

#include "AutoTest.h"

class TestDecorate :  public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testNone();
    void testGCppVs7();
    void testDMgr();
    void cleanupTestCase(){}
};

DECLARE_TEST(TestDecorate);

#endif  // __DECORATE_TEST_H__
