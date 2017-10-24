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

#include "aux-ginga.h"
#include "NclAction.h"

GINGA_FORMATTER_BEGIN

/**
 * @brief Creates a new action.
 * @param event Target event.
 * @param transition Action transition.
 */
NclAction::NclAction (NclEvent *event, EventStateTransition transition)
{
  g_assert_nonnull (event);
  _event = event;
  _transition = transition;
  _duration = "";
  _value = "";
}

/**
 * @brief Destroys action.
 */
NclAction::~NclAction ()
{
}

/**
 * @brief Gets target event.
 * @return Target event.
 */
NclEvent *
NclAction::getEvent ()
{
  return _event;
}

/**
 * @brief Gets target event type.
 * @return Target event type.
 */
EventType
NclAction::getEventType ()
{
  return _event->getType ();
}

/**
 * @brief Gets action transition.
 * @return Action transition.
 */
EventStateTransition
NclAction::getEventStateTransition ()
{
  return _transition;
}

/**
 * @brief Gets action duration.
 * @return Action duration.
 */
string
NclAction::getDuration ()
{
  return _duration;
}

/**
 * @brief Sets action duration.
 * @param duration Duration.
 */
void
NclAction::setDuration (const string &duration)
{
  _duration = duration;
}

/**
 * @brief Gets action value.
 * @return Action value.
 */
string
NclAction::getValue (void)
{
  return _value;
}

/**
 * @brief Sets action value.
 * @param value Value.
 */
void
NclAction::setValue (const string &value)
{
  _value = value;
}

GINGA_FORMATTER_END
