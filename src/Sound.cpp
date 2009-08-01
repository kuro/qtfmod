
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
