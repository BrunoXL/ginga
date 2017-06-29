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

#ifndef PARSER_H
#define PARSER_H

GINGA_PRAGMA_DIAG_PUSH ()
GINGA_PRAGMA_DIAG_IGNORE (-Wsign-conversion)
GINGA_PRAGMA_DIAG_IGNORE (-Wundef)
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/util/XercesDefs.hpp>
XERCES_CPP_NAMESPACE_USE
GINGA_PRAGMA_DIAG_POP ()

#include "ncl/NclDocument.h"
#include "ncl/TransitionUtil.h"
#include "ncl/Rule.h"
#include "ncl/SimpleRule.h"
#include "ncl/CompositeRule.h"
#include "ncl/ContentNode.h"
#include "ncl/AbsoluteReferenceContent.h"
#include "ncl/CausalConnector.h"
#include "ncl/ValueAssessment.h"
#include "ncl/SwitchNode.h"
#include "ncl/SpatialAnchor.h"
#include "ncl/TextAnchor.h"
#include "ncl/LabeledAnchor.h"
#include "ncl/RectangleSpatialAnchor.h"
#include "ncl/CausalLink.h"
#include "ncl/ReferredNode.h"
#include "ncl/Comparator.h"
#include "ncl/Descriptor.h"
using namespace ginga::ncl;

#include <vector>
using namespace std;

GINGA_FORMATTER_BEGIN

class NclParser : public ErrorHandler
{
public:
  NclParser ();
  ~NclParser ();

  NclDocument *importDocument (string &docLocation);

  void warning (const SAXParseException &);
  void error (const SAXParseException &);
  void fatalError (const SAXParseException &);
  void resetErrors () {};

  NclDocument *parse (const string &);

private:
  NclDocument *_doc;            // NCL document
  string _path;                 // document's absolute path
  string _dirname;              // directory part of document's path

// STRUCTURE
  void solveNodeReferences (CompositeNode *composition);

// COMPONENTS
  ContextNode *posCompileContext (DOMElement *ctx_element, ContextNode *ctx);



// INTERFACES


// PRESENTATION CONTROL

  SwitchNode *posCompileSwitch (DOMElement *parentElement, SwitchNode *parentObject);
  DescriptorSwitch *parseDescriptorSwitch (DOMElement *parentElement);
  DescriptorSwitch *createDescriptorSwitch (DOMElement *parentElement);
  vector<Node *> *getSwitchConstituents (SwitchNode *switchNode);

  map<string, map<string, Node *> *> _switchConstituents;
  void addBindRuleToSwitch (SwitchNode *parentObject, DOMElement *childObject);
  void addUnmappedNodesToSwitch (SwitchNode *switchNode);
  void addDefaultDescriptorToDescriptorSwitch (DescriptorSwitch *descriptorSwitch, DOMElement *defaultDescriptor);
  void addDefaultComponentToSwitch (SwitchNode *switchNode, DOMElement *defaultComponent);
  void addBindRuleToDescriptorSwitch (DescriptorSwitch *descriptorSwitch, DOMElement *bindRule_element);
  void addDescriptorToDescriptorSwitch (DescriptorSwitch *descriptorSwitch, GenericDescriptor *descriptor);

// PRESENTATION SPECIFICATION


  // --------------------------------------------------------------
  void parseNcl (DOMElement *);
  void parseHead (DOMElement *);

  NclDocument * parseImportNCL (DOMElement *, string *, string *);
  Base *parseImportBase (DOMElement *, NclDocument **, string *, string *);
  void parseImportedDocumentBase (DOMElement *);

  RuleBase *parseRuleBase (DOMElement *);
  CompositeRule *parseCompositeRule (DOMElement *);
  SimpleRule *parseRule (DOMElement *);

  TransitionBase *parseTransitionBase (DOMElement *);
  Transition *parseTransition (DOMElement *);

  RegionBase *parseRegionBase (DOMElement *);
  LayoutRegion *parseRegion (DOMElement *, RegionBase *, LayoutRegion *);

  DescriptorBase *parseDescriptorBase (DOMElement *);
  Descriptor *parseDescriptor (DOMElement *);

  ConnectorBase *parseConnectorBase (DOMElement *);
  CausalConnector *parseCausalConnector (DOMElement *);
  CompoundCondition *parseCompoundCondition (DOMElement *);
  SimpleCondition *parseSimpleCondition (DOMElement *);
  CompoundStatement *parseCompoundStatement (DOMElement *);
  AssessmentStatement *parseAssessmentStatement (DOMElement *);
  AttributeAssessment *parseAttributeAssessment (DOMElement *);
  ValueAssessment *parseValueAssessment (DOMElement *);
  CompoundAction *parseCompoundAction (DOMElement *);
  SimpleAction *parseSimpleAction (DOMElement *);

  ContextNode *parseBody (DOMElement *);
  Node *parseContext (DOMElement *);
  Port *parsePort (DOMElement *, CompositeNode *);

  Node *parseSwitch (DOMElement *);
  SwitchPort *parseSwitchPort (DOMElement *, SwitchNode *);
  Port *parseMapping (DOMElement *, SwitchNode *, SwitchPort *);

  Node *parseMedia (DOMElement *);
  PropertyAnchor *parseProperty (DOMElement *);
  Anchor *parseArea (DOMElement *);

  Link *parseLink (DOMElement *, CompositeNode *);
  Parameter *parseLinkParam (DOMElement *);
  Bind *parseBind (DOMElement *, Link *, CompositeNode *);
  Parameter *parseBindParam (DOMElement *);
};

GINGA_FORMATTER_END

#endif /*PARSER_H*/
