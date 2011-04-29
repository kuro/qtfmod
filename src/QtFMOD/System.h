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
 * @file System.h
 * @brief System definition
 */

#pragma once

#include <QObject>
#include <QUuid>
#include <QSharedPointer>

#include <fmod.hpp>

namespace QtFMOD
{

class Sound;
class Channel;
class DSP;
class DSPConnection;

class System : public QObject
{
    Q_OBJECT

public:
    System (QObject* parent = NULL);
    virtual ~System ();

    int error () const;
    QString errorString () const;

    FMOD_OUTPUTTYPE output () const;
    void setOutput (FMOD_OUTPUTTYPE output);

    int numDrivers () const;
    void driverInfo (int id, QString& name, QUuid& uuid) const;

    int driver () const;

    void init (int max_channels, FMOD_INITFLAGS flags = FMOD_INIT_NORMAL);
    void close ();

    operator FMOD::System* () const;

    // sound
    QSharedPointer<Sound> createSound (const QString& name,
                                       FMOD_MODE mode = FMOD_DEFAULT,
                                       FMOD_CREATESOUNDEXINFO* exinfo = NULL
                                      );
    QSharedPointer<Sound> createStream (const QString& name,
                                        FMOD_MODE mode = FMOD_DEFAULT,
                                        FMOD_CREATESOUNDEXINFO* exinfo = NULL
                                       );

    void playSound (FMOD_CHANNELINDEX channel_id, QSharedPointer<Sound> sound,
                    bool paused, QSharedPointer<Channel>& channel);

    void spectrum (QVector<float>& spectrumArray, int channelOffset,
                   FMOD_DSP_FFT_WINDOW windowType) const;

    void waveData (QVector<float>& waveArray, int channelOffset) const;

    QSharedPointer<DSP> createDSP (FMOD_DSP_TYPE type);
    QSharedPointer<DSPConnection> addDSP (QSharedPointer<DSP>& dsp);

    /// @name general post-init system functions
    //@{

public slots:
    void update ();

public:

    void set3DSettings (float dopplerScale, float distanceFactor,
                        float rolloffScale);
    void get3DSettings (float* dopplerScale, float* distanceFactor,
                        float* rolloffScale);

    void set3DNumListeners (int nbListeners);

    void set3DListenerAttributes (int listener,
                                  const float* pos,
                                  const float* vel,
                                  const float* forward,
                                  const float* up);
    //@}

public:
    static FMOD_RESULT F_CALLBACK callback (
        FMOD_SYSTEM* channel,
        FMOD_SYSTEM_CALLBACKTYPE type,
        void* command_data1, void* command_data2);

private:
    struct Private;
    QScopedPointer<Private> d;
};
} // namespace QtFMOD

// vim: sw=4
