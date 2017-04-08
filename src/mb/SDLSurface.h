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

#ifndef SDLSURFACE_H_
#define SDLSURFACE_H_


GINGA_MB_BEGIN

class SDLWindow;
class IFontProvider;

typedef struct DrawData
{
  int coord1;
  int coord2;
  int coord3;
  int coord4;
  short dataType;
  int r;
  int g;
  int b;
  int a;
} DrawData;

class SDLSurface
{
private:
  SDL_Surface *sur;
  SDL_Surface *pending;
  SDLWindow *parent;
  bool hasExtHandler;
  SDL_Color *chromaColor;
  SDL_Color *borderColor;
  SDL_Color *bgColor;
  SDL_Color *surfaceColor;
  IFontProvider *iFont;
  int caps;
  bool isDeleting;

  vector<DrawData *> drawData;
  pthread_mutex_t ddMutex;

  pthread_mutex_t sMutex;
  pthread_mutex_t pMutex;

public:
  SDLSurface ();
  SDLSurface (void *underlyingSurface);
  virtual ~SDLSurface ();

public:

  void setChromaColor (SDL_Color color);
  SDL_Color *getChromaColor ();
  void setBorderColor (SDL_Color color);
  SDL_Color *getBorderColor ();
  void setBgColor (SDL_Color color);
  SDL_Color *getBgColor ();
  void setColor (SDL_Color color);
  SDL_Color *getColor ();
  void setSurfaceFont (void *font);
  


};

GINGA_MB_END

#endif /*SDLSURFACE_H_*/
