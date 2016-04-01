/**
 * this file is for testing ProjectXML class.
 */
#ifndef __PROJECT_XML_H_TEST__
# define __PROJECT_XML_H_TEST__

#include "AutoTest.h"

class TestProjectXML : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void test();
    void cleanupTestCase(){}
};

DECLARE_TEST(TestProjectXML);

#endif  // __PROJECT_XML_H_TEST__
