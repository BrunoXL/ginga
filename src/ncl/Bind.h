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

#ifndef _BIND_H_
#define _BIND_H_

#include "Parameter.h"
using namespace ::ginga::ncl;

#include "GenericDescriptor.h"
using namespace ::ginga::ncl;

#include "Role.h"
using namespace ::ginga::ncl;

#include "NodeEntity.h"
#include "CompositeNode.h"
using namespace ::ginga::ncl;

#include "InterfacePoint.h"
#include "SwitchPort.h"
#include "Port.h"
using namespace ::ginga::ncl;

GINGA_NCL_BEGIN

class Bind
{
private:
  Node *node;
  InterfacePoint *interfacePoint;
  GenericDescriptor *descriptor;
  Role *role;
  bool embed;

protected:
  map<string, Parameter *> *parameters;
  set<string> typeSet;

public:
  Bind (Node *node, InterfacePoint *interfPt, GenericDescriptor *desc,
        Role *role);

  virtual ~Bind ();
  bool instanceOf (string s);
  GenericDescriptor *getDescriptor ();
  InterfacePoint *getInterfacePoint ();
  Node *getNode ();
  Role *getRole ();
  void setInterfacePoint (InterfacePoint *interfPt);
  void setNode (Node *node);
  void setRole (Role *role);
  void setDescriptor (GenericDescriptor *desc);
  Parameter *setParameterValue (string propertyLabel,
                                Parameter *propertyValue);

  string toString ();
  void addParameter (Parameter *parameter);
  vector<Parameter *> *getParameters ();
  Parameter *getParameter (string name);
  bool removeParameter (Parameter *parameter);
  vector<Node *> *getNodeNesting ();
  InterfacePoint *getEndPointInterface ();
};

GINGA_NCL_END

#endif //_BIND_H_