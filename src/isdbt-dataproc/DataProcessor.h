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

#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

#include "isdbt-tsparser/IMpegDescriptor.h"
using namespace ::ginga::tsparser;

#include "isdbt-tsparser/IAIT.h"
using namespace ::ginga::tsparser;

#include "dsmcc/ServiceDomain.h"
#include "dsmcc/IServiceDomainListener.h"
#include "dsmcc/IObjectListener.h"
#include "dsmcc/MessageProcessor.h"
using namespace ::br::pucrio::telemidia::ginga::core::dataprocessing::carousel;

#include "EPGProcessor.h"
#include "IEPGListener.h"
using namespace ::br::pucrio::telemidia::ginga::core::dataprocessing::epg;

#include "FilterManager.h"
#include "system/ITimeBaseProvider.h"
using namespace ::ginga::system;

#include "isdbt-tuner/ISTCProvider.h"
using namespace ::ginga::tuner;

#include "isdbt-tsparser/IDemuxer.h"
#include "isdbt-tsparser/IFilterListener.h"
using namespace ::ginga::tsparser;

#include "dsmcc/IStreamEventListener.h"
using namespace ::br::pucrio::telemidia::ginga::core::dataprocessing;

#include "dsmcc/IObjectListener.h"
#include "dsmcc/IServiceDomainListener.h"
using namespace ::br::pucrio::telemidia::ginga::core::dataprocessing::carousel;

#include "IEPGListener.h"
using namespace ::br::pucrio::telemidia::ginga::core::dataprocessing::epg;


#include "dsmcc/IStreamEventListener.h"

#include "dsmcc/NPTProcessor.h"
using namespace ::br::pucrio::telemidia::ginga::core::dataprocessing::dsmcc::npt;


struct notifyData {
	IStreamEventListener* listener;
	StreamEvent* se;
};

BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_BEGIN

	class DataProcessor : public IFilterListener, public ITunerListener,
				public IServiceDomainListener, public Thread {

		private:
			EPGProcessor* epgProcessor;
			FilterManager* filterManager;
			map<unsigned int, MessageProcessor*> processors;
			map<string, set<IStreamEventListener*>*> eventListeners;
			set<IObjectListener*> objectListeners;
			IServiceDomainListener* sdl;
			set<unsigned int> processedIds;
			NPTProcessor* nptProcessor;
			vector<ITransportSection*> sections;
			IDemuxer* demux;
			IAIT* ait;
			bool running;
			bool removeOCFilter;
			bool nptPrinter;

		public:
			DataProcessor();

		private:
			virtual ~DataProcessor();

		public:
			void deleteAIT();
			void setNptPrinter(bool nptPrinter);

			bool applicationInfoMounted(IAIT* ait);
			void serviceDomainMounted(
					string mountPoint,
					map<string, string>* names,
					map<string, string>* paths);

			void setDemuxer(IDemuxer* demux);
			void removeOCFilterAfterMount(bool removeIt);

			void setSTCProvider(ISTCProvider* stcProvider);
			ITimeBaseProvider* getNPTProvider();

			void createStreamTypeSectionFilter(short streamType);
			void createPidSectionFilter(int pid);

			void addSEListener(
					string eventType, IStreamEventListener* listener);

			void removeSEListener(
					string eventType, IStreamEventListener* listener);

			void setServiceDomainListener(IServiceDomainListener* listener);
			void addObjectListener(IObjectListener* listener);
			void removeObjectListener(IObjectListener* listener);

		private:
			void notifySEListeners(StreamEvent* se);
			static void* notifySEListener(void* data);
			void notifyEitListeners(set<IEventInfo*>* events);

		public:
			void receiveData(char* buff, unsigned int size){};
			void receiveSection(ITransportSection* section);
			void updateChannelStatus(short newStatus, Channel* channel);
			bool isReady();

		private:
			void run();
	};

BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_END
#endif /*DataProcessor_H_*/
