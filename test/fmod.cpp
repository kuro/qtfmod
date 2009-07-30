
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

    sys.init(32);
    chk(sys);

    sys.update();
    chk(sys);

    sys.close();
    chk(sys);
}

void TestFMOD::drivers ()
{
    System sys;
    chk(sys);
    int num = sys.numDrivers();
    chk(sys);

    QString name;
    QUuid uuid;
    for (int i = 0; i < num; i++) {
        sys.driverInfo(i, name, uuid);
        qDebug() << "driver" << (i+1) << "of" << num;
        qDebug() << '\t' << name;
        qDebug() << '\t' << uuid;
    }
}

int fmod (int argc, char *argv[])
{
    TestFMOD tc;
    return QTest::qExec(&tc, argc, argv);
}

// vim: sw=4
