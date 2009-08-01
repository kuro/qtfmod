
/**
 * @file fileio.h
 * @brief fileio definition
 */

#pragma once

FMOD_RESULT F_CALLBACK qtfmod_file_open (
    const char *name, int unicode, unsigned int *filesize,
    void **handle, void **userdata);

FMOD_RESULT F_CALLBACK qtfmod_file_read (
    void *handle, void *buffer,
    unsigned int sizebytes, unsigned int *bytesread, void *userdata);

FMOD_RESULT F_CALLBACK qtfmod_file_seek (
    void *handle, unsigned int pos, void *userdata);

FMOD_RESULT F_CALLBACK qtfmod_file_close (void *handle, void *userdata);

// vim: sw=4
