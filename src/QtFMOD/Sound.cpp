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

#include "Tag.h"

#include <fmod_errors.h>

#include <QDebug>

using namespace QtFMOD;

struct Sound::Private
{
    mutable FMOD_RESULT fr;
    FMOD::Sound* fsound;
    QHash<QString, Tag> tags;

    Private (FMOD::Sound* fsound) :
        fr(FMOD_OK),
        fsound(fsound)
    {
    }
    ~Private ()
    {
        fr = fsound->release();
    }
};

/**
 * @note This is intended to be wrapped in a shared pointer, without a parent.
 */
Sound::Sound (FMOD::Sound* fsound) :
    QObject(),
    d(new Private(fsound))
{
}

Sound::~Sound ()
{
}

FMOD::Sound* Sound::internalPointer () const
{
    return d->fsound;
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

Sound::operator FMOD::Sound*& () const
{
    return d->fsound;
}

/**
 * Performs an update before returning the accumulated tags.
 *
 * @note Redundant tags will be overwritten.
 */
QHash<QString, Tag> Sound::tags (int* nbUpdated) const
{
    if (nbUpdated) {
        d->fr = d->fsound->getNumTags(NULL, nbUpdated);
        if (d->fr != FMOD_OK) {
            return d->tags;
        }
    }

    // manually clear updated flags of existing tags
    QHashIterator<QString, QtFMOD::Tag> it (d->tags);
    while (it.hasNext()) {
        it.next();
        const_cast<Tag&>(it.value()).setUpdated(false);
    }

    // ask fmod for updated tags
    FMOD_TAG ftag;
    while ((d->fr = d->fsound->getTag(NULL, -1, &ftag)) == FMOD_OK) {
        Tag tag (ftag);
        d->tags.insert(tag.name(), tag);
    }

    if (d->fr == FMOD_ERR_TAGNOTFOUND) {
        d->fr = FMOD_OK;
    } else {
        qWarning("unexpected fmod result after reading tags: %d", d->fr);
    }

    return d->tags;
}

// vim: sw=4
