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

#ifndef EPGPROCESSOR_H_
#define EPGPROCESSOR_H_

#ifdef __cplusplus
extern "C" {
#endif
	#include <sys/stat.h>
	#include <stdio.h>
	#include <stdio.h>
#ifdef __cplusplus
}
#endif

#include "util/functions.h"
using namespace ::br::pucrio::telemidia::util;

#include "tsparser/IServiceInfo.h"
#include "tsparser/IEventInfo.h"
#include "tsparser/ITOT.h"
using namespace ::br::pucrio::telemidia::ginga::core::tsparser::si;

#include "IDataProcessor.h"
#include "IEPGProcessor.h"

#include <set>
#include <vector>
#include <map>
#include <string>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace dataprocessing {
namespace epg {
	class EPGProcessor : public IEPGProcessor {
		private:
			set<string>* processedSections;
			bool presentMapReady;
			bool scheduleMapReady;
			bool serviceMapReady;
			bool timeMapReady;

		protected:
			//TODO: link service id from sdt to eit and cdt
			static EPGProcessor* _instance;
			IServiceInfo* service;
			map<unsigned int, IEventInfo*>* eventPresent;
			map<unsigned int, IEventInfo*>* eventSchedule;
			ITOT* tot;
			set<string>* cdt;
			int files;
			set<IEPGListener*>* epgListeners;
			set<IEPGListener*>* serviceListeners;
			set<IEPGListener*>* timeListeners;
			IDataProcessor* dataProcessor;

		private:
			virtual ~EPGProcessor();

		public:
			static EPGProcessor* getInstance();
			void release();
			void setDataProcessor(IDataProcessor* dataProcessor);
			void decodeSdt(string fileName);
			void decodeSdtSection(ITransportSection* section);
			set<IEventInfo*>* decodeEit(string fileName);
			void decodeTot(ITransportSection* section);
			void decodeCdt(string fileName);
			void decodeEitSection(ITransportSection* section);
			void addEPGListener(IEPGListener* listener, string request, unsigned char type);
			map<string, struct SIField> createMap();
			void removeEPGListener(IEPGListener * listener);

			//void generatePresentMap();

		private:
			EPGProcessor();
			struct SIField* handleFieldStr(string str);
			int savePNG(char* data, int pngSize);
			void generateEitMap(map<unsigned int, IEventInfo*>* actualMap);
			void generateSdtMap(IServiceInfo* si);
			void generateTotMap(ITOT* tot);
			void addProcessedSection(ITransportSection* section);
			void callMapGenerator(unsigned int tableId);
			bool checkProcessedSections(ITransportSection* section);
			void printFieldMap(map<string, struct SIField>* fieldMap);

	};
}
}
}
}
}
}
}

#endif /*EPGPROCESSOR_H_*/
