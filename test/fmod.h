
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
    void play_sound ();
};


// vim: sw=4
