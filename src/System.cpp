
/**
 * @file System.cpp
 * @brief System implementation
 */

#include "System.moc"

#include <fmod_errors.h>

#include <QDebug>

using namespace QtFMOD;

#define make_uuid(guid)                                                     \
    QUuid(guid.Data1, guid.Data2, guid.Data3,                               \
          guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],       \
          guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]        \
         );

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

int System::numDrivers () const
{
    int num;
    d->fr = d->sys->getNumDrivers(&num);
    return num;
}

void System::driverInfo (int id, QString& name, QUuid& uuid) const
{
    char name_buf[1024];
    FMOD_GUID guid;

    d->fr = d->sys->getDriverInfo(id, name_buf, sizeof(name_buf), &guid);

    name = name_buf;
    uuid = make_uuid(guid);
}

System::operator FMOD::System* () const
{
    return d->sys;
}

void System::init (int max_channels, FMOD_INITFLAGS flags)
{
    d->fr = d->sys->init(max_channels, flags, NULL);
}

void System::close ()
{
    d->fr = d->sys->close();
}

void System::update ()
{
    d->fr = d->sys->update();
}

// vim: sw=4
