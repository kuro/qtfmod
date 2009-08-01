
/**
 * @file System.h
 * @brief System definition
 */

#pragma once

#include <QObject>
#include <QSharedDataPointer>
#include <QUuid>

#include <fmod.hpp>

#include "Sound.h"
#include "Channel.h"

namespace QtFMOD
{
class System : public QObject
{
    Q_OBJECT

public:
    System ();
    ~System ();

    int error () const;
    QString errorString () const;

    int numDrivers () const;
    void driverInfo (int id, QString& name, QUuid& uuid) const;

    void init (int max_channels, FMOD_INITFLAGS flags = FMOD_INIT_NORMAL);
    void close ();

    operator FMOD::System* () const;

    // sound
    Sound* createSound (const QString& name,
                        FMOD_MODE mode = FMOD_DEFAULT,
                        FMOD_CREATESOUNDEXINFO* exinfo = NULL
                        );
    Sound* createStream (const QString& name,
                         FMOD_MODE mode = FMOD_DEFAULT,
                         FMOD_CREATESOUNDEXINFO* exinfo = NULL
                         );

    Channel* playSound (FMOD_CHANNELINDEX channel_id, Sound* sound,
                        bool paused = false);

public slots:
    void update ();

public:
    FMOD_RESULT callback (FMOD_SYSTEM* channel,
                          FMOD_SYSTEM_CALLBACKTYPE type,
                          void* command_data1, void* command_data2);

private:
    struct Private;
    Private* d;
};
} // namespace QtFMOD

// vim: sw=4
