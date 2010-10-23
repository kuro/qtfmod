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
 * @file Channel.cpp
 * @brief Channel implementation
 */

#include "Channel.moc"

#include "DSP.h"
#include "DSPConnection.h"

#include <fmod_errors.h>

#include <QDebug>

using namespace QtFMOD;

struct Channel::Private
{
    mutable FMOD_RESULT fr;
    FMOD::Channel* fchannel;

    Private () :
        fr(FMOD_OK),
        fchannel(NULL)
    {
    }
    ~Private ()
    {
    }
};

/**
 * @note This is intended to be wrapped in a shared pointer, without a parent.
 */
Channel::Channel () :
    QObject(),
    d(new Private)
{
}

Channel::~Channel ()
{
    // this channel may still be playing.  this is just a wrapper, and channels
    // can't be released, so remove the callback
    d->fchannel->setCallback(NULL);
}

void Channel::setInternalPointer (FMOD::Channel* fchannel)
{
    d->fchannel = fchannel;
    d->fchannel->setUserData(this);
    d->fchannel->setCallback(Channel::callback);
}

FMOD::Channel* Channel::internalPointer () const
{
    return d->fchannel;
}

bool Channel::isNull () const
{
    return (d->fchannel == NULL);
}

int Channel::error () const
{
    return d->fr;
}

QString Channel::errorString () const
{
    return FMOD_ErrorString(d->fr);
}

Channel::operator FMOD::Channel*& () const
{
    return d->fchannel;
}

bool Channel::isPlaying () const
{
    bool v;
    d->fr = d->fchannel->isPlaying(&v);
    return v;
}

/**
 * Static channel callback.
 */
FMOD_RESULT F_CALLBACK Channel::callback (FMOD_CHANNEL* fchannel,
                                          FMOD_CHANNEL_CALLBACKTYPE type,
                                          void* command_data1,
                                          void* command_data2)
{
    Q_UNUSED(command_data1);
    Q_UNUSED(command_data2);

    FMOD_RESULT fr;
    Channel* channel = NULL;

    // get QtFMOD::Channel from the user data
    fr = FMOD_Channel_GetUserData(fchannel, (void**)&channel);

    if (fr != FMOD_OK) {
        qCritical() << FMOD_ErrorString(fr);
        return FMOD_OK;
    }

    Q_ASSERT(channel);
    Q_ASSERT(fchannel==reinterpret_cast<FMOD_CHANNEL*>(channel->d->fchannel));

    // process
    switch (type) {
    case FMOD_CHANNEL_CALLBACKTYPE_END:
        qDebug() << "callback: channel: end";
        emit channel->soundEnded();
        break;
    case FMOD_CHANNEL_CALLBACKTYPE_VIRTUALVOICE:
        qDebug() << "callback: channel: virtual voice";
        break;
    case FMOD_CHANNEL_CALLBACKTYPE_SYNCPOINT:
        qDebug() << "callback: channel: syncpoint";
        break;
    case FMOD_CHANNEL_CALLBACKTYPE_OCCLUSION:
        qDebug() << "callback: channel: occlusion";
        break;
    default:
        qCritical() << "callback: channel: unsupported callback type";
        break;
    }

    return FMOD_OK;
}

void Channel::setPaused (bool paused)
{
    d->fr = d->fchannel->setPaused(paused);
}

bool Channel::paused () const
{
    bool paused;
    d->fr = d->fchannel->getPaused(&paused);
    return paused;
}

/**
 * @param array A power of two (64 to 8192).
 */
void Channel::spectrum (QVector<float>& array, int channelOffset,
                        FMOD_DSP_FFT_WINDOW windowType) const
{
    d->fr = d->fchannel->getSpectrum(array.data(), array.size(),
                                     channelOffset, windowType);
}

/**
 * @param waveArray Max size 16384.
 */
void Channel::waveData (QVector<float>& waveArray, int channelOffset) const
{
    d->fr = d->fchannel->getWaveData(waveArray.data(), waveArray.size(),
                                     channelOffset);
}

QSharedPointer<DSPConnection> Channel::addDSP (QSharedPointer<DSP>& dsp)
{
    FMOD::DSPConnection* fdspConnection = NULL;
    d->fr = d->fchannel->addDSP(dsp->internalPointer(), &fdspConnection);
    return QSharedPointer<DSPConnection>(new DSPConnection(fdspConnection));
}

// vim: sw=4
