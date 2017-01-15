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

#ifndef _LINK_H_
#define _LINK_H_

#include "../Entity.h"
#include "../Parameter.h"
using namespace ::br::pucrio::telemidia::ncl;

#include "../descriptor/GenericDescriptor.h"
using namespace ::br::pucrio::telemidia::ncl::descriptor;

#include "../components/NodeEntity.h"
using namespace ::br::pucrio::telemidia::ncl::components;

#include "../connectors/Connector.h"
#include "../connectors/Role.h"
using namespace ::br::pucrio::telemidia::ncl::connectors;

#include "../interfaces/InterfacePoint.h"
#include "../interfaces/SwitchPort.h"
using namespace ::br::pucrio::telemidia::ncl::interfaces;

#include "Bind.h"

#include <map>
#include <vector>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ncl {
namespace link {
	class LinkComposition;
	class Link : public Entity {
		protected:
			Connector *connector;
			map<string, vector<Bind*>*> *roleBinds;
			vector<Bind*> *binds;
			map<string, Parameter*> *parameters;

		private:
			LinkComposition* composition;

		public:
			Link(string id, Connector *connector);
			virtual ~Link();
			Bind* bind(Node *node, InterfacePoint *interfPt,
				    GenericDescriptor *desc, string roleId);

			Bind* bind(Node *node, InterfacePoint *interfPt,
				    GenericDescriptor *desc, Role *role);

			bool isConsistent();
			Bind* getBind(Node *node, InterfacePoint *interfPt,
				    GenericDescriptor *desc, Role *role);

			vector<Bind*> *getBinds();
			Connector *getConnector();
			void setParentComposition(LinkComposition* composition);
			LinkComposition* getParentComposition();
			unsigned int getNumBinds();
			unsigned int getNumRoleBinds(Role *role);
			vector<Bind*> *getRoleBinds(Role *role);
			bool isMultiPoint();
			string toString();
			void setConnector(Connector *connector);
			bool unBind(Bind *bind);
			void addParameter(Parameter *parameter);
			vector<Parameter*> *getParameters();
			Parameter *getParameter(string name);
			void removeParameter(Parameter *parameter);
			void updateConnector(Connector *newConnector);
			bool containsNode(Node *node, GenericDescriptor *descriptor);

		protected:
			bool containsNode(Node *node, GenericDescriptor *descriptor,
				    vector<Bind*> *binds);
	};
}
}
}
}
}

#endif //_LINK_H_