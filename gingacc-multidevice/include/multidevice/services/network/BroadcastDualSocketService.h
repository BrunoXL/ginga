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

#ifndef _BroadcastDualSocketService_H_
#define _BroadcastDualSocketService_H_

#include "ISocketService.h"

#include "system/compat/SystemCompat.h"
#include "system/compat/PracticalSocket.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::compat;

#include "system/thread/Thread.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::thread;

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __DARWIN_UNIX03
#include <ifaddrs.h>
#define inaddrr(x) (*(struct in_addr *) myAddr->x[sizeof sa.sin_port])
#endif

#include <pthread.h>
#include <vector>
#include <string>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace multidevice {
  class BroadcastDualSocketService : public ISocketService {
	private:

	    string broadcastIPAddr;
		unsigned int interfaceIP;

	    unsigned int broadcastReadPort;
		unsigned int broadcastWritePort;


		UDPSocket* readSocket;
		UDPSocket* writeSocket;


		pthread_mutex_t mutexBuffer;
		vector<struct frame*>* outputBuffer;

	public:
		BroadcastDualSocketService(
				unsigned int readPort, unsigned int writePort);

		~BroadcastDualSocketService();

	private:
		bool createSocket();
		bool addToGroup();
		bool setSocketOptions();
		bool tryToBind();

	public:
		unsigned int getInterfaceIPAddress(){return 0;};
		int getServicePort();
		void dataRequest(char* data, int taskSize, bool repeat=true);

	private:
		bool sendData(struct frame* f);

	public:
		bool checkOutputBuffer();
		bool checkInputBuffer(char* data, int* size);
  };
}
}
}
}
}
}

#endif /*_BroadcastDualSocketService_H_*/