
/**
 * @file fmod.h
 * @brief fmod definition
 */

#pragma once

#include <QObject>

class TestFMOD : public QObject
{
    Q_OBJECT

private slots:
    void initialize ();
    void drivers ();
};


// vim: sw=4
