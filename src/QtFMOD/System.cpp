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

/**
 * @file System.cpp
 * @brief System implementation
 */

#include "System.moc"
#include "fileio.h"

#include "Sound.h"
#include "Channel.h"
#include "DSP.h"
#include "DSPConnection.h"

#include <fmod_errors.h>
#include <QFile>

#include <QDebug>
#include <QCoreApplication>

using namespace QtFMOD;

#define make_uuid(guid)                                                     \
    QUuid(guid.Data1, guid.Data2, guid.Data3,                               \
          guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],       \
          guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]        \
         );

struct System::Private
{
    mutable FMOD_RESULT fr;
    FMOD::System* fsystem;

    Private () :
        fr(FMOD_OK),
        fsystem(NULL)
    {
    }
    ~Private ()
    {
        fr = fsystem->release();
    }
};

System::System (QObject* parent) :
    QObject(parent),
    d(new System::Private)
{
    d->fr = FMOD::System_Create(&d->fsystem);
    d->fsystem->setUserData(this);
}

System::~System ()
{
}

int System::error () const
{
    return d->fr;
}

QString System::errorString () const
{
    return FMOD_ErrorString(d->fr);
}

FMOD_OUTPUTTYPE System::output () const
{
    FMOD_OUTPUTTYPE output;
    d->fr = d->fsystem->getOutput(&output);
    return output;
}

void System::setOutput (FMOD_OUTPUTTYPE output)
{
    d->fr = d->fsystem->setOutput(output);
}

int System::numDrivers () const
{
    int num;
    d->fr = d->fsystem->getNumDrivers(&num);
    return num;
}

void System::driverInfo (int id, QString& name, QUuid& uuid) const
{
    char name_buf[1024];
    FMOD_GUID guid;

    d->fr = d->fsystem->getDriverInfo(id, name_buf, sizeof(name_buf), &guid);

    name = name_buf;
    uuid = make_uuid(guid);
}

int System::driver () const
{
    int retval;
    d->fr = d->fsystem->getDriver(&retval);
    return retval;
}

System::operator FMOD::System* () const
{
    return d->fsystem;
}

void System::init (int max_channels, FMOD_INITFLAGS flags)
{
    d->fr = d->fsystem->init(max_channels, flags, NULL);

    d->fsystem->setCallback(System::callback);
}

void System::close ()
{
    d->fr = d->fsystem->close();
}

void System::update ()
{
    d->fr = d->fsystem->update();
}

void System::set3DSettings (float dopplerScale, float distanceFactor,
                            float rolloffScale)
{
    d->fr = d->fsystem->set3DSettings(dopplerScale, distanceFactor,
                                      rolloffScale);
}

void System::get3DSettings (float* dopplerScale, float* distanceFactor,
                            float* rolloffScale)
{
    d->fr = d->fsystem->get3DSettings(dopplerScale, distanceFactor,
                                      rolloffScale);
}

void System::set3DNumListeners (int nbListeners)
{
    d->fr = d->fsystem->set3DNumListeners(nbListeners);
}

void System::set3DListenerAttributes (int listener,
                                      const float* pos,
                                      const float* vel,
                                      const float* forward,
                                      const float* up)
{
    d->fr = d->fsystem->set3DListenerAttributes(
        listener,
        reinterpret_cast<const FMOD_VECTOR*>(pos),
        reinterpret_cast<const FMOD_VECTOR*>(vel),
        reinterpret_cast<const FMOD_VECTOR*>(forward),
        reinterpret_cast<const FMOD_VECTOR*>(up)
        );
}

QSharedPointer<Sound> System::createSound (const QString& name, FMOD_MODE mode,
                                           FMOD_CREATESOUNDEXINFO* exinfo)
{
    FMOD_CREATESOUNDEXINFO default_info;

    if (exinfo == NULL) {
        exinfo = &default_info;
        memset(&default_info, 0, sizeof(default_info));
        default_info.cbsize = sizeof(default_info);

        if (QFile::exists(name)) {
            default_info.useropen  = qtfmod_file_open;
            default_info.userclose = qtfmod_file_close;
            default_info.userread  = qtfmod_file_read;
            default_info.userseek  = qtfmod_file_seek;
        }
    }

    FMOD::Sound* fsnd = NULL;
    d->fr = d->fsystem->createSound(name.toUtf8(), mode, exinfo, &fsnd);
    return QSharedPointer<Sound>(new Sound(fsnd));
}

QSharedPointer<Sound> System::createStream (const QString& name, FMOD_MODE mode,
                                            FMOD_CREATESOUNDEXINFO* exinfo)
{
    FMOD_CREATESOUNDEXINFO default_info;

    if (exinfo == NULL) {
        exinfo = &default_info;
        memset(&default_info, 0, sizeof(default_info));
        default_info.cbsize = sizeof(default_info);

        if (QFile::exists(name)) {
            default_info.useropen  = qtfmod_file_open;
            default_info.userclose = qtfmod_file_close;
            default_info.userread  = qtfmod_file_read;
            default_info.userseek  = qtfmod_file_seek;
        }
    }

    FMOD::Sound* fsnd = NULL;
    d->fr = d->fsystem->createStream(name.toUtf8(), mode, exinfo, &fsnd);
    return QSharedPointer<Sound>(new Sound(fsnd));
}

void System::playSound (FMOD_CHANNELINDEX channel_id,
                        QSharedPointer<Sound> sound,
                        bool paused, QSharedPointer<Channel>& channel)
{
    if (!channel) {
        channel = QSharedPointer<Channel>(new Channel);
    }

    FMOD::Channel* fchan = channel->internalPointer();

    d->fr = d->fsystem->playSound(channel_id, sound->internalPointer(),
                                  paused, &fchan);

    channel->setInternalPointer(fchan);
}

/**
 * Static system callback.
 */
FMOD_RESULT F_CALLBACK System::callback (FMOD_SYSTEM* fsystem,
                                         FMOD_SYSTEM_CALLBACKTYPE type,
                                         void* command_data1,
                                         void* command_data2)
{
    Q_UNUSED(command_data1);
    Q_UNUSED(command_data2);

    FMOD_RESULT fr;
    System* system = NULL;

    // get QtFMOD::System from the user data
    fr = FMOD_System_GetUserData(fsystem, (void**)&system);

    if (fr != FMOD_OK) {
        qCritical() << FMOD_ErrorString(fr);
        return FMOD_OK;
    }

    Q_ASSERT(system);
    Q_ASSERT(fsystem == reinterpret_cast<FMOD_SYSTEM*>(system->d->fsystem));

    // process callback
    switch (type) {
    case FMOD_SYSTEM_CALLBACKTYPE_DEVICELISTCHANGED:
        qDebug() << "callback: system: devicelistchanged";
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_MEMORYALLOCATIONFAILED:
        qDebug() << "callback: system: memoryallocationfailed";
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_THREADCREATED:
        qDebug() << "callback: system: threadcreated";
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_BADDSPCONNECTION:
        qDebug() << "callback: system: baddspconnection";
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_BADDSPLEVEL:
        qDebug() << "callback: system: baddsplevel";
        break;
    default:
        qCritical() << "callback: system: unsupported callback type";
        break;
    }

    return FMOD_OK;
}

/**
 * @param spectrumArray A power of two (64 to 8192).
 */
void System::spectrum (QVector<float>& spectrumArray, int channelOffset,
                       FMOD_DSP_FFT_WINDOW windowType) const
{
    d->fr = d->fsystem->getSpectrum(spectrumArray.data(), spectrumArray.size(),
                                    channelOffset, windowType);
}

/**
 * @param waveArray Max size 16384.
 */
void System::waveData (QVector<float>& waveArray, int channelOffset) const
{
    d->fr = d->fsystem->getWaveData(waveArray.data(), waveArray.size(),
                                    channelOffset);
}

QSharedPointer<DSP> System::createDSP (FMOD_DSP_TYPE type)
{
    FMOD::DSP* fdsp = NULL;
    d->fr = d->fsystem->createDSPByType(type, &fdsp);
    return QSharedPointer<DSP>(new DSP(fdsp));
}

QSharedPointer<DSPConnection> System::addDSP (QSharedPointer<DSP>& dsp)
{
    FMOD::DSPConnection* fdspConnection = NULL;
    d->fr = d->fsystem->addDSP(dsp->internalPointer(), &fdspConnection);
    return QSharedPointer<DSPConnection>(new DSPConnection(fdspConnection));
}

// vim: sw=4
