
/**
 * @file Channel.cpp
 * @brief Channel implementation
 */

#include "Channel.moc"

#include <fmod_errors.h>

#include <QDebug>

using namespace QtFMOD;

static
FMOD_RESULT channel_callback (FMOD_CHANNEL* fchannel,
                              FMOD_CHANNEL_CALLBACKTYPE type,
                              void* command_data1, void* command_data2)
{
    FMOD_RESULT fr;
    Channel* channel = NULL;

    fr = FMOD_Channel_GetUserData(fchannel, (void**)&channel);

    if (fr != FMOD_OK) {
        qCritical() << FMOD_ErrorString(fr);
        return FMOD_OK;
    }

    if (channel) {
        return channel->callback(fchannel, type, command_data1, command_data2);
    } else {
        qCritical() << "user data for channel is undefined";
        return FMOD_OK;
    }
}

struct Channel::Private : public QSharedData
{
    mutable FMOD_RESULT fr;
    FMOD::Channel* fchannel;

    Private () :
        fchannel(NULL)
    {
    }
    ~Private ()
    {
    }
};

Channel::Channel (FMOD::Channel* fchannel) :
    d(new Private)
{
    d->fchannel = fchannel;

    d->fchannel->setUserData(this);
    d->fchannel->setCallback(channel_callback);
}

Channel::~Channel ()
{
}

bool Channel::isNull () const
{
    return (d->fchannel == NULL);
}

int Channel::error () const
{
    return d->fr;
}

QString Channel::errorString () const
{
    return FMOD_ErrorString(d->fr);
}

Channel::operator FMOD::Channel* () const
{
    return d->fchannel;
}

bool Channel::isPlaying () const
{
    bool v;
    d->fr = d->fchannel->isPlaying(&v);
    return v;
}

FMOD_RESULT Channel::callback (FMOD_CHANNEL* channel,
                               FMOD_CHANNEL_CALLBACKTYPE type,
                               void* command_data1,
                               void* command_data2)
{
    Q_UNUSED(channel);
    Q_UNUSED(command_data1);
    Q_UNUSED(command_data2);

    Q_ASSERT(channel == reinterpret_cast<FMOD_CHANNEL*>(d->fchannel));

    switch (type) {
    case FMOD_CHANNEL_CALLBACKTYPE_END:
        qDebug() << "callback: channel: end";
        emit soundEnded();
        break;
    case FMOD_CHANNEL_CALLBACKTYPE_VIRTUALVOICE:
        qDebug() << "callback: channel: virtual voice";
        break;
    case FMOD_CHANNEL_CALLBACKTYPE_SYNCPOINT:
        qDebug() << "callback: channel: syncpoint";
        break;
    case FMOD_CHANNEL_CALLBACKTYPE_OCCLUSION:
        qDebug() << "callback: channel: occlusion";
        break;
    default:
        qCritical() << "callback: channel: unsupported callback type";
        break;
    }

    return FMOD_OK;
}

// vim: sw=4
