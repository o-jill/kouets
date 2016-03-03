#ifndef __FILECONFIG_TEST_H__
# define __FILECONFIG_TEST_H__

#include "AutoTest.h"

class TestFileConfig :  public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void test();
    void cleanupTestCase(){}
};

DECLARE_TEST(TestFileConfig);

#endif  // __FILECONFIG_TEST_H__
