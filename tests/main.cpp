#include <QtTest>
#include "instructionstest.h"
#include "observertest.h"


int main(int argc, char *argv[])
{
    unsigned int status = 0;
    status |= (unsigned int)QTest::qExec(new InstructionTest, argc, argv);
    status |= (unsigned int)QTest::qExec(new ObserverTest, argc, argv);

    return (int)status;
}
