
/**
 * @file Channel.h
 * @brief Channel definition
 */

#pragma once

#include <QObject>
#include <QSharedDataPointer>

#include <fmod.hpp>

namespace QtFMOD
{
class Channel : public QObject
{
    Q_OBJECT

public:
    ~Channel ();

    Channel (const Channel& other);
    Channel& operator= (const Channel& other);

    int error () const;
    QString errorString () const;

    bool isNull () const;

    bool isPlaying () const;

    operator FMOD::Channel* () const;

signals:
    void soundEnded ();

private:
    Channel (FMOD::Channel* fsnd);
    friend class System;

public:
    FMOD_RESULT callback (FMOD_CHANNEL* channel,
                          FMOD_CHANNEL_CALLBACKTYPE type,
                          void* command_data1, void* command_data2);

private:
    struct Private;
    Private* d;
};
} // namespace QtFMOD


// vim: sw=4
