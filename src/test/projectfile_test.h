#ifndef __PROJECT_FILE_TEST_H__
#define __PROJECT_FILE_TEST_H__

#include "AutoTest.h"

class TestProjectFile : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase(){}
    void test();
    void cleanupTestCase(){}
};

DECLARE_TEST(TestProjectFile);

#endif  // __PROJECT_FILE_TEST_H__
