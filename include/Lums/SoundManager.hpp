/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    SoundManager.hpp                               oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_SOUND_MANAGER_HPP
#define LUMS_SOUND_MANAGER_HPP

#include <vector>
#include <string>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace lm
{

    class SoundManager
    {
    public:

        SoundManager();
        static SoundManager&    get();
        ~SoundManager();

    private:

        void        openContext();
        void        getDevices();
        void        setDevice(ALCchar* name);

        std::vector<const ALCchar*>    _devices;
        ALCdevice*                     _currentDevice;
        ALCcontext*                    _currentContext;
        static SoundManager*           _singleton;
    };

}

#endif