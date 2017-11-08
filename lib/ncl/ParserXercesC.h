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

#ifndef PARSER_XERCES_C_H
#define PARSER_XERCES_C_H

GINGA_PRAGMA_DIAG_PUSH ()
GINGA_PRAGMA_DIAG_IGNORE (-Wsign-conversion)
GINGA_PRAGMA_DIAG_IGNORE (-Wundef)
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
XERCES_CPP_NAMESPACE_USE
GINGA_PRAGMA_DIAG_POP ()

#include "NclArea.h"
#include "NclAreaLabeled.h"
#include "NclDocument.h"
#include "NclProperty.h"
#include "NclSwitch.h"

GINGA_NAMESPACE_BEGIN

class ParserXercesC: public ErrorHandler
{
public:
  static NclDocument *parse (const string &, int, int, string *);

private:
  NclDocument *_doc;            // NCL document
  string _path;                 // document's absolute path
  string _dirname;              // directory part of document's path
  string _errmsg;               // last error
  int _width;                   // screen width (in pixels)
  int _height;                  // screen height (in pixels)

  map<string,map<string,string>> _descriptors; // cached descriptors
  map<string,map<string,string>> _regions;     // cached regions
  map<string,FormatterPredicate *> _rules;     // cached rules

  struct ConnRole
  {
    string role;
    NclBind::RoleType roleType;
    FormatterEvent::Type eventType;
    FormatterEvent::Transition transition;
    FormatterPredicate *predicate;
    string value;
    string key;
  };
  map<string,list<ConnRole>> _connectors;

  ParserXercesC (int, int);
  ~ParserXercesC ();

  string getErrMsg ();
  void setErrMsg (const string &);

  NclDocument *parse0 (const string &);
  bool parseNcl (DOMElement *);
  void parseHead (DOMElement *);

  NclDocument *parse1 (const string &);
  NclDocument *parseImportNCL (DOMElement *, string *, string *);
  void parseImportBase (DOMElement *, NclDocument **, string *, string *);
  void parseImportedDocumentBase (DOMElement *);

  void parseRuleBase (DOMElement *);
  void parseCompositeRule (DOMElement *, FormatterPredicate *);
  void parseRule (DOMElement *, FormatterPredicate *);

  void parseTransitionBase (DOMElement *);
  void parseTransition (DOMElement *);

  void parseRegionBase (DOMElement *);
  void parseRegion (DOMElement *, GingaRect);

  void parseDescriptorBase (DOMElement *);
  void parseDescriptor (DOMElement *);

  void parseConnectorBase (DOMElement *);
  void parseCausalConnector (DOMElement *);

  FormatterPredicate *parseAssessmentStatement (DOMElement *);
  FormatterPredicate *parseCompoundStatement (DOMElement *);

  void parseCompoundCondition (DOMElement *, list<ConnRole> *, FormatterPredicate *);
  void parseCondition (DOMElement *, list<ConnRole> *, FormatterPredicate *);

  void parseCompoundAction (DOMElement *, list<ConnRole> *);
  void parseSimpleAction (DOMElement *, list<ConnRole> *);

  NclContext *parseBody (DOMElement *);
  void solveNodeReferences (NclComposition *);
  void posCompileContext (DOMElement *, NclContext *);
  void posCompileSwitch (DOMElement *, NclSwitch *);

  NclNode *parseContext (DOMElement *);
  NclPort *parsePort (DOMElement *, NclComposition *);

  NclNode *parseSwitch (DOMElement *);
  NclNode *parseMedia (DOMElement *);
  NclProperty *parseProperty (DOMElement *);
  NclAnchor *parseArea (DOMElement *);

  NclLink *parseLink (DOMElement *, NclContext *);
  NclBind *parseBind (DOMElement *, NclLink *, list<ConnRole> *,
                      map<string, string> *, NclContext *);

  // From ErrorHandler.
  void warning (const SAXParseException &);
  void error (const SAXParseException &);
  void fatalError (const SAXParseException &);
  void resetErrors () {};
};

GINGA_NAMESPACE_END

#endif // PARSER_XERCESC_H
