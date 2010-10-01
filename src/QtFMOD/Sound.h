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
 * @file Sound.h
 * @brief Sound definition
 */

#pragma once

#include <QObject>
#include <QHash>

#include <fmod.hpp>

#include "Tag.h"

namespace QtFMOD
{
class Sound : public QObject
{
    Q_OBJECT

public:
    virtual ~Sound ();

    int error () const;
    QString errorString () const;

    bool isNull () const;

    operator FMOD::Sound*& () const;

    FMOD::Sound* internalPointer () const;

    QHash<QString, Tag> tags (int* nbUpdated) const;

private:
    Sound (FMOD::Sound* fsound);
    friend class System;

private:
    struct Private;
    QScopedPointer<Private> d;
};
} // namespace QtFMOD

// vim: sw=4
