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
 * @file DSP.cpp
 * @brief DSP implementation
 */

#include "DSP.moc"

#include <fmod.hpp>
#include <fmod_errors.h>

using namespace QtFMOD;

struct DSP::Private
{
    mutable FMOD_RESULT fr;
    FMOD::DSP* fdsp;

    Private (FMOD::DSP* fdsp) :
        fr(FMOD_OK),
        fdsp(fdsp)
    {
    }
    ~Private ()
    {
        fdsp->release();
    }
};

DSP::DSP (FMOD::DSP* fdsp) :
    QObject(),
    d(new Private(fdsp))
{
    d->fdsp->setUserData(this);
}

DSP::~DSP ()
{
}

FMOD::DSP* DSP::internalPointer () const
{
    return d->fdsp;
}

bool DSP::isNull () const
{
    return (d->fdsp == NULL);
}

int DSP::error () const
{
    return d->fr;
}

QString DSP::errorString () const
{
    return FMOD_ErrorString(d->fr);
}

