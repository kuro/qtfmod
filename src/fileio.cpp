
/**
 * @file fileio.cpp
 * @brief fileio implementation
 */

#include <fmod.h>

#include "fileio.h"

#include <QFile>
#include <QDebug>

FMOD_RESULT F_CALLBACK qtfmod_file_open (
    const char *name, int unicode, unsigned int *filesize,
    void **handle, void **userdata)
{
    Q_UNUSED(unicode);
    Q_UNUSED(userdata);

    QFile* file = new QFile (name);

    file->open(QIODevice::ReadOnly);

    if (file->error() != QFile::NoError) {
        qWarning() << file->errorString();
        return FMOD_ERR_FILE_BAD;
    }

    *filesize = file->size();
    *handle = file;

    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK qtfmod_file_read (
    void *handle, void *buffer,
    unsigned int sizebytes, unsigned int *bytesread, void *userdata)
{
    Q_UNUSED(userdata);

    QFile* file = qobject_cast<QFile*>(static_cast<QObject*>(handle));
    Q_ASSERT(file);

    if (file->atEnd()) {
        return FMOD_ERR_FILE_EOF;
    }

    qint64 result = file->read(static_cast<char*>(buffer), sizebytes);

    if (result < 0) {
        qWarning() << file->errorString();
        return FMOD_ERR_FILE_EOF;
    }

    if (file->error() != QFile::NoError) {
        qWarning() << file->errorString();
        return FMOD_ERR_FILE_BAD;
    }

    *bytesread = result;

    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK qtfmod_file_seek (
    void *handle, unsigned int pos, void *userdata)
{
    Q_UNUSED(userdata);

    QFile* file = qobject_cast<QFile*>(static_cast<QObject*>(handle));
    Q_ASSERT(file);

    if (!file->seek(pos)) {
        return FMOD_ERR_FILE_COULDNOTSEEK;
    }

    if (file->error() != QFile::NoError) {
        qWarning() << file->errorString();
        return FMOD_ERR_FILE_BAD;
    }

    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK qtfmod_file_close (void *handle, void *userdata)
{
    Q_UNUSED(userdata);

    QFile* file = qobject_cast<QFile*>(static_cast<QObject*>(handle));
    Q_ASSERT(file);

    file->close();

    if (file->error() != QFile::NoError) {
        qWarning() << file->errorString();
    }

    delete file;

    return FMOD_OK;
}


// vim: sw=4
