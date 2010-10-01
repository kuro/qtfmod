
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

private:
    struct Private;
    QSharedDataPointer<Private> d;

    void setUpdated (bool updated);
    friend class Sound;
};
} // namespace QtFMOD
