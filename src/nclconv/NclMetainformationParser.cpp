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
#include "NclMetainformationParser.h"

GINGA_NCLCONV_BEGIN

NclMetainformationParser::NclMetainformationParser (NclParser *nclParser)
    : ModuleParser (nclParser)
{
}

Meta *
NclMetainformationParser::parseMeta (DOMElement *meta_element)
{
  string name, content;

  if (!dom_element_try_get_attr(name, meta_element, "name"))
    {
      syntax_error ("meta: missing name");
    }

  if (!dom_element_try_get_attr(content, meta_element, "content"))
    {
      syntax_error ("meta: missing content");
    }

  return new Meta (name, deconst (void *, content.c_str ()));
}

Metadata *
NclMetainformationParser::parseMetadata (arg_unused (DOMElement *parentElement))
{
  return new Metadata ();
}

GINGA_NCLCONV_END
