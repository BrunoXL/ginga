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

#ifndef LINKATTRIBUTEASSESSMENT_H_
#define LINKATTRIBUTEASSESSMENT_H_

#include "ncl/EventUtil.h"
using namespace ::ginga::ncl;

#include "NclAttributionEvent.h"
#include "NclFormatterEvent.h"
#include "NclPresentationEvent.h"
using namespace ::br::pucrio::telemidia::ginga::ncl::model::event;

#include "NclLinkAssessment.h"

BR_PUCRIO_TELEMIDIA_GINGA_NCL_MODEL_LINK_BEGIN

class NclLinkAttributeAssessment : public NclLinkAssessment
{
private:
  NclFormatterEvent *event;
  short attributeType;
  string offset;

public:
  NclLinkAttributeAssessment (NclFormatterEvent *ev, short attrType);

  NclFormatterEvent *getEvent ();
  void setOffset (string offset);
  string getOffset ();
  void setEvent (NclFormatterEvent *ev);
  short getAttributeType ();
  void setAttributeType (short attrType);
  string getValue ();

private:
  string getAssessmentWithOffset (string assessmentValue);
};

BR_PUCRIO_TELEMIDIA_GINGA_NCL_MODEL_LINK_END
#endif /*LINKATTRIBUTEASSESSMENT_H_*/