/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
*******************************************************************************/

#include "player/ImagePlayer.h"

#include "player/PlayersComponentSupport.h"

#if HAVE_IC
#include "ic/IInteractiveChannelManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::ic;
#endif //HAVE_IC

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace player {
	ImagePlayer::ImagePlayer(GingaScreenID screenId, string mrl) :
			Player(screenId, mrl) {

		IGingaLocatorFactory* glf = NULL;
		string path, name, clientPath, newMrl;
		bool resolved = false;

		if (mrl.substr(0, 7) == "http://" ||
				mrl.substr(0, 8) == "https://") {

#if HAVE_IC
			IInteractiveChannelManager* icm;

			icm = InteractiveChannelManager::getInstance();
			IInteractiveChannel* ic = icm->createInteractiveChannel(mrl);
			newMrl = itos((long int)this);
			ic->setSourceTarget(newMrl);
			ic->reserveUrl(mrl, NULL, "GingaNCL/0.13.6");
			ic->performUrl();

			icm->releaseInteractiveChannel(ic);

			mrl = newMrl;
#endif //HAVE_IC
		}

		if (fileExists(mrl)) {
			provider = dm->createImageProvider(myScreen, mrl.c_str());

		} else {
			if (!SystemCompat::isAbsolutePath(mrl)) {
				newMrl = SystemCompat::getUserCurrentPath() + mrl;
				if (fileExists(newMrl)) {
					resolved = true;
					mrl = newMrl;
					newMrl = "";
				}
			}

			if (!resolved) {
				if (mrl.find(SystemCompat::getIUriD()) != std::string::npos) {
					path = mrl.substr(0, mrl.find_last_of(SystemCompat::getIUriD()));
					name = mrl.substr(
							mrl.find_last_of(SystemCompat::getIUriD()) + 1,
							mrl.length() - mrl.find_last_of(SystemCompat::getIUriD()));

				} else if (mrl.find(SystemCompat::getFUriD()) !=
						std::string::npos) {

					path = mrl.substr(0, mrl.find_last_of(
							SystemCompat::getFUriD()));

					name = mrl.substr(
							mrl.find_last_of(SystemCompat::getFUriD()) + 1,
							mrl.length() - mrl.find_last_of(
									SystemCompat::getFUriD()));
				}

				glf = GingaLocatorFactory::getInstance();

				if (glf != NULL) {
					clog << "ImagePlayer trying to find '" << mrl << "'";
					clog << endl;
					clientPath = glf->getLocation(path);
					newMrl = clientPath + name;
					clog << "ImagePlayer found newMrl = '" << newMrl;
					clog << "'" << endl;

					if (fileExists(newMrl)) {
						provider = dm->createImageProvider(
								myScreen, newMrl.c_str());

					} else {
						provider = 0;
						clog << "ImagePlayer::ImagePlayer Warning! File ";
						clog << " Not Found: '" << newMrl.c_str();
						clog << "'" << endl;
					}

				} else {
					provider = 0;
					clog << "ImagePlayer::ImagePlayer Warning! ";
					clog << "GLF Component NOT";
					clog << " Found!" << endl;
				}
			}
		}

		if (provider != 0) {
			surface = prepareSurface(provider, mrl);
		}
	}

	ImagePlayer::~ImagePlayer() {
		if (provider != 0) {
			dm->releaseImageProvider(myScreen, provider);
		}
	}

	bool ImagePlayer::play() {
		if (provider == 0) {

			return false;
		}

		dm->playProviderOver(provider, surface);
		return Player::play();
	}

	void ImagePlayer::stop() {
		Player::stop();
	}

	void ImagePlayer::resume() {
		ImagePlayer::play();
	}

	void ImagePlayer::setPropertyValue(string name, string value) {
		//TODO: set brightness, rotate...
		//refresh changes
		GingaWindowID win;

		if (surface != 0) {
			win = dm->getSurfaceParentWindow(surface);
			if (win != 0) {
				dm->renderWindowFrom(myScreen, win, surface);
			}
		}

		Player::setPropertyValue(name, value);
	}

	GingaSurfaceID ImagePlayer::prepareSurface(
			GingaProviderID provider, string mrl) {

		GingaSurfaceID renderedSurface = 0;

		renderedSurface = dm->createSurfaceFrom(myScreen, 0);
		dm->playProviderOver(provider, renderedSurface);

		return renderedSurface;
	}
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::core::player::IPlayer*
		createImagePlayer(GingaScreenID screenId, const char* mrl) {

	return new ::br::pucrio::telemidia::ginga::core::player::ImagePlayer(
			screenId, mrl);
}

extern "C" void destroyImagePlayer(
		::br::pucrio::telemidia::ginga::core::player::IPlayer* p) {

	delete p;
}
