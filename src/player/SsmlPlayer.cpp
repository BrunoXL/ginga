/* Copyright (C) 2006-2017 PUC-Rio/Laboratorio TeleMidia

This file is part of Ginga (Ginga-NCL).

Ginga is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Ginga is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License
along with Ginga.  If not, see <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include <cstdlib>

#include "player/SsmlPlayer.h"

#include "player/PlayersComponentSupport.h"

// TODO: Develop a ginga common audio system and integrate this player in it.
// The eSpeak header
#include <espeak/speak_lib.h>

// size of the max read by the voice syntetizer
#define MAX_READ 100000

bool isRunning;
bool terminateSpeak;

// Callback method which delivers the synthetized audio samples and the events.
static int SynthCallback(short *wav, int numsamples, espeak_EVENT *events)
{
    if (terminateSpeak == true)
        return 1;

    return 0;
}


namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace player {

    SsmlPlayer::SsmlPlayer(GingaScreenID screenId, string mrl) :
        Thread(), Player(screenId, mrl) {

    }

    SsmlPlayer::~SsmlPlayer() {

    }

    void SsmlPlayer::setFile(string mrl) {
        clog << "SsmlPlayer::setFile!! " << endl;

        if (mrl == "" || !fileExists(mrl)) {
            clog << "SsmlPlayer::setFile Warning! File not found: '";
            clog << mrl << "'" << endl;
            return;
        }

        if (mrl.length() > 5) {
            string fileType;
	
            this->mrl = mrl;
            fileType = this->mrl.substr(this->mrl.length() - 5, 5);
            if (fileType != ".ssml") {
                clog << "SsmlPlayer::loadFile Warning! Unknown file ";
                clog << "type for: '" << this->mrl << "'" << endl;
            }

        } else {
            clog << "SsmlPlayer::loadFile Warning! Unknown extension ";
            clog << "type for: '" << mrl << "'" << endl;
        }
    }

    // This method is the most important one. It sets up the audio synthesizer, the
    // output audio device, reads the input SSML file and calls the apropriate
    // methods to perform the audio synthesis and playback.
    void SsmlPlayer::loadSsml() {
        espeak_AUDIO_OUTPUT outType = AUDIO_OUTPUT_SYNCH_PLAYBACK;
        espeak_POSITION_TYPE pType = POS_CHARACTER;
        espeak_VOICE voiceType;
        espeak_ERROR errType = EE_OK;
        int sampleRate = 0;
        clog << "SsmlPlayer::loadSsml!! " << endl;

        voiceType.name = NULL;
        // TODO: Hardcoded to Brazilian Portuguese
        voiceType.languages = "pt-br";
        voiceType.gender = 0;
        voiceType.age = 0;
        voiceType.variant = 0;
       
        ifstream fis;

        fis.open((this->mrl).c_str(), ifstream::in);

        if (!fis.is_open() && (mrl != "" || content == "")) {
            clog << "SsmlPlayer::loadFile Warning! can't open input ";
            clog << "file: '" << this->mrl << "'" << endl;
            return;
        }

        if (isRunning == true) {
            terminateSpeak = true;
            while (isRunning == true)
                sleep (1);
        }
       
        sampleRate = espeak_Initialize(outType, MAX_READ, NULL, 0);
        isRunning = true;

        errType = espeak_SetVoiceByProperties(&voiceType);
       
        espeak_SetSynthCallback(SynthCallback);

        string line;
        do {

            if (terminateSpeak == true)
                break;
           
            getline (fis, line);
            errType = espeak_Synth(line.c_str(),
                     line.length(),
                     0,
                     pType,
                     0,
                     espeakSSML,
                     NULL,
                     NULL);

        } while (!fis.eof());

        fis.close();
       
        espeak_Synchronize();
        espeak_Terminate();

        if (terminateSpeak == false)
            notifyPlayerListeners(PL_NOTIFY_STOP, "");
       
        terminateSpeak = false;
        isRunning = false;
      

    }

    bool SsmlPlayer::play() {
        clog << "SsmlPlayer::play ok" << endl;

        bool ret = Player::play();
        Thread::startThread();

        return ret;

    }

    void SsmlPlayer::stop() {
        clog << "SsmlPlayer::stop ok" << endl;

        Player::stop();
    }


    void SsmlPlayer::resume() {
        SsmlPlayer::play();
    }


    void SsmlPlayer::setPropertyValue(string name, string value) {
        Player::setPropertyValue(name, value);
       
    }

    void SsmlPlayer::run() {
        clog << "SsmlPlayer::run thread created!" << endl;
        loadSsml();
       
    }

   
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::core::player::IPlayer*
createSsmlPlayer(
    GingaScreenID screenId, const char* mrl, bool hasVisual) {
   
    return (new ::br::pucrio::telemidia::ginga::core::player::
            SsmlPlayer(screenId, (string)mrl));
}

extern "C" void destroySsmlPlayer(
    ::br::pucrio::telemidia::ginga::core::player::IPlayer* p) {
   
    delete p;
}