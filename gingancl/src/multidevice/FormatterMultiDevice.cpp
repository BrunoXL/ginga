/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
*******************************************************************************/

#include "gingancl/multidevice/FormatterMultiDevice.h"
#include "gingancl/multidevice/FormatterBaseDevice.h"

#if HAVE_MULTIDEVICE
#include "gingancl/multidevice/FormatterPassiveDevice.h"
#include "gingancl/multidevice/FormatterActiveDevice.h"
#endif //HAVE_MULTIDEVICE
#include "gingancl/multidevice/FMDComponentSupport.h"

#include "mb/LocalScreenManager.h"
#include "mb/IInputManager.h"
#include "mb/interface/CodeMap.h"
using namespace ::br::pucrio::telemidia::ginga::core::mb;


#include "ncl/layout/DeviceLayout.h"
using namespace ::br::pucrio::telemidia::ncl::layout;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace ncl {
namespace multidevice {
	ILocalScreenManager* FormatterMultiDevice::dm   = NULL;
#if HAVE_MULTIDEVICE
	IRemoteDeviceManager* FormatterMultiDevice::rdm = NULL;
#else
	void* FormatterMultiDevice::rdm = NULL;
#endif

	FormatterMultiDevice::FormatterMultiDevice(
			GingaScreenID screenId,
			IDeviceLayout* deviceLayout,
			int x, int y, int w, int h, bool useMulticast, int srvPort) {

		this->xOffset          = x;
		this->yOffset          = y;
		this->defaultWidth     = w;
		this->defaultHeight    = h;
		this->deviceClass      = -1;
		this->hasRemoteDevices = false;
		this->deviceLayout     = deviceLayout;
		this->activeBaseUri    = "";
		this->activeUris       = NULL;
		this->bitMapScreen     = NULL;
		this->serialized       = NULL;
		this->presContext      = NULL;
		this->focusManager     = NULL;
		this->parent           = NULL;
		this->myScreen         = screenId;
		this->enableMulticast  = useMulticast;

		if (dm == NULL) {
#if HAVE_COMPONENTS
			dm = ((LocalScreenManagerCreator*)(
					cm->getObject("LocalScreenManager")))();

#else
			dm = LocalScreenManager::getInstance();
#endif
		}

		LocalScreenManager::addIEListenerInstance(this);

		if (defaultWidth == 0) {
			defaultWidth = dm->getDeviceWidth(myScreen);
		}

		if (defaultHeight == 0) {
			defaultHeight = dm->getDeviceHeight(myScreen);
		}

		im = dm->getInputManager(myScreen);

		im->setAxisValues(
				(int)(dm->getDeviceWidth(myScreen) / 2),
				(int)(dm->getDeviceHeight(myScreen) / 2), 0);

		printScreen = dm->createWindow(
				myScreen, 0, 0, defaultWidth, defaultHeight, -1.0);

		printScreen->setCaps(printScreen->getCap("ALPHACHANNEL"));
		printScreen->draw();

		Thread::mutexInit(&mutex, false);
		Thread::mutexInit(&lMutex, false);
	}

	FormatterMultiDevice::~FormatterMultiDevice() {
		set<IPlayer*>::iterator i;
		LocalScreenManager::removeIEListenerInstance(this);

		presContext = NULL;

		clog << "FormatterMultiDevice::~FormatterMultiDevice ";
		clog << "checking listening list";
		clog << endl;

		if (!listening.empty()) {
			Thread::mutexLock(&lMutex);
/*			i = listening.begin();
			while (i != listening.end()) {
				(*i)->removeListener(this);
				++i;
			}*/
			listening.clear();
			Thread::mutexUnlock(&lMutex);
		}

		clog << "FormatterMultiDevice::~FormatterMultiDevice ";
		clog << "destroying mutexes";
		clog << endl;

		Thread::mutexDestroy(&mutex);
		Thread::mutexDestroy(&lMutex);

		/*if (im != NULL) {
			clog << "FormatterMultiDevice::~FormatterMultiDevice ";
			clog << "stop listening events";
			clog << endl;
			im->removeInputEventListener(this);
		}*/

		clog << "FormatterMultiDevice::~FormatterMultiDevice ";
		clog << "all done";
		clog << endl;
	}

	void FormatterMultiDevice::printGingaWindows() {
		string fileUri = "";
		IWindow* iWin;
		FormatterLayout* formatterLayout;

		vector<GingaWindowID> sortedIds;
		map<int, FormatterLayout*>::iterator i;
		vector<GingaWindowID>::iterator j;

		int quality = 100;
		int dumpW   = defaultWidth;
		int dumpH   = defaultHeight;

		cout << "FormatterMultiDevice::printGingaWindows(" << this << ") ";
		cout << "layout manager has '" << layoutManager.size();
		cout << "' layouts" << endl;

		cout << "Serialized window Id = '";
		cout << (unsigned long)serialized->getId();
		cout << "'" << endl;

		serialized->getDumpFileUri(quality, dumpW, dumpH);

		cout << "BitMapScreen window Id = '";
		if (bitMapScreen != NULL) {
			cout << (unsigned long)bitMapScreen->getId() << "'";
			bitMapScreen->getDumpFileUri(quality, dumpW, dumpH);

		} else {
			cout << "NULL'";
		}
		cout << endl;

		i = layoutManager.begin();
		while (i != layoutManager.end()) {
			formatterLayout = i->second;

			cout << "device '" << i->first << "' ";

			formatterLayout->getSortedIds(&sortedIds);
			if (!sortedIds.empty()) {
				if (i->first == 1) {
					quality = 45;
					dumpW   = 480 / 1.8;
					dumpH   = 320 / 1.8;
				}

				cout << "has the following : ";
				j = sortedIds.begin();
				while (j != sortedIds.end()) {
					iWin = dm->getIWindowFromId(myScreen, (*j));

					if (iWin != NULL) {
						iWin->getDumpFileUri(quality, dumpW, dumpH);
					}

					cout << "'" << (unsigned long)(*j) << "' ";
					++j;
				}
				cout << endl;

			} else {
				cout << "is empty " << endl;
			}

			sortedIds.clear();
			++i;
		}

		cout << "FormatterMultiDevice::printGingaWindows all done";
		cout << endl;
	}

	void FormatterMultiDevice::listenPlayer(IPlayer* player) {
		Thread::mutexLock(&lMutex);
		listening.insert(player);
		player->addListener(this);
		Thread::mutexUnlock(&lMutex);
	}

	void FormatterMultiDevice::stopListenPlayer(IPlayer* player) {
		set<IPlayer*>::iterator i;

		Thread::mutexLock(&lMutex);
		i = listening.find(player);
		if (i != listening.end()) {
			listening.erase(i);
			player->removeListener(this);
		}
		Thread::mutexUnlock(&lMutex);
	}

	void FormatterMultiDevice::setParent(IFormatterMultiDevice* parent) {
		this->parent = (FormatterMultiDevice*)parent;
	}

	void FormatterMultiDevice::setPresentationContex(
			IPresentationContext* presContext) {

		this->presContext = presContext;
	}

	void FormatterMultiDevice::setFocusManager(
			IFormatterFocusManager* focusManager) {

		this->focusManager = focusManager;
	}

	void FormatterMultiDevice::setBackgroundImage(string uri) {
		dm->setBackgroundImage(myScreen, uri);
	}

	void* FormatterMultiDevice::getMainLayout() {
		return mainLayout;
	}

	void* FormatterMultiDevice::getFormatterLayout(int devClass) {
		map<int, FormatterLayout*>::iterator i;

		i = layoutManager.find(devClass);
		if (i != layoutManager.end()) {
			return i->second;
		}

		return NULL;
	}

	string FormatterMultiDevice::getScreenShot() {
		return serializeScreen(deviceClass, printScreen);
	}

	string FormatterMultiDevice::serializeScreen(
			int devClass, IWindow* mapWindow) {

		string fileUri = "";
		FormatterLayout* formatterLayout;
		vector<GingaWindowID> sortedIds;
		map<int, FormatterLayout*>::iterator i;
		int quality = 100;
		int dumpW = defaultWidth;
		int dumpH = defaultHeight;

		i = layoutManager.find(devClass);
		if (i != layoutManager.end()) {
			formatterLayout = i->second;
			mapWindow->clearContent();
			formatterLayout->getSortedIds(&sortedIds);
			if (!sortedIds.empty()) {
				if (!dm->mergeIds(myScreen, mapWindow->getId(), &sortedIds)) {
					return "";
				}

				if (devClass == 1) {
					quality = 45;
					dumpW   = 480 / 1.8;
					dumpH   = 320 / 1.8;
				}
			}
			fileUri = mapWindow->getDumpFileUri(quality, dumpW, dumpH);

			clog << "FormatterMultiDevice::serializeScreen fileURI = '";
			clog << fileUri << "' sortedIds size = '" << sortedIds.size();
			clog << endl;
		}

		return fileUri;
	}

	void FormatterMultiDevice::postMediaContent(int destDevClass) {
		string fileUri;
		IWindow* bmpScr = NULL;
		vector<GingaWindowID> wins;

		/*clog << "FormatterMultiDevice::postMediaContent to class '";
		clog << destDevClass << "'";
		clog << endl;*/

		Thread::mutexLock(&mutex);
		if (destDevClass == IDeviceDomain::CT_PASSIVE) {
			if (parent != NULL) {
				bmpScr = parent->bitMapScreen;

			} else {
				bmpScr = bitMapScreen;
			}

			fileUri = serializeScreen(destDevClass, serialized);
			if (fileUri != "" && fileExists(fileUri)) {
#if HAVE_MULTIDEVICE
				rdm->postMediaContent(destDevClass, fileUri);

				clog << "FormatterMultiDevice::postMediaContent(";
				clog << this << ")";
				clog << " serialized window id = '";
				clog << (unsigned long)serialized->getId();

#endif //HAVE_MULTIDEVICE

				if (bmpScr != NULL) {
					wins.push_back(serialized->getId());
					dm->mergeIds(myScreen, bmpScr->getId(), &wins);

					bmpScr->show();

					clog << "' bmpScr = '";
					clog << (unsigned long)bmpScr->getId();

					/*bmpScr->clearContent();
					bmpScr->stretchBlit(serialized);
					bmpScr->show();
					bmpScr->validate();*/

					//renderFromUri(bitMapScreen, fileUri);
				}

				clog << "'" << endl;
			}

		} else if (destDevClass == IDeviceDomain::CT_ACTIVE) {
			if (!activeUris->empty()) {

			}
		}
		Thread::mutexUnlock(&mutex);
	}

	FormatterLayout* FormatterMultiDevice::getFormatterLayout(
			CascadingDescriptor* descriptor, ExecutionObject* object) {

		map<int, FormatterLayout*>::iterator i;
		FormatterLayout* layout;
		LayoutRegion* region;
		int devClass;

		/*clog << "FormatterMultiDevice::getFormatterLayout for '";
		clog << object->getId() << "' formatterMultiDevice class = '";
		clog << this->deviceClass << "'" << endl;*/

		region = descriptor->getRegion();
		if (region == NULL) {
			if (layoutManager.count(this->deviceClass) != 0) {
				region = descriptor->getRegion(
						layoutManager[this->deviceClass], object);
			}

			if (region == NULL) {
				clog << "FormatterMultiDevice::getFormatterLayout ";
				clog << "region is NULL";
				clog << endl;
				return NULL;
			}
		}

		devClass = region->getDeviceClass();
		i = layoutManager.find(devClass);

		if (i == layoutManager.end()) {
			if (devClass == IDeviceDomain::CT_PASSIVE) {
				layout = new FormatterLayout(
						myScreen, 0, 0, DV_QVGA_WIDTH, DV_QVGA_HEIGHT);

				layoutManager[devClass] = layout;
				return layout;
			}

			/*clog << "FormatterMultiDevice::getFormatterLayout NOT FOUND for ";
			clog << "class '" << devClass << "'" << endl;*/
			return NULL;

		} else {
			/*clog << "FormatterMultiDevice::getFormatterLayout FOUND class '";
			clog << devClass << "' in LAYOUTMANAGER" << endl;*/
			return i->second;
		}
	}

	GingaWindowID FormatterMultiDevice::prepareFormatterRegion(
			ExecutionObject* executionObject, ISurface* renderedSurface) {

		FormatterLayout* layout;
		CascadingDescriptor* descriptor;
		string regionId, plan = "";
		GingaWindowID windowId = NULL;

		map<int, FormatterLayout*>::iterator i;
		LayoutRegion* bitMapRegion;
		LayoutRegion* ncmRegion;

		descriptor = executionObject->getDescriptor();
		if (descriptor != NULL) {
			layout = getFormatterLayout(descriptor, executionObject);
			if (layout != NULL) {
				if (descriptor->getFormatterRegion() != NULL) {
					plan = descriptor->getFormatterRegion()->getPlan();
				}

				windowId = layout->prepareFormatterRegion(
						executionObject, renderedSurface, plan);

				if (bitMapScreen != NULL) {
					/*clog << endl;
					clog << "FormatterMultiDevice::prepareFormatterRegion ";
					clog << "bitMapScreen != NULL" << endl;*/
					return windowId;
				}

				regionId = layout->getBitMapRegionId();
				/*clog << endl;
				clog << "FormatterMultiDevice::prepareFormatterRegion map '";
				clog << regionId << "'" << endl;*/

				if (regionId == "") {
					return windowId;
				}

				i = layoutManager.find(1);
				if (i == layoutManager.end()) {
					/*clog << endl;
					clog << "FormatterMultiDevice::prepareFormatterRegion ";
					clog << "CANT FIND devClass '" << deviceClass << "'";
					clog << endl;*/
					return windowId;
				}

				layout = i->second;

				ncmRegion = layout->getDeviceRegion();

				bitMapRegion = ncmRegion->getOutputMapRegion();
				if (bitMapRegion == NULL) {
					clog << endl;
					clog << "FormatterMultiDevice::prepareFormatterRegion(";
					clog << this << ") ";
					clog << "CANT FIND bitMapRegion";
					clog << " for id '" << regionId << "' devClass = '";
					clog << deviceClass << "'" << endl;
					clog << endl;
					return windowId;
				}

				bitMapScreen = dm->createWindow(
						myScreen,
						bitMapRegion->getAbsoluteLeft(),
						bitMapRegion->getAbsoluteTop(),
						bitMapRegion->getWidthInPixels(),
						bitMapRegion->getHeightInPixels(),
						bitMapRegion->getZIndexValue());

				clog << endl << endl;
				clog << "FormatterMultiDevice::prepareFormatterRegion(";
				clog << this << ") ";
				clog << "BITMAPREGION '";
				clog << regionId << "' left = '";
				clog << bitMapRegion->getLeftInPixels();
				clog << "' top = '" << bitMapRegion->getTopInPixels();
				clog << "' width = '" << bitMapRegion->getWidthInPixels();
				clog << "' height = '" << bitMapRegion->getHeightInPixels();
				clog << "' zIndex = '" << bitMapRegion->getZIndexValue();
				clog << endl << endl;

				bitMapScreen->setCaps(bitMapScreen->getCap("ALPHACHANNEL"));
				bitMapScreen->draw();
			}
		}

		return windowId;
	}

	void FormatterMultiDevice::showObject(ExecutionObject* executionObject) {
		FormatterLayout* layout;
		CascadingDescriptor* descriptor;
		FormatterRegion* fRegion;
		LayoutRegion* region;
		int devClass;
		string fileUri;
		string url;
		string relativePath;
		string tempRelPath;
		string value;
		Content* content;

		/*INCLSectionProcessor* nsp = NULL;
		vector<StreamData*>* streams;*/

		descriptor = executionObject->getDescriptor();
		if (descriptor != NULL) {
			region = descriptor->getRegion();
			layout = getFormatterLayout(descriptor, executionObject);
			if (region != NULL && layout != NULL) {
				devClass = region->getDeviceClass();

				clog << "FormatterMultiDevice::showObject '";
				clog << executionObject->getId() << "' class '";
				clog << devClass << "'" << endl;

				fRegion = descriptor->getFormatterRegion();
				if (devClass != IDeviceDomain::CT_BASE) {
					clog << "FormatterMultiDevice::showObject as base" << endl;

					if (fRegion != NULL) {
						fRegion->setGhostRegion(true);
					}
				}

				if (devClass != IDeviceDomain::CT_ACTIVE) {
					layout->showObject(executionObject);
				}

				if (hasRemoteDevices) {
					if (devClass == IDeviceDomain::CT_PASSIVE) {
						postMediaContent(devClass);

					} else if (devClass == IDeviceDomain::CT_ACTIVE) {
						//clog << "activeBaseUri: "<<activeBaseUri<<endl;
						//clog << "activeUris: "<<activeUris<<endl;

						content = ((NodeEntity*)(
								executionObject->getDataObject()->
										getDataEntity()))->getContent();

						tempRelPath = "";

						if (content != NULL && content->instanceOf(
								"ReferenceContent")) {

							url = ((ReferenceContent*)content)->
									getCompleteReferenceUrl();

							clog << "FormatterMultiDevice::showObject ";
							clog << "executionObject.url = '" << url;
							clog << "'" << endl;

							clog << "FormatterMultiDevice::showObject ";
							clog << "executionObject.activeBaseUri = '";
							clog << activeBaseUri << "'" << endl;

							size_t pos = url.find_last_of(SystemCompat::getIUriD());

							if(pos != string::npos)
								tempRelPath = url.substr(
												activeBaseUri.size(),
												url.size()-activeBaseUri.size());
							else
								tempRelPath = url;

							//relativePath = SystemCompat::convertRelativePath(tempRelPath);

							/*
							size_t pos = url.find_last_of(SystemCompat::getIUriD());
							if(pos != string::npos)
								relativePath = url.substr( pos + 1, url.size() - pos - 1 );
							else
								relativePath = url;
							*/
							clog << "FormatterMultiDevice::showObject ";
							clog << "executionObject.RP = '";
							clog << tempRelPath << "'" << endl;
						}
#if HAVE_MULTIDEVICE
						rdm->postEvent(devClass,
								IDeviceDomain::FT_PRESENTATIONEVENT,
								(char*)("start::" + tempRelPath).c_str(),
								("start::" + tempRelPath).size());

						/**streams = nsp->createNCLSections(
								"0x01.0x01",
								executionObject->getId(),
								activeBaseUri,
								activeUris,
								NULL);

						rdm->postNclMetadata(devClass, streams);

						fileUri = "start::" + executionObject->getId();*
						rdm->postEvent(
								devClass,
								IDeviceDomain::FT_PRESENTATIONEVENT,
								(char*)(fileUri.c_str()),
								fileUri.length());*/
#endif //HAVE_MULTIDEVICE
					}
				}
			}
		}
	}

	void FormatterMultiDevice::hideObject(ExecutionObject* executionObject) {
		FormatterLayout* layout;
		CascadingDescriptor* descriptor;
		LayoutRegion* region;
		int devClass;
		string fileUri;

		clog << "FormatterMultiDevice::hideObject '";
		clog << executionObject->getId() << "'" << endl;

		descriptor = executionObject->getDescriptor();
		if (descriptor != NULL) {
			region = descriptor->getRegion();
			layout = getFormatterLayout(descriptor, executionObject);
			if (region != NULL && layout != NULL) {
				devClass = region->getDeviceClass();
				if (devClass != IDeviceDomain::CT_ACTIVE) {
					/*clog << "FormatterMultiDevice::hideObject '";
					clog << executionObject->getId() << "' class '";
					clog << devClass << "'" << endl;*/

					layout->hideObject(executionObject);
				}

				if (hasRemoteDevices) {
					if (devClass == IDeviceDomain::CT_PASSIVE) {
						postMediaContent(devClass);

					} else if (devClass == IDeviceDomain::CT_ACTIVE) {
						Content* content;
						string relativePath = "";
						string url;

						content = ((NodeEntity*)(
								executionObject->getDataObject()->
										getDataEntity()))->getContent();

						if (content != NULL && content->instanceOf(
								"ReferenceContent")) {

							url = ((ReferenceContent*)content)->
									getCompleteReferenceUrl();

							/*clog << "FormatterMultiDevice::hideObject";
							clog << " executionObject.url = '" << url << "'";
							clog << " activeBaseUri = '" << activeBaseUri;
							clog << "'";
							clog << endl;*/

							relativePath = url.substr(
									activeBaseUri.size()+1,
									url.size() - activeBaseUri.size());

							/*clog << "FormatterMultiDevice::hideObject";
							clog << " executionObject.RP = '" << relativePath;
							clog << "'" << endl;*/
						}

						/*clog << "FormatterMultiDevice::hideObject";
						clog << " POSTING STOP EVENT";
						clog << endl;*/
#if HAVE_MULTIDEVICE
						rdm->postEvent(
								devClass,
								IDeviceDomain::FT_PRESENTATIONEVENT,
								(char*)("stop::" + relativePath).c_str(),
								("stop::" + relativePath).size());
#endif //HAVE_MULTIDEVICE
					}
				}
			}
		}
	}

	void FormatterMultiDevice::renderFromUri(IWindow* win, string uri) {
		ISurface* s;
		IPlayer* img;
		bool hasVisual = true;

#if HAVE_COMPONENTS
		img = ((PlayerCreator*)(cm->getObject(
				"ImagePlayer")))(myScreen, uri.c_str(), &hasVisual);
#else
		img = new ImagePlayer(myScreen, uri.c_str());
#endif

		img->setOutWindow(win->getId());

		win->setColorKey(0, 0, 0);
		win->clearContent();

		s = img->getSurface();
		win->renderFrom(s);

		win->show();
		win->validate();

		delete img;
	}

	void FormatterMultiDevice::tapObject(int devClass, int x, int y) {
		FormatterLayout* layout;
		ExecutionObject* object;

		if (layoutManager.count(devClass) != 0) {
			layout = layoutManager[devClass];

			object = layout->getObject(x, y);
			if (focusManager != NULL && object != NULL) {
				clog << "FormatterMultiDevice::tapObject '";
				clog << object->getId() << "'" << endl;

				focusManager->tapObject((void*)object);

//				object->selectionEvent(CodeMap::KEY_NULL, 0);

			} else {
				clog << "FormatterMultiDevice::tapObject can't ";
				clog << "find object at '" << x << "' and '";
				clog << y << "' coords" << endl;
			}

		} else {
			clog << "FormatterMultiDevice::tapObject can't find layout of '";
			clog << devClass << "' device class" << endl;
		}
	}

	bool FormatterMultiDevice::newDeviceConnected(
			int newDevClass,
			int w,
			int h) {

		bool isNewClass = false;
		/*INCLSectionProcessor* nsp = NULL;
		vector<StreamData*>* streams;*/

		clog << "FormatterMultiDevice::newDeviceConnected class '";
		clog << newDevClass << "', w = '" << w << "', h = '" << h << "'";
		clog << endl;

		if (presContext != NULL) {
			presContext->incPropertyValue(
					SYSTEM_DEVNUMBER + "(" + itos(newDevClass) + ")");
		}

		if (!hasRemoteDevices) {
			hasRemoteDevices = true;
		}

		if (layoutManager.count(newDevClass) == 0) {
			layoutManager[newDevClass] = new FormatterLayout(
					myScreen, 0, 0, w, h);

			isNewClass = true;
		}

		if (newDevClass == IDeviceDomain::CT_ACTIVE) {
			clog << "FormatterMulDevice::newDeviceConnected class = ";
			clog << IDeviceDomain::CT_ACTIVE << endl;

			/*streams = nsp->createNCLSections(
					"0x01.0x01",
					"nclApp",
					activeBaseUri,
					activeUris,
					NULL);

			rdm->postNclMetadata(newDevClass, streams);*/

		} else {
			postMediaContent(newDevClass);
		}

		return isNewClass;
	}

	bool FormatterMultiDevice::receiveRemoteEvent(
			int remoteDevClass,
			int eventType,
			string eventContent) {

		vector<string>* params;
		int eventCode;

		/*clog << "FormatterActiveDevice::receiveRemoteEvent from class '";
		clog << remoteDevClass << "', eventType '" << eventType << "', ";
		clog << "eventContent = '" << eventContent << "'" << endl;*/

		if (remoteDevClass == IDeviceDomain::CT_PASSIVE &&
				eventType == IDeviceDomain::FT_SELECTIONEVENT) {

			if (eventContent.find(",") != std::string::npos) {
				params = split(eventContent, ",");
				if (params != NULL) {
					if (params->size() == 3) {
						string strCode, strX, strY;
						strCode = (*params)[0];
						eventCode = CodeMap::getInstance()->getCode(strCode);
						if (eventCode == CodeMap::KEY_TAP) {
							strX    = (*params)[1];
							strY    = (*params)[2];

							tapObject(
									IDeviceDomain::CT_PASSIVE,
									(int)util::stof(strX),
									(int)util::stof(strY));

						} else if (eventCode != CodeMap::KEY_NULL) {
							im->postInputEvent(eventCode);
						}
					}
					delete params;
				}

			} else {
				eventCode = CodeMap::getInstance()->getCode(eventContent);
				if (eventCode != CodeMap::KEY_NULL) {
					im->postInputEvent(eventCode);
				}
			}
		}

		return true;
	}

	void FormatterMultiDevice::addActiveUris(
			string baseUri, vector<string>* uris) {

		if (activeUris != NULL) {
			delete activeUris;
		}

		activeUris    = uris;
		activeBaseUri = baseUri;
		clog << "FormatterMultiDevice::addActiveUris activeBaseUri="<<baseUri<<endl;
	}

	void FormatterMultiDevice::updatePassiveDevices() {
		postMediaContent(IDeviceDomain::CT_PASSIVE);
	}

	void FormatterMultiDevice::updateStatus(
			short code, string parameter, short type, string value) {

		switch(code) {
			case IPlayer::PL_NOTIFY_UPDATECONTENT:
				if (type == IPlayer::TYPE_PASSIVEDEVICE) {
					FormatterMultiDevice::updatePassiveDevices();
				}
				break;

			default:
				break;
		}
	}
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::ncl::multidevice::
		IFormatterMultiDevice* createFormatterMultiDevice(
				GingaScreenID screenId,
				IDeviceLayout* deviceLayout,
				int devClass,
				string playerId,
				int x,
				int y,
				int w,
				int h,
				bool useMulticast, int srvPort) {

    clog << "FormatterMultiDevice::createFormatter useMulticast = "<<(useMulticast?"TRUE":"FALSE")<<endl;

	if (devClass == IDeviceDomain::CT_BASE) {
		return (new ::br::pucrio::telemidia::ginga::ncl::multidevice::
				FormatterBaseDevice(screenId, deviceLayout, playerId, x, y, w, h, useMulticast, srvPort));

#if HAVE_MULTIDEVICE
	} else if (devClass == IDeviceDomain::CT_PASSIVE) {
		return (new ::br::pucrio::telemidia::ginga::ncl::multidevice::
				FormatterPassiveDevice(screenId, deviceLayout, x, y, w, h, useMulticast, srvPort));

	} else if (devClass == IDeviceDomain::CT_ACTIVE) {
		return (new ::br::pucrio::telemidia::ginga::ncl::multidevice::
				FormatterActiveDevice(screenId, deviceLayout, x, y, w, h, useMulticast, srvPort));
#endif //HAVE_MULTIDEVICE
	}

	return NULL;
}

extern "C" void destroyFormatterMultiDevice(
		::br::pucrio::telemidia::ginga::ncl::multidevice::
				IFormatterMultiDevice* fmd) {

	delete fmd;
}