/*
 * Copyright 2009 Blanton Black
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
