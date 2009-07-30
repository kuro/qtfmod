
#include "fmod.moc"
#include <System.h>

#include <QtTest>

#define chk(obj)                                                            \
    if (obj.error() != 0) {                                                 \
        QFAIL(qPrintable(obj.errorString()));                               \
    }

void TestFMOD::initialize ()
{
    System sys;
    chk(sys);
}

int fmod (int argc, char *argv[])
{
    TestFMOD tc;
    return QTest::qExec(&tc, argc, argv);
}

// vim: sw=4
