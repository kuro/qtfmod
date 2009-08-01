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
 * @file Sound.cpp
 * @brief Sound implementation
 */

#include "Sound.moc"

#include <fmod_errors.h>

#include <QDebug>

using namespace QtFMOD;

struct Sound::Private : public QSharedData
{
    mutable FMOD_RESULT fr;
    FMOD::Sound* fsound;

    Private () :
        fsound(NULL)
    {
    }
    ~Private ()
    {
        fr = fsound->release();
    }
};

Sound::Sound (FMOD::Sound* fsound) :
    d(new Private)
{
    d->fsound = fsound;
}

Sound::~Sound ()
{
}

bool Sound::isNull () const
{
    return (d->fsound == NULL);
}

int Sound::error () const
{
    return d->fr;
}

QString Sound::errorString () const
{
    return FMOD_ErrorString(d->fr);
}

Sound::operator FMOD::Sound* () const
{
    return d->fsound;
}

// vim: sw=4
