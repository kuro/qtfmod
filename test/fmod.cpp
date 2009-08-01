/*
 * Copyright 2009 Blanton Black
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
