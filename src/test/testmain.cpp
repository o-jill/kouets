#include "AutoTest.h"

#include <QTest>
#include <QDebug>

#if 1
// This is all you need to run all the tests
TEST_MAIN
#else
// Or supply your own main function
int main(int argc, char *argv[])
{
    int failures = AutoTest::run(argc, argv);
    if (failures == 0) {
        qDebug() << QString("ALL TESTS PASSED.(%1)").arg(failures);
    } else {
        qDebug() << QString("TESTS FAILED!(%1)").arg(failures);
    }
    return failures;
}
#endif
