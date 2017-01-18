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

#ifndef _SWITCHEVENT_H_
#define _SWITCHEVENT_H_

#include "ncl/InterfacePoint.h"
using namespace ::ginga::ncl;

#include "NclFormatterEvent.h"
#include "INclEventListener.h"
using namespace ::br::pucrio::telemidia::ginga::ncl::model::event;

BR_PUCRIO_TELEMIDIA_GINGA_NCL_MODEL_SWITCHES_BEGIN

class NclSwitchEvent : public NclFormatterEvent, public INclEventListener
{
private:
  InterfacePoint *interfacePoint;
  string key;
  NclFormatterEvent *mappedEvent;

public:
  NclSwitchEvent (string id,
               void *executionObjectSwitch, // NclExecutionObjectSwitch
               InterfacePoint *interfacePoint, int eventType, string key);

  virtual ~NclSwitchEvent ();

  InterfacePoint *getInterfacePoint ();
  short getEventType ();
  string getKey ();
  void setMappedEvent (NclFormatterEvent *event);
  NclFormatterEvent *getMappedEvent ();
  virtual void eventStateChanged (void *event, short transition,
                                  short previousState);

  short getPriorityType ();
};

BR_PUCRIO_TELEMIDIA_GINGA_NCL_MODEL_SWITCHES_END
#endif //_SWITCHEVENT_H_