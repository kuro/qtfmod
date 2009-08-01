
#include "fmod.moc"
#include <System.h>

#include <QtTest>

using namespace QtFMOD;

#define chk(obj)                                                            \
    if ((obj).error() != 0) {                                               \
        QFAIL((obj).errorString().toUtf8());                                \
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

void TestFMOD::play_sound ()
{

    System sys;
    chk(sys);

    sys.init(32);
    chk(sys);

    QTimer timer;
    connect(&timer, SIGNAL(timeout()), &sys, SLOT(update()));
    timer.start(16);

    Sound* snd = NULL;

    snd = sys.createSound("jaguar.wav");
    chk(sys);

    QVERIFY(!snd->isNull());

    Channel* chan = NULL;
    chan = sys.playSound(FMOD_CHANNEL_FREE, snd);
    QVERIFY(!chan->isNull());

    QVERIFY(chan->isPlaying());

    QEventLoop loop;
    connect(chan, SIGNAL(soundEnded()), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY(!chan->isPlaying());

    sys.close();
    chk(sys);
}

int fmod (int argc, char *argv[])
{
    QCoreApplication app (argc, argv);
    TestFMOD tc;
    return QTest::qExec(&tc, argc, argv);
}

// vim: sw=4
