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

#ifndef NCLNCMSTRUCTURECOMPILER_H
#define NCLNCMSTRUCTURECOMPILER_H

#include "ncl/NclDocument.h"
#include "ncl/NclDocument.h"
using namespace ::br::pucrio::telemidia::ncl;

#include "ncl/transition/TransitionBase.h"
using namespace ::br::pucrio::telemidia::ncl::transition;

#include "ncl/metainformation/Meta.h"
#include "ncl/metainformation/Metadata.h"
using namespace ::br::pucrio::telemidia::ncl::metainformation;

#include "ncl/components/ContextNode.h"
using namespace ::br::pucrio::telemidia::ncl::components;

#include "DocumentParser.h"
#include "ModuleParser.h"
using namespace ::br::pucrio::telemidia::converter::framework;

#include "NclStructureParser.h"
using namespace ::br::pucrio::telemidia::converter::framework::ncl;

#include <string>
using namespace std;

#include "NclPresentationSpecConverter.h"
#include "NclComponentsConverter.h"
#include "NclLinkingConverter.h"
#include "NclLayoutConverter.h"
#include "NclInterfacesConverter.h"
#include "NclPresentationControlConverter.h"
#include "NclConnectorsConverter.h"

#include <xercesc/dom/DOM.hpp>
XERCES_CPP_NAMESPACE_USE

namespace br {
namespace pucrio {
namespace telemidia {
namespace converter {
namespace ncl {
  class NclStructureConverter : public NclStructureParser {
	public:
		NclStructureConverter(DocumentParser *documentParser);
		virtual void addBodyToNcl(void *parentObject, void *childObject);
		virtual void addPortToBody(void *parentObject, void *childObject);
		virtual void addPropertyToBody(void *parentObject, void *childObject);
		virtual void addContextToBody(void *parentObject, void *childObject);
		virtual void addSwitchToBody(void *parentObject, void *childObject);
		virtual void addDescriptorBaseToHead(
			    void *parentObject, void *childObject);

		virtual void addHeadToNcl(void *parentObject, void *childObject);
		virtual void addRegionBaseToHead(
			    void *parentObject, void *childObject);

		virtual void addTransitionBaseToHead(
			    void* parentObject, void* childObject);

		virtual void addLinkToBody(void *parentObject, void *childObject);
		virtual void addMediaToBody(void *parentObject, void *childObject);
		virtual void addRuleBaseToHead(void *parentObject, void *childObject);
		virtual void addConnectorBaseToHead(
			    void *parentObject, void *childObject);

		virtual void *createBody(
			    DOMElement *parentElement, void *objGrandParent);

	private:
		void solveNodeReferences(CompositeNode *composition);

	public:
		virtual void* posCompileBody(
			    DOMElement *parentElement, void *parentObject);

		virtual void* createHead(
			    DOMElement *parentElement, void *objGrandParent);

		virtual void* createNcl(
			    DOMElement *parentElement, void *objGrandParent);

		virtual void addImportedDocumentBaseToHead(
			    void *parentObject, void *childObject);

		void addMetaToHead(void* parentObject, void* childObject);
		void addMetadataToHead(void* parentObject, void* childObject);
  };
}
}
}
}
}

#endif //NCLNCMSTRUCTURECOMPILER_H
