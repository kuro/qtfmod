/*
 * Copyright 2010 Blanton Black
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
 * @file DSPConnection.cpp
 * @brief DSPConnection implementation
 */

#include "DSPConnection.moc"

#include <fmod.hpp>
#include <fmod_errors.h>

using namespace QtFMOD;

struct DSPConnection::Private
{
    mutable FMOD_RESULT fr;
    FMOD::DSPConnection* fdspConnection;

    Private (FMOD::DSPConnection* fdspConnection) :
        fr(FMOD_OK),
        fdspConnection(fdspConnection)
    {
    }
};

DSPConnection::DSPConnection (FMOD::DSPConnection* fdspConnection) :
    QObject(),
    d(new Private(fdspConnection))
{
    d->fdspConnection->setUserData(this);
}

DSPConnection::~DSPConnection ()
{
}

FMOD::DSPConnection* DSPConnection::internalPointer () const
{
    return d->fdspConnection;
}

bool DSPConnection::isNull () const
{
    return (d->fdspConnection == NULL);
}

int DSPConnection::error () const
{
    return d->fr;
}

QString DSPConnection::errorString () const
{
    return FMOD_ErrorString(d->fr);
}

