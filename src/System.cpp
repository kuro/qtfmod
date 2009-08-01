
/**
 * @file System.cpp
 * @brief System implementation
 */

#include "System.moc"
#include "fileio.h"

#include <fmod_errors.h>
#include <QFile>

#include <QDebug>
#include <QCoreApplication>

using namespace QtFMOD;

static
FMOD_RESULT system_callback (FMOD_SYSTEM* fsystem,
                             FMOD_SYSTEM_CALLBACKTYPE type,
                             void* command_data1, void* command_data2)
{
    FMOD_RESULT fr;
    System* system = NULL;

    fr = FMOD_System_GetUserData(fsystem, (void**)&system);

    if (fr != FMOD_OK) {
        qCritical() << FMOD_ErrorString(fr);
        return FMOD_OK;
    }

    if (system) {
        return system->callback(fsystem, type, command_data1, command_data2);
    } else {
        qCritical() << "user data for system is undefined";
        return FMOD_OK;
    }
}

#define make_uuid(guid)                                                     \
    QUuid(guid.Data1, guid.Data2, guid.Data3,                               \
          guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],       \
          guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]        \
         );

struct System::Private : public QSharedData
{
    mutable FMOD_RESULT fr;
    FMOD::System* fsystem;

    Private () :
        fsystem(NULL)
    {
    }
    ~Private ()
    {
        fr = fsystem->release();
    }
};

System::System () :
    d(new System::Private)
{
    d->fr = FMOD::System_Create(&d->fsystem);
    d->fsystem->setUserData(this);
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
    d->fr = d->fsystem->getNumDrivers(&num);
    return num;
}

void System::driverInfo (int id, QString& name, QUuid& uuid) const
{
    char name_buf[1024];
    FMOD_GUID guid;

    d->fr = d->fsystem->getDriverInfo(id, name_buf, sizeof(name_buf), &guid);

    name = name_buf;
    uuid = make_uuid(guid);
}

System::operator FMOD::System* () const
{
    return d->fsystem;
}

void System::init (int max_channels, FMOD_INITFLAGS flags)
{
    d->fr = d->fsystem->init(max_channels, flags, NULL);

    d->fsystem->setCallback(system_callback);
}

void System::close ()
{
    d->fr = d->fsystem->close();
}

void System::update ()
{
    d->fr = d->fsystem->update();
}

#include <QSharedPointer>
class Blah : public QObject
{
public:
    Blah () { qDebug() << "blah in"; }
    ~Blah () { qDebug() << "blah out"; }
};

Sound* System::createSound (const QString& name, FMOD_MODE mode,
                            FMOD_CREATESOUNDEXINFO* exinfo)
{
    FMOD_CREATESOUNDEXINFO default_info;

    QSharedPointer<Blah> blah (new Blah);

    if (exinfo == NULL) {
        exinfo = &default_info;
        memset(&default_info, 0, sizeof(default_info));
        default_info.cbsize = sizeof(default_info);

        if (QFile::exists(name)) {
            qDebug() << "using qt io";
            default_info.useropen  = qtfmod_file_open;
            default_info.userclose = qtfmod_file_close;
            default_info.userread  = qtfmod_file_read;
            default_info.userseek  = qtfmod_file_seek;
        }
    }

    FMOD::Sound* fsnd = NULL;
    d->fr = d->fsystem->createSound(name.toUtf8(), mode, exinfo, &fsnd);
    return new Sound(fsnd);
}

Sound* System::createStream (const QString& name, FMOD_MODE mode,
                             FMOD_CREATESOUNDEXINFO* exinfo)
{
    FMOD_CREATESOUNDEXINFO default_info;

    if (exinfo == NULL) {
        exinfo = &default_info;
        memset(&default_info, 0, sizeof(default_info));
        default_info.cbsize = sizeof(default_info);

        if (QFile::exists(name)) {
            qDebug() << "using qt io";
            default_info.useropen  = qtfmod_file_open;
            default_info.userclose = qtfmod_file_close;
            default_info.userread  = qtfmod_file_read;
            default_info.userseek  = qtfmod_file_seek;
        }
    }

    FMOD::Sound* fsnd = NULL;
    d->fr = d->fsystem->createStream(name.toUtf8(), mode, exinfo, &fsnd);
    return new Sound(fsnd);
}

Channel* System::playSound (FMOD_CHANNELINDEX channel_id, Sound* sound,
                            bool paused)
{
    FMOD::Channel* fchan = NULL;
    d->fr = d->fsystem->playSound(channel_id, *sound, paused, &fchan);
    return new Channel(fchan);
}

FMOD_RESULT System::callback (FMOD_SYSTEM* system,
                              FMOD_SYSTEM_CALLBACKTYPE type,
                              void* command_data1,
                              void* command_data2)
{
    Q_UNUSED(system);
    Q_UNUSED(command_data1);
    Q_UNUSED(command_data2);

    Q_ASSERT(system == reinterpret_cast<FMOD_SYSTEM*>(d->fsystem));

    switch (type) {
    case FMOD_SYSTEM_CALLBACKTYPE_DEVICELISTCHANGED:
        qDebug() << "callback: system: devicelistchanged";
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_MEMORYALLOCATIONFAILED:
        qDebug() << "callback: system: memoryallocationfailed";
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_THREADCREATED:
        qDebug() << "callback: system: threadcreated";
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_BADDSPCONNECTION:
        qDebug() << "callback: system: baddspconnection";
        break;
    case FMOD_SYSTEM_CALLBACKTYPE_BADDSPLEVEL:
        qDebug() << "callback: system: baddsplevel";
        break;
    default:
        qCritical() << "callback: system: unsupported callback type";
        break;
    }

    return FMOD_OK;
}

// vim: sw=4
