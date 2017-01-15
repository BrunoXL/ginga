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

#ifndef _IPLAYERADAPTERMANAGER_H_
#define _IPLAYERADAPTERMANAGER_H_

#include "system/ITimeBaseProvider.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::time;

#include "player/INCLPlayer.h"
using namespace ::br::pucrio::telemidia::ginga::core::player;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace ncl {
namespace adapters {
  class IPlayerAdapterManager {
	public:
		virtual ~IPlayerAdapterManager(){};
		virtual NclPlayerData* getNclPlayerData()=0;
		virtual ITimeBaseProvider* getTimeBaseProvider()=0;
		virtual void* getObjectPlayer(void* execObj)=0;
		virtual bool removePlayer(void* object)=0;
  };
}
}
}
}
}
}

#endif //_IPLAYERADAPTERMANAGER_H_