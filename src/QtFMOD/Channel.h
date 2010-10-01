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

#include <fmod.hpp>

namespace QtFMOD
{
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

    operator FMOD::Channel* () const;

signals:
    void soundEnded ();

private:
    Channel (FMOD::Channel* fsnd, QObject* parent = NULL);
    friend class System;

public:
    FMOD_RESULT callback (FMOD_CHANNEL* channel,
                          FMOD_CHANNEL_CALLBACKTYPE type,
                          void* command_data1, void* command_data2);

private:
    struct Private;
    QScopedPointer<Private> d;
};
} // namespace QtFMOD


// vim: sw=4
