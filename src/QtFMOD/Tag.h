/*
 * Copyright 2010 Blanton Black
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
 * @file Tag.h
 * @brief Tag definition
 */

#pragma once

#include <QSharedDataPointer>
#include <QVariant>

#include <fmod.h>

namespace QtFMOD
{
class Tag
{
public:
    Tag ();
    Tag (FMOD_TAG tag);
    Tag (const Tag& other);
    virtual ~Tag ();

    Tag& operator= (const Tag& other);

    QString name () const;
    QByteArray data () const;

    QVariant value () const;

    bool updated () const;

    QImage toImage () const;

private:
    struct Private;
    QSharedDataPointer<Private> d;

    void setUpdated (bool updated);
    friend class Sound;
};
} // namespace QtFMOD
