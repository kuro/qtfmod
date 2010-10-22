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
#include <QImage>
#include <QBuffer>
#include <QDebug>

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
 * Removes null characters from the end of a string.
 *
 * @todo Consider performance.
 */
static inline
QString sanitize (const QString& str)
{
    QString retval (str);
    while (retval.endsWith(QChar('\0'))) {
        retval.chop(1);
    }
    return retval;
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
        retval = sanitize(QString::fromLocal8Bit((char*)d->ftag.data,
                                                 d->ftag.datalen));
        break;
    case FMOD_TAGDATATYPE_STRING_UTF16:
        retval = sanitize(QString::fromUtf16((ushort*)d->ftag.data,
                                             d->ftag.datalen));
        break;
    //case FMOD_TAGDATATYPE_STRING_UTF16BE:
    //    break;
    case FMOD_TAGDATATYPE_STRING_UTF8:
        retval = sanitize(QString::fromUtf8((char*)d->ftag.data,
                                            d->ftag.datalen));
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

QImage Tag::toImage () const
{
    if (name() == "APIC") {
        QBuffer dev;
        dev.setData(data());
        dev.open(QIODevice::ReadOnly);

        quint8 textEncoding;
        QString mimeType;
        quint8 pictureType;
        QString description;
        QByteArray pictureData;

        quint8 c;

        dev.getChar((char*)&textEncoding);

        forever {
            dev.getChar((char*)&c);
            if (c == 0) {
                break;
            }
            mimeType.append(c);
        }

        dev.getChar((char*)&pictureType);

        forever {
            dev.getChar((char*)&c);
            if (c == 0) {
                break;
            }
            description.append(c);
        }

        pictureData = dev.readAll();

        dev.close();

        return QImage::fromData(pictureData);
    } else {
        qWarning() << "tag" << name() << "is not an image";
        return QImage();
    }
}
