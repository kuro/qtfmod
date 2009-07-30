
/**
 * @file System.h
 * @brief System definition
 */

#pragma once

#include <QObject>
#include <QSharedDataPointer>
#include <QUuid>

#include <fmod.hpp>

struct SystemPrivate;

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

public slots:
    void update ();

private:
    QSharedDataPointer<SystemPrivate> d;
};

// vim: sw=4
