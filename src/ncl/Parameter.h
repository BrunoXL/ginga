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

#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#include <string>
#include <set>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ncl {
	class Parameter {
		public:
			Parameter(string n, string v);
			virtual ~Parameter() {};
			bool instanceOf(string s);
			string getName();
			string getValue();
			void setName(string n);
			void setValue(string v);

		protected:
			string name;
			set<string> typeSet; //informacoes de tipo

		private:
			string value;
	};
}
}
}
}

#endif /*PARAMETER_H_*/
