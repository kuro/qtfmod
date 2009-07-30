
/**
 * @file System.cpp
 * @brief System implementation
 */

#include "System.moc"

#include <fmod.hpp>
#include <fmod_errors.h>

#include <QDebug>

struct SystemPrivate : public QSharedData
{
    mutable FMOD_RESULT fr;
    FMOD::System* sys;

    SystemPrivate () :
        sys(NULL)
    {
        fr = FMOD::System_Create(&sys);
    }
    ~SystemPrivate ()
    {
        fr = sys->release();
    }
};

System::System () :
    d(new SystemPrivate)
{
}

System::~System ()
{
}

int System::error () const
{
    return d->fr;
}

QString System::errorString () const
{
    return FMOD_ErrorString(d->fr);
}

// vim: sw=4
