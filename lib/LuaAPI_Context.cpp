/* Copyright (C) 2006-2018 PUC-Rio/Laboratorio TeleMidia

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
along with Ginga.  If not, see <https://www.gnu.org/licenses/>.  */

#include "LuaAPI.h"
#include "Context.h"

Context *
LuaAPI::Context_check (lua_State *L, int i)
{
  g_return_val_if_fail (L != NULL, NULL);
  return *((Context **) luaL_checkudata (L, i, LuaAPI::_CONTEXT));
}

void
LuaAPI::Context_push (lua_State *L, Context *ctx)
{
  g_return_if_fail (L != NULL);
  g_return_if_fail (ctx != NULL);

  LuaAPI::_pushLuaWrapper (L, ctx);
}

void
LuaAPI::Context_call (lua_State *L, Context *ctx, const char *name,
                      int nargs, int nresults)
{
  g_return_if_fail (L != NULL);
  g_return_if_fail (ctx != NULL);
  g_return_if_fail (name != NULL);
  g_return_if_fail (nargs >= 0);
  g_return_if_fail (nresults >= 0);

  LuaAPI::_callLuaWrapper (L, ctx, name, nargs, nresults);
}