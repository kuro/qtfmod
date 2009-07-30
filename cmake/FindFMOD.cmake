find_path(FMOD_INCLUDE_DIR
    NAMES fmod.h
    PATHS
    /opt/fmodex/api/inc
    /usr/include/fmodex
    "C:/Program Files/FMOD SoundSystem/FMOD Programmers API Win32/api/inc"
    )

find_path(FMOD_EVENT_INCLUDE_DIR
    NAMES fmod_event.h
    PATHS
    /opt/fmodex/fmoddesignerapi/api/inc
    "C:/Program Files/FMOD SoundSystem/FMOD Programmers API Win32/fmoddesignerapi/api/inc"
    )

if (UNIX)
    find_library(FMOD_LIBRARY
        NAMES fmodex
        PATHS
        /opt/fmodex/api/lib
        )
    find_library(FMOD_EVENT_LIBRARY
        NAMES fmodevent
        PATHS
        /opt/fmodex/fmoddesignerapi/api/lib
        )
    find_library(FMOD_EVENT_NET_LIBRARY
        NAMES fmodeventnet
        PATHS
        /opt/fmodex/fmoddesignerapi/api/lib
        )
elseif (MSVC)
    find_library(FMOD_LIBRARY
        NAMES fmodex_vc
        PATHS
        "C:/Program Files/FMOD SoundSystem/FMOD Programmers API Win32/api/lib"
        )
    find_library(FMOD_EVENT_LIBRARY
        NAMES fmod_event
        PATHS
        "C:/Program Files/FMOD SoundSystem/FMOD Programmers API Win32/fmoddesignerapi/api/lib"
        )
    find_library(FMOD_EVENT_NET_LIBRARY
        NAMES fmod_event_net
        PATHS
        "C:/Program Files/FMOD SoundSystem/FMOD Programmers API Win32/fmoddesignerapi/api/lib"
        )
endif ()

set(FMOD_INCLUDE_DIRS
    ${FMOD_INCLUDE_DIR}
    ${FMOD_EVENT_INCLUDE_DIR}
    )
set(FMOD_LIBRARIES
    ${FMOD_EVENT_NET_LIBRARY}
    ${FMOD_EVENT_LIBRARY}
    ${FMOD_LIBRARY}
    )

if (FMOD_LIBRARY AND FMOD_INCLUDE_DIR)
    set(FMOD_FOUND TRUE)
else (FMOD_LIBRARY AND FMOD_INCLUDE_DIR)
    set(FMOD_FOUND FALSE)
endif (FMOD_LIBRARY AND FMOD_INCLUDE_DIR)
