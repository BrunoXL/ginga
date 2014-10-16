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

#include "config.h"

#include "player/PlayersComponentSupport.h"
#include "player/ProgramAV.h"
#include "player/AVPlayer.h"

#include "util/functions.h"
using namespace ::br::pucrio::telemidia::util;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace player {
	ProgramAV::ProgramAV(GingaScreenID screenId) : Player(screenId, "") {
		currentPid       = -1;
		currentPlayer    = NULL;
		fullScreenBounds = "";
	}

	ProgramAV::~ProgramAV() {
		map<int, IPlayer*>::iterator i;

		i = players.begin();
		while (i != players.end()) {
			i->second->stop();
			if (i->second == currentPlayer) {
				currentPlayer = NULL;
			}
			delete i->second;
			++i;
		}

		players.clear();
		namePids.clear();
		playerBounds.clear();

		if (currentPlayer != NULL) {
			delete currentPlayer;
			currentPlayer = NULL;
		}
	}

	ProgramAV* ProgramAV::_instance = 0;

	ProgramAV* ProgramAV::getInstance(GingaScreenID screenId) {
		if (ProgramAV::_instance == NULL) {
			ProgramAV::_instance = new ProgramAV(screenId);
		}
		return ProgramAV::_instance;
	}

	void ProgramAV::release() {
		if (currentPlayer != NULL) {
			currentPlayer->stop();
		}

		if (_instance != NULL) {
			delete _instance;
			_instance = NULL;
		}
	}

	int64_t ProgramAV::getVPts() {
		if (currentPlayer != NULL) {
			return currentPlayer->getVPts();
		}

		return 0;
	}

	GingaSurfaceID ProgramAV::getSurface() {
		GingaSurfaceID pSur = 0;

		if (currentPlayer != NULL) {
			pSur = currentPlayer->getSurface();
			if (pSur != 0) {
				dm->setExternalHandler(pSur, true);
			}
		}

		return pSur;
	}

	void ProgramAV::addPidName(string name, int pid) {
		clog << "ProgramAV::addPidName '" << name << "' = '" << pid;
		clog << "'" << endl;
		namePids[name] = pid;
	}

	int ProgramAV::getPidByName(string name) {
		map<string, int>::iterator i;

		i = namePids.find(name);
		if (i != namePids.end()) {
			return i->second;
		}

		return -1;
	}

	void ProgramAV::forcePids(string pValue) {
		vector<string>* vals;
		string name;

		vals = split(pValue, ",");
		if (vals->size() == 3) {
			name = getNameFromMrl((*vals)[0]);
			setAVPid(name, util::stof((*vals)[1]), util::stof((*vals)[2]));
		}

		delete vals;
	}

	void ProgramAV::setAVPid(string name, int aPid, int vPid) {
		IPlayer* p;
		int pid;

		pid = getPidByName(name);
		if (pid < 0) {
			return;
		}

		p = getPlayer(pid);
		if (p != NULL) {
			clog << "ProgramAV::setAVPid";
			clog << " aPid = '" << aPid << "'";
			clog << " vPid = '" << vPid << "'";
			clog << endl;

			((AVPlayer*)p)->setAVPid(aPid, vPid);

		} else {
			clog << "ProgramAV::setAVPid Warning! Can't find name '";
			clog << name << "' to set '" << aPid << "' and '" << vPid << "'";
			clog << endl;
		}
	}

	string ProgramAV::getNameFromMrl(string mrl) {
		if (mrl.substr(0, 11) == "sbtvd-ts://") {
			if (mrl.find("dtv_channel.ts") != std::string::npos) {
				return "0";
			}
			return mrl.substr(11, mrl.length() - 11);
		}

		return mrl;
	}

	void ProgramAV::showPlayer(string mrl) {
		IPlayer* player;

		player = getPlayer(mrl);
		if (player != NULL) {
			player->setPropertyValue("show", "0xFF");
		}
	}

	void ProgramAV::hidePlayer(string mrl) {
		IPlayer* player;

		player = getPlayer(mrl);
		if (player != NULL) {
			player->setPropertyValue("hide", "0x00");
		}
	}

	void ProgramAV::createPlayer(string mrl) {
		string name;
		int pid;

		name = getNameFromMrl(mrl);
		if (isNumeric((void*)(name.c_str()))) {
			pid = util::stof(name);

		} else {
			pid = getPidByName(name);
		}

		if (pid < 0) {
			clog << "ProgramAV::createPlayer Warning! Can't create player '";
			clog << mrl << "' with name '" << name << "': pid '" << pid;
			clog << "' not found!" << endl;
			return;
		}

		currentPid = pid;

#if HAVE_COMPONENTS
		currentPlayer = ((PlayerCreator*)(cm->getObject("AVPlayer")))(
				myScreen, mrl.c_str());

#else
		currentPlayer = new AVPlayer(myScreen, mrl.c_str());
#endif

		if (fullScreenBounds != "") {
			playerBounds[pid] = fullScreenBounds;
			currentPlayer->setPropertyValue("createWindow", fullScreenBounds);
			fullScreenBounds = "";
		}

		setPlayer(pid, currentPlayer);

		clog << "ProgramAV::createPlayer for '" << mrl << "' all done" << endl;
	}

	void ProgramAV::setPlayer(int pid, IPlayer* player) {
		map<int, IPlayer*>::iterator i;
		IPlayer* ePlayer;

		i = players.find(pid);
		if (i == players.end()) {
			players[pid] = player;

		} else {
			ePlayer = players[pid];
			players[pid] = player;
			delete ePlayer;
			ePlayer = NULL;
		}
	}

	IPlayer* ProgramAV::getPlayer(string mrl) {
		string name;
		int pid;

		name = getNameFromMrl(mrl);
		if (isNumeric((void*)(name.c_str()))) {
			pid = util::stof(name);

		} else {
			pid = getPidByName(name);
		}

		return getPlayer(pid);
	}

	IPlayer* ProgramAV::getPlayer(int pid) {
		map<int, IPlayer*>::iterator i;

		i = players.find(pid);
		if (i != players.end()) {
			return i->second;
		}

		return NULL;
	}

	void ProgramAV::setPropertyValue(string pName, string pValue) {
		clog << "ProgramAV::setPropertyValue '" << pName << "' = '";
		clog << pValue << "'" << endl;

		if (pName.substr(0, 11) == "sbtvd-ts://") {
			addPidName(getNameFromMrl(pName), util::stof(pValue));

		} else if (pName == "createPlayer") {
			createPlayer(pValue);

		} else if (pName == "showPlayer") {
			showPlayer(pValue);

		} else if (pName == "hidePlayer") {
			hidePlayer(pValue);

		} else if (pName == "setBoundaries") {
			fullScreenBounds = pValue;

		} else if (pName == "forcePids") {
			forcePids(pValue);

		} else if (currentPlayer != NULL) {
			if (pName == "bounds") {
				if (pValue == "") {
					if (playerBounds.count(currentPid) != 0) {
						currentPlayer->setPropertyValue(
								pName, playerBounds[currentPid]);
					}

				} else {
					currentPlayer->setPropertyValue(pName, pValue);
				}

			} else {
				currentPlayer->setPropertyValue(pName, pValue);
				Player::setPropertyValue(pName, pValue);
			}
		}
	}
}
}
}
}
}
}

using namespace ::br::pucrio::telemidia::ginga::core::player;

extern "C" IPlayer* createProgramAV(GingaScreenID screenId) {
	return (IPlayer*)(Player*)ProgramAV::getInstance(screenId);
}

extern "C" void destroyProgramAV(IPlayer* pav) {
	//TODO: static release method
	delete pav;
}
