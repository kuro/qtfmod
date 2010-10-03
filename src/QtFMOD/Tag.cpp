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
 * @file Tag.cpp
 * @brief Tag implementation
 */

#include "Tag.h"

#include <QString>

using namespace QtFMOD;

struct Tag::Private : QSharedData
{
    FMOD_TAG ftag;

    Private (FMOD_TAG ftag) :
        ftag(ftag)
    {
    }
};

Tag::Tag () :
    d(new Private(FMOD_TAG()))
{
}

Tag::Tag (FMOD_TAG tag) :
    d(new Private(tag))
{
}

Tag::Tag (const Tag& other) :
    d(other.d)
{

}

Tag::~Tag ()
{
}

/**
 * @remarks A bit of a hack so Sound can avoid asking for all tags again.
 */
void Tag::setUpdated (bool updated)
{
    d->ftag.updated = updated;
}

Tag& Tag::operator= (const Tag& other)
{
    d = other.d;
    return *this;
}

QString Tag::name () const
{
    return QString::fromLocal8Bit(d->ftag.name);
}

QByteArray Tag::data () const
{
    return QByteArray::fromRawData((char*)d->ftag.data, d->ftag.datalen);
}

/**
 * @todo resolve unsupported types
 * @todo check tag data lengths
 */
QVariant Tag::value () const
{
    QVariant retval;
    switch (d->ftag.datatype) {
    case FMOD_TAGDATATYPE_BINARY:
        retval = QByteArray::fromRawData((char*)d->ftag.data, d->ftag.datalen);
        break;
    //case FMOD_TAGDATATYPE_INT:
    //    break;
    //case FMOD_TAGDATATYPE_FLOAT:
    //    break;
    case FMOD_TAGDATATYPE_STRING:
        retval = QString::fromLocal8Bit((char*)d->ftag.data, d->ftag.datalen);
        break;
    case FMOD_TAGDATATYPE_STRING_UTF16:
        retval = QString::fromUtf16((ushort*)d->ftag.data, d->ftag.datalen);
        break;
    //case FMOD_TAGDATATYPE_STRING_UTF16BE:
    //    break;
    case FMOD_TAGDATATYPE_STRING_UTF8:
        retval = QString::fromUtf8((char*)d->ftag.data, d->ftag.datalen);
        break;
    //case FMOD_TAGDATATYPE_CDTOC:
    //    break;
    default:
        qWarning("unsupported tag data type: %d", d->ftag.datatype);
        retval = QByteArray::fromRawData((char*)d->ftag.data, d->ftag.datalen);
        break;
    }
    return retval;
}

bool Tag::updated () const
{
    return d->ftag.updated;
}
