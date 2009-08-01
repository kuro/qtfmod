
/**
 * @file Sound.h
 * @brief Sound definition
 */

#pragma once

#include <QObject>
#include <QSharedDataPointer>

#include <fmod.hpp>

namespace QtFMOD
{
class Sound : public QObject
{
    Q_OBJECT

public:
    ~Sound ();

    int error () const;
    QString errorString () const;

    bool isNull () const;

    operator FMOD::Sound* () const;

private:
    Sound (FMOD::Sound* fsound);
    friend class System;

private:
    struct Private;
    Private* d;
};
} // namespace QtFMOD

// vim: sw=4
