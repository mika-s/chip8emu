#include <QtTest>
#include "instructionstest.h"
#include "observertest.h"


int main(int argc, char *argv[])
{
    int status = 0;
    status |= QTest::qExec(new InstructionTest, argc, argv);
    status |= QTest::qExec(new ObserverTest, argc, argv);

    return status;
}
