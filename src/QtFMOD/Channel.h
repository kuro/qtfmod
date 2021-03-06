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
 * @file Channel.h
 * @brief Channel definition
 */

#pragma once

#include <QObject>
#include <QSharedPointer>

#include <fmod.hpp>

namespace QtFMOD
{

class DSP;
class DSPConnection;

class Channel : public QObject
{
    Q_OBJECT

public:
    virtual ~Channel ();

    Channel (const Channel& other);
    Channel& operator= (const Channel& other);

    int error () const;
    QString errorString () const;

    bool isNull () const;

    bool isPlaying () const;

    void setPaused (bool paused);
    bool paused () const;

    void setVolume (float volume);
    float volume () const;

    operator FMOD::Channel*& () const;

    void spectrum (QVector<float>& array, int channelOffset,
                   FMOD_DSP_FFT_WINDOW windowType) const;

    void waveData (QVector<float>& waveArray, int channelOffset) const;

    QSharedPointer<DSPConnection> addDSP (QSharedPointer<DSP>& dsp);

    /// @name Standard functionality
    //@{
    void setPosition (unsigned int position, FMOD_TIMEUNIT timeUnit);
    unsigned int position (FMOD_TIMEUNIT timeUnit) const;
    //@}

    /// @name 3D functionality
    //@{
    void set3DAttributes (const float* pos, const float* vel = NULL);
    //@}

signals:
    void soundEnded ();

private:
    Channel ();
    friend class System;

    void setInternalPointer (FMOD::Channel* fchannel);
    FMOD::Channel* internalPointer () const;

public:
    static FMOD_RESULT F_CALLBACK callback (
        FMOD_CHANNEL* channel,
        FMOD_CHANNEL_CALLBACKTYPE type,
        void* command_data1, void* command_data2);

private:
    struct Private;
    QScopedPointer<Private> d;
};
} // namespace QtFMOD


// vim: sw=4
