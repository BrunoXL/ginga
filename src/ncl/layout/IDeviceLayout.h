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

#ifndef I_DEVICE_LAYOUT_H
#define I_DEVICE_LAYOUT_H

#include "ginga.h"

BR_PUCRIO_TELEMIDIA_NCL_LAYOUT_BEGIN

class IDeviceProperty
{
public:
  virtual ~IDeviceProperty(){};
  virtual void setDeviceLeft(int left)=0;
  virtual int getDeviceLeft()=0;
  virtual void setDeviceTop(int top)=0;
  virtual int getDeviceTop()=0;
  virtual void setDeviceWidth(int width)=0;
  virtual int getDeviceWidth()=0;
  virtual void setDeviceHeight(int height)=0;
  virtual int getDeviceHeight()=0;
};

class IDeviceLayout {
public:
  virtual ~IDeviceLayout(){};
  virtual string getLayoutName()=0;
  virtual void addDevice(string name, int x, int y,
                         int width, int height)=0;
  virtual IDeviceProperty* getDeviceProperty(string name)=0;
};

BR_PUCRIO_TELEMIDIA_NCL_LAYOUT_END

#endif /* I_DEVICE_LAYOUT_H */
