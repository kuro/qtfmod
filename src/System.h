
/**
 * @file System.h
 * @brief System definition
 */

#pragma once

#include <QObject>
#include <QSharedDataPointer>

struct SystemPrivate;

class System : public QObject
{
    Q_OBJECT

public:
    System ();
    ~System ();

    int error () const;
    QString errorString () const;

private:
    QSharedDataPointer<SystemPrivate> d;
};

// vim: sw=4
