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

#include "util/Color.h"
#include "util/functions.h"
using namespace ::br::pucrio::telemidia::util;

#include "system/compat/SystemCompat.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::compat;

#include "config.h"

#if HAVE_COMPONENTS
#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;
#endif

#include "mb/LocalScreenManager.h"
#include "mb/interface/IContinuousMediaProvider.h"
using namespace ::br::pucrio::telemidia::ginga::core::mb;

#include "mb/interface/CodeMap.h"
#include "mb/interface/IDeviceScreen.h"

extern "C" {
#include "string.h"
}

#include <iostream>
using namespace std;

bool debugging = false;
bool enableChoro = false;

string updateFileUri(string file) {
	if (!SystemCompat::isAbsolutePath(file)) {
		if (debugging) {
			return SystemCompat::getUserCurrentPath() + "gingacc-mb/test/" + file;

		} else {
			return SystemCompat::getUserCurrentPath() + file;
		}
	}

	return file;
}

void testScreen(ILocalScreenManager* dm, GingaScreenID screen) {
	IContinuousMediaProvider* aud1;
	IContinuousMediaProvider* aud2;
	IContinuousMediaProvider* aud3;
	
	string m1, m2, m3;
	bool notTrue = false;

	/* AUDIO PROVIDER */
	aud1 = dm->createContinuousMediaProvider(
			screen,
			"/root/workspaces/NCL/Garrincha/media/animGar.mp4",
			&notTrue,
			false);

	aud1->playOver(NULL, true, NULL);

	m1 = updateFileUri("techno.mp3");
	aud2 = dm->createContinuousMediaProvider(screen, m1.c_str(), &notTrue, false);
	aud2->playOver(NULL, true, NULL);

	if (enableChoro) {
		m2 = updateFileUri("choro.mp3");
		aud3 = dm->createContinuousMediaProvider(screen, m2.c_str(), &notTrue, false);
		aud3->playOver(NULL, true, NULL);
	}

	m3 = updateFileUri("rock.mp3");
	aud3 = dm->createContinuousMediaProvider(screen, m3.c_str(), &notTrue, false);
	aud3->playOver(NULL, true, NULL);
}

int main(int argc, char** argv) {
	GingaScreenID screen1;
	ILocalScreenManager* dm;
	int i;

	SystemCompat::setLogTo(SystemCompat::LOG_STDO);
	initTimeStamp();

#if HAVE_COMPONENTS
	IComponentManager* cm = IComponentManager::getCMInstance();

	dm = ((LocalScreenManagerCreator*)(cm->getObject("LocalScreenManager")))();
#else
	cout << "gingacc-mb test works only with enabled component support";
	cout << endl;
	exit(0);
#endif

	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "--enable-log") == 0) && ((i + 1) < argc)) {
			if (strcmp(argv[i + 1], "stdout") == 0) {
				SystemCompat::setLogTo(SystemCompat::LOG_STDO);

			} else {
				SystemCompat::setLogTo(SystemCompat::LOG_STDO);
			}

		} else if ((strcmp(argv[i], "--debug") == 0)) {
			debugging = true;

		} else if ((strcmp(argv[i], "--enable-choro") == 0)) {
			enableChoro = true;
		}
	}

	cout << "gingacc-mb test has created the screen manager. ";
	cout << endl;

	screen1 = dm->createScreen(argc, argv);
	cout << "gingacc-mb test has created screen '" << screen1;
	cout << "'. calling providers";
	cout << endl;
	testScreen(dm, screen1);

	cout << "gingacc-mb test has shown providers. ";
	cout << "press enter to automatic release";
	cout << endl;
	getchar();

	dm->clearWidgetPools(screen1);
	dm->releaseScreen(screen1);
	dm->releaseMB(screen1);

	delete dm;
	cout << "gingacc-mb test all done. ";
	cout << "press enter to exit";
	cout << endl;

	//TODO: tests

	getchar();
	return 0;
}