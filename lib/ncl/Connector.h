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

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "Action.h"
#include "Condition.h"
#include "Entity.h"
#include "Parameter.h"
#include "Role.h"

GINGA_NCL_BEGIN

class Connector : public Entity
{
public:
  Connector (NclDocument *, const string &);
  virtual ~Connector ();

  Condition *getCondition ();
  void initCondition (Condition *);

  Action *getAction ();
  void initAction (Action *);

  Role *getRole (const string &);

private:
  map<string,string> _param;
  Condition *_condition;
  Action *_action;

  Role *searchRole (Condition *, const string &);
  Role *searchRole (Action *, const string &);
};

GINGA_NCL_END

#endif // CONNECTOR_H
