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

#include "ginga.h"
#include "NclInterfacesParser.h"

#include "NclParser.h"

GINGA_PRAGMA_DIAG_IGNORE (-Wsign-conversion)

GINGA_NCLCONV_BEGIN

NclInterfacesParser::NclInterfacesParser (NclParser *nclParser)
    : ModuleParser (nclParser)
{
}

SwitchPort *
NclInterfacesParser::parseSwitchPort (DOMElement *switchPort_element,
                                      SwitchNode *switchNode)
{
  SwitchPort *switchPort = createSwitchPort (switchPort_element, switchNode);

  if (unlikely (switchPort == NULL))
    {
      syntax_error ("switchPort: bad parent '%s'",
                    dom_element_tagname(switchPort_element).c_str());
    }

    for(DOMElement *child:
        dom_element_children_by_tagname(switchPort_element, "mapping"))
      {
        Port *mapping = parseMapping (child, switchPort);
        if (mapping)
          {
            switchPort->addPort (mapping);
          }
      }

  return switchPort;
}

Port *
NclInterfacesParser::parseMapping (DOMElement *parent, SwitchPort *switchPort)
{
  DOMElement *switchElement;
  SwitchNode *switchNode;
  NodeEntity *mappingNodeEntity;
  Node *mappingNode;
  InterfacePoint *interfacePoint;
  Port *port;

  switchElement = (DOMElement *)parent->getParentNode ()->getParentNode (); // FIXME: this is not safe!

  string id = dom_element_get_attr(switchElement, "id");
  string component = dom_element_get_attr(parent, "component");

  switchNode = (SwitchNode *)getNclParser ()->getNode (id); // FIXME: not safe!
  mappingNode = switchNode->getNode (component);

  if (unlikely (mappingNode == NULL))
    syntax_error ("mapping: bad component '%s'", component.c_str ());

  mappingNodeEntity = (NodeEntity *)mappingNode->getDataEntity (); // FIXME: this is not safe!

  string interface;
  if (dom_element_has_attr(parent, "interface"))
    {
      interface = dom_element_get_attr(parent, "interface");
      interfacePoint = mappingNodeEntity->getAnchor (interface);

      if (interfacePoint == NULL)
        {
          if (mappingNodeEntity->instanceOf ("CompositeNode"))
            {
              interfacePoint
                  = ((CompositeNode *)mappingNodeEntity)->getPort (interface);
            }
        }
    }
  else
    {
      interfacePoint = mappingNodeEntity->getAnchor (0);
    }

  if (unlikely (interfacePoint == NULL))
    syntax_error ("mapping: bad interface '%s'", interface);

  port = new Port (switchPort->getId (), mappingNode, interfacePoint);

  return port;
}

Anchor *
NclInterfacesParser::parseArea (DOMElement *parent)
{
  string anchorId;
  string position, anchorLabel;
  Anchor *anchor;

  if (unlikely (!dom_element_has_attr(parent, "id")))
    syntax_error ("area: missing id");

  anchorId = dom_element_get_attr(parent, "id");

  anchor = NULL;

  if (dom_element_has_attr(parent, "begin")
      || dom_element_has_attr(parent, "end")
      || dom_element_has_attr(parent, "first")
      || dom_element_has_attr(parent, "last"))
    {
      anchor = createTemporalAnchor (parent);
    }
  else if (dom_element_has_attr (parent, "text"))
    {
      position = dom_element_get_attr(parent, "position");

      anchor = new TextAnchor (
          anchorId, dom_element_get_attr(parent, "text"),
          xstrto_int (position));
    }
  else if (dom_element_has_attr(parent, "coords"))
    {
      anchor = createSpatialAnchor (parent);
    }
  else if (dom_element_has_attr(parent, "label"))
    {
      anchorLabel = dom_element_get_attr(parent, "label");

      anchor = new LabeledAnchor (anchorId, anchorLabel);
    }
  else
    {
      anchor = new LabeledAnchor (anchorId, anchorId);
    }

  g_assert_nonnull (anchor);

  return anchor;
}

PropertyAnchor *
NclInterfacesParser::parseProperty (DOMElement *parent)
{
  string attributeName, attributeValue;
  PropertyAnchor *anchor;

  if (unlikely (!dom_element_has_attr(parent, "name")))
    syntax_error ("property: missing name");

  attributeName = dom_element_get_attr(parent, "name");

  anchor = new PropertyAnchor (attributeName);
  if (dom_element_try_get_attr(attributeValue, parent, "value"))
    {
      anchor->setPropertyValue (attributeValue);
    }

  return anchor;
}

Port *
NclInterfacesParser::parsePort (DOMElement *parent, CompositeNode *context)
{
  string id, attValue;
  Node *portNode;
  NodeEntity *portNodeEntity;
  InterfacePoint *portInterfacePoint = NULL;
  Port *port = NULL;

  if (unlikely (!dom_element_has_attr(parent, "id")))
    syntax_error ("port: missing id");

  id = dom_element_get_attr(parent, "id");

  if (unlikely (context->getPort (id) != NULL))
    syntax_error ("port '%s': duplicated id", id.c_str ());

  if (!unlikely (dom_element_has_attr (parent, "component")))
    syntax_error ("port '%s': missing component", id.c_str ());

  attValue = dom_element_get_attr(parent, "component");

  portNode = context->getNode (attValue);
  if (unlikely (portNode == NULL))
    {
      syntax_error ("port '%s': bad component '%s'", id.c_str (),
                    attValue.c_str ());
    }

  portNodeEntity = (NodeEntity *)portNode->getDataEntity ();
  if (!dom_element_has_attr(parent, "interface"))
    {
      if (portNode->instanceOf ("ReferNode")
          && ((ReferNode *)portNode)->getInstanceType () == "new")
        {
          portInterfacePoint = portNode->getAnchor (0);
          if (portInterfacePoint == NULL)
            {
              portInterfacePoint = new LambdaAnchor (portNode->getId ());
              portNode->addAnchor (0, (Anchor *)portInterfacePoint);
            }
        }
      else if (portNodeEntity->instanceOf ("Node"))
        {
          portInterfacePoint = portNodeEntity->getAnchor (0);
          if (unlikely (portInterfacePoint == NULL))
            {
              syntax_error ("port '%s': bad interface '%s'",
                            id.c_str (), portNodeEntity->getId ().c_str ());
            }
        }
      else
        {
          g_assert_not_reached ();
        }
    }
  else
    {
      attValue = dom_element_get_attr(parent, "interface");

      if (portNode->instanceOf ("ReferNode")
          && ((ReferNode *)portNode)->getInstanceType () == "new")
        {
          portInterfacePoint = portNode->getAnchor (attValue);
        }
      else
        {
          portInterfacePoint = portNodeEntity->getAnchor (attValue);
        }

      if (portInterfacePoint == NULL)
        {
          if (portNodeEntity->instanceOf ("CompositeNode"))
            {
              portInterfacePoint
                  = ((CompositeNode *)portNodeEntity)->getPort (attValue);
            }
          else
            {
              portInterfacePoint = portNode->getAnchor (attValue);
            }
        }
    }

  if (unlikely (portInterfacePoint == NULL))
    {
      attValue = dom_element_get_attr(parent, "interface");
      syntax_error ("port '%s': bad interface '%s'", id.c_str (),
                    attValue.c_str ());
    }

  port = new Port (id, portNode, portInterfacePoint);
  return port;
}

SpatialAnchor *
NclInterfacesParser::createSpatialAnchor (DOMElement *areaElement)
{
  SpatialAnchor *anchor = NULL;
  string coords, shape;

  if (dom_element_try_get_attr(coords, areaElement, "coords"))
    {
      if (!dom_element_try_get_attr(shape, areaElement, "shape"))
        {
          shape = "rect";
        }

      if (shape == "rect" || shape == "default")
        {
          long int x1, y1, x2, y2;
          sscanf (coords.c_str (), "%ld,%ld,%ld,%ld", &x1, &y1, &x2, &y2);
          anchor = new RectangleSpatialAnchor (
                dom_element_get_attr(areaElement, "id"),
                x1, y1, x2 - x1, y2 - y1);
        }
      else if (shape == "circle")
        {
          // TODO
        }
      else if (shape == "poly")
        {
          // TODO
        }
    }
  return anchor;
}

IntervalAnchor *
NclInterfacesParser::createTemporalAnchor (DOMElement *areaElement)
{
  IntervalAnchor *anchor = NULL;
  string begin, end;
  double begVal, endVal;
  short firstSyntax, lastSyntax;

  if (dom_element_has_attr(areaElement, "begin")
      || dom_element_has_attr(areaElement, "end"))
    {
      if (dom_element_try_get_attr(begin, areaElement ,"begin"))
        {
          begVal = ::ginga::util::strUTCToSec (begin) * 1000;
        }
      else
        {
          begVal = 0;
        }

      if (dom_element_try_get_attr(end, areaElement, "end"))
        {
          endVal = ::ginga::util::strUTCToSec (end) * 1000;
        }
      else
        {
          endVal = IntervalAnchor::OBJECT_DURATION;
        }

      if (xnumeq (endVal, IntervalAnchor::OBJECT_DURATION) || endVal > begVal)
        {
          anchor = new RelativeTimeIntervalAnchor (
              dom_element_get_attr(areaElement, "id"),
              begVal, endVal);
        }
    }

  // region delimeted through sample identifications
  if (dom_element_has_attr(areaElement, "first")
      || dom_element_has_attr(areaElement, "last"))
    {
      begVal = 0;
      endVal = IntervalAnchor::OBJECT_DURATION;
      firstSyntax = ContentAnchor::CAT_NPT;
      lastSyntax = ContentAnchor::CAT_NPT;

      if (dom_element_try_get_attr(begin, areaElement, "first"))
        {
          if (begin.find ("s") != std::string::npos)
            {
              firstSyntax = ContentAnchor::CAT_SAMPLES;
              begVal = xstrtod (
                  begin.substr (0, begin.length () - 1));
            }
          else if (begin.find ("f") != std::string::npos)
            {
              firstSyntax = ContentAnchor::CAT_FRAMES;
              begVal = xstrtod (
                  begin.substr (0, begin.length () - 1));
            }
          else if (begin.find ("npt") != std::string::npos
                   || begin.find ("NPT") != std::string::npos)
            {
              firstSyntax = ContentAnchor::CAT_NPT;
              begVal = xstrtod (
                  begin.substr (0, begin.length () - 3));
            }
        }

      if (dom_element_try_get_attr(end, areaElement, "last"))
        {
          if (end.find ("s") != std::string::npos)
            {
              lastSyntax = ContentAnchor::CAT_SAMPLES;
              endVal
                  = xstrtod (end.substr (0, end.length () - 1));
            }
          else if (end.find ("f") != std::string::npos)
            {
              lastSyntax = ContentAnchor::CAT_FRAMES;
              endVal
                  = xstrtod (end.substr (0, end.length () - 1));
            }
          else if (end.find ("npt") != std::string::npos
                   || end.find ("NPT") != std::string::npos)
            {
              lastSyntax = ContentAnchor::CAT_NPT;
              endVal
                  = xstrtod (end.substr (0, end.length () - 3));
            }
        }

      anchor = new SampleIntervalAnchor (
            dom_element_get_attr(areaElement, "id"),
          begVal, endVal);

      ((SampleIntervalAnchor *)anchor)
          ->setValueSyntax (firstSyntax, lastSyntax);
    }

  if (anchor != NULL)
    {
      anchor->setStrValues (begin, end);
    }

  return anchor;
}

SwitchPort *
NclInterfacesParser::createSwitchPort (DOMElement *parent,
                                       SwitchNode *switchNode)
{
  SwitchPort *switchPort;
  string id;

  if (unlikely (!dom_element_has_attr(parent, "id")))
    syntax_error ("switchPort: missing id");

  id = dom_element_get_attr(parent, "id");

  if (unlikely (switchNode->getPort (id) != NULL))
    syntax_error ("switchPort '%s': duplicated id", id.c_str ());

  switchPort = new SwitchPort (id, switchNode);
  return switchPort;
}

GINGA_NCLCONV_END
