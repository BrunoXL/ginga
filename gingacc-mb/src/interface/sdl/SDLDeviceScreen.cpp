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

#include "config.h"

#include "mb/interface/sdl/SDLDeviceScreen.h"

#include "mb/interface/sdl/output/SDLWindow.h"
#include "mb/interface/sdl/output/SDLSurface.h"

#include "mb/interface/sdl/input/SDLEventBuffer.h"
#include "mb/interface/sdl/input/SDLInputEvent.h"

#include "mb/ILocalScreenManager.h"
#include "mb/InputManager.h"

extern "C" {
#include "SDL_endian.h"
#include <string.h>
#include <stdlib.h>
}

#if !HAVE_COMPSUPPORT
#include "mb/interface/sdl/content/audio/SDLAudioProvider.h"
#include "mb/interface/sdl/content/image/SDLImageProvider.h"
#include "mb/interface/sdl/content/text/SDLFontProvider.h"
#include "mb/interface/sdl/content/video/SDLVideoProvider.h"
#endif

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace mb {
#if HAVE_COMPSUPPORT
	IComponentManager* SDLDeviceScreen::cm = IComponentManager::getCMInstance();
#endif

	map<SDLDeviceScreen*, short> SDLDeviceScreen::sdlScreens;
	pthread_mutex_t SDLDeviceScreen::sMutex;
	bool SDLDeviceScreen::hasRenderer                 = false;

	pthread_mutex_t SDLDeviceScreen::ieMutex;
	map<int, int> SDLDeviceScreen::gingaToSDLCodeMap;
	map<int, int> SDLDeviceScreen::sdlToGingaCodeMap;

	set<ReleaseContainer*> SDLDeviceScreen::releaseList;
	pthread_mutex_t SDLDeviceScreen::rlMutex;

	map<GingaScreenID, map<int, set<IWindow*>*>*> SDLDeviceScreen::renderMap;
	pthread_mutex_t SDLDeviceScreen::wrMutex;

	set<IContinuousMediaProvider*> SDLDeviceScreen::cmpRenderList;
	set<IDiscreteMediaProvider*> SDLDeviceScreen::dmpRenderList;
	pthread_mutex_t SDLDeviceScreen::mplMutex;

	const unsigned int SDLDeviceScreen::DSA_UNKNOWN = 0;
	const unsigned int SDLDeviceScreen::DSA_4x3     = 1;
	const unsigned int SDLDeviceScreen::DSA_16x9    = 2;

	SDLDeviceScreen::SDLDeviceScreen(
			int argc, char** args,
			GingaScreenID myId, GingaWindowID parentId) {

		int i;

		aSystem        = "";
		aspect         = DSA_UNKNOWN;
		hSize          = 0;
		vSize          = 0;
		hRes           = 0;
		wRes           = 0;
		im             = NULL;
		id             = myId;
		uParentId      = parentId;
		renderer       = NULL;
		mbMode         = "";
		mbSubSystem    = "";
		screen         = NULL;
		sdlId          = 0;
		backgroundLayer = NULL;

		for (i = 0; i < argc; i++) {
			if ((strcmp(args[i], "subsystem") == 0) && ((i + 1) < argc)) {
				mbSubSystem.assign(args[i + 1]);

			} else if ((strcmp(args[i], "mode") == 0) && ((i + 1) < argc)) {
				mbMode.assign(args[i + 1]);

			} else if ((strcmp(args[i], "audio") == 0) && ((i + 1) < argc)) {
				aSystem.assign(args[i + 1]);
			}
		}

		if (aSystem != "" && aSystem != "sdlffmpeg") {
			cout << "SDLDeviceScreen::SDLDeviceScreen Warning! Not ";
			cout << "supported audio system: '" << aSystem << "'! Using ";
			cout << "SDL2_ffmpeg instead." << endl;
		}
		aSystem = "SDLAudioProvider";

		if (mbSubSystem == "dfb") {
			mbSubSystem = "directfb";
		}

		waitingCreator = false;
		pthread_mutex_init(&cMutex, NULL);
		pthread_cond_init(&cond, NULL);

		uSur              = NULL;
		uSurPendingAction = SPA_NONE;
		pthread_mutex_init(&uSurMutex, NULL);

		pthread_mutex_init(&winMutex, NULL);
		pthread_mutex_init(&surMutex, NULL);
		pthread_mutex_init(&cmpMutex, NULL);
		pthread_mutex_init(&dmpMutex, NULL);

		if (!hasRenderer) {
			hasRenderer = true;
			pthread_mutex_init(&sMutex, NULL);
			pthread_mutex_init(&wrMutex, NULL);
			pthread_mutex_init(&rlMutex, NULL);
			pthread_mutex_init(&mplMutex, NULL);

			pthread_t tId;
			pthread_attr_t tattr;

			pthread_attr_init(&tattr);
			pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
			pthread_attr_setscope(&tattr, PTHREAD_SCOPE_SYSTEM);

			pthread_create(&tId, &tattr, SDLDeviceScreen::rendererT, this);
			pthread_detach(tId);
		}

		pthread_mutex_lock(&sMutex);
		sdlScreens[this] = STP_INIT;
		pthread_mutex_unlock(&sMutex);
	}

	SDLDeviceScreen::~SDLDeviceScreen() {
		map<SDLDeviceScreen*, short>::iterator i;
		map<GingaScreenID, map<int, set<IWindow*>*>*>::iterator j;
		map<int, set<IWindow*>*>::iterator k;

		waitingCreator = false;
		pthread_mutex_destroy(&cMutex);
		pthread_cond_destroy(&cond);

		uSur              = NULL;
		uSurPendingAction = SPA_NONE;
		pthread_mutex_destroy(&uSurMutex);

		pthread_mutex_lock(&sMutex);
		i = sdlScreens.find(this);
		if (i != sdlScreens.end()) {
			sdlScreens.erase(i);
		}
		pthread_mutex_unlock(&sMutex);

		pthread_mutex_lock(&wrMutex);
		j = renderMap.find(id);
		if (j != renderMap.end()) {
			k = j->second->begin();
			while (k != j->second->end()) {
				delete k->second;
				++k;
			}
			delete j->second;
			renderMap.erase(j);
		}
		pthread_mutex_unlock(&wrMutex);

		releaseScreen();

		pthread_mutex_destroy(&winMutex);
		pthread_mutex_destroy(&surMutex);
		pthread_mutex_destroy(&cmpMutex);
	}

	void SDLDeviceScreen::releaseScreen() {
		pthread_mutex_lock(&sMutex);
		sdlScreens[this] = STP_RELEASE;
		pthread_mutex_unlock(&sMutex);
	}

	void SDLDeviceScreen::releaseMB() {
		int errCount = 0;
		int numSDL;

		pthread_mutex_lock(&sMutex);
		numSDL = sdlScreens.size();
		pthread_mutex_unlock(&sMutex);

		while (numSDL > 1) {
			SystemCompat::uSleep(100000);
			errCount++;

			pthread_mutex_lock(&sMutex);
			numSDL = sdlScreens.size();
			pthread_mutex_unlock(&sMutex);

			if (errCount > 5 || numSDL <= 1) {
				break;
			}
		}

		pthread_mutex_lock(&mplMutex);
		hasRenderer = false;
		dmpRenderList.clear();
		cmpRenderList.clear();
		pthread_mutex_unlock(&mplMutex);

		pthread_mutex_destroy(&sMutex);
		pthread_mutex_destroy(&wrMutex);
		pthread_mutex_destroy(&rlMutex);
		pthread_mutex_destroy(&mplMutex);
	}

	void SDLDeviceScreen::clearWidgetPools() {
		pthread_mutex_lock(&sMutex);
		sdlScreens[this] = STP_CLEAR;
		pthread_mutex_unlock(&sMutex);
	}

	string SDLDeviceScreen::getScreenName() {
		return "sdl";
	}

	void SDLDeviceScreen::setParentScreen(GingaWindowID parentId) {

	}

	void SDLDeviceScreen::setBackgroundImage(string uri) {
		if (backgroundLayer == NULL) {
			backgroundLayer = createWindow(0, 0, wRes, hRes, -50000);
		}

		backgroundLayer->renderImgFile(uri);
		backgroundLayer->show();
	}

	unsigned int SDLDeviceScreen::getWidthResolution() {
		/*
		 * wRes == 0 is an initial state. So pthread_cond_t
		 * is not necessary here.
		 */
		while (wRes == 0) {
			SystemCompat::uSleep(1000000 / SDLDS_FPS);
		}

		return wRes;
	}

	void SDLDeviceScreen::setWidthResolution(unsigned int wRes) {
		this->wRes = wRes;

		if (screen != NULL) {
			SDL_SetWindowSize(screen, this->wRes, this->hRes);
		}
	}

	unsigned int SDLDeviceScreen::getHeightResolution() {
		/*
		 * hRes == 0 is an initial state. So pthread_cond_t
		 * is not necessary here.
		 */
		while (hRes == 0) {
			SystemCompat::uSleep(1000000 / SDLDS_FPS);
		}
		return hRes;
	}

	void SDLDeviceScreen::setHeightResolution(unsigned int hRes) {
		this->hRes = hRes;

		if (screen != NULL) {
			SDL_SetWindowSize(screen, this->wRes, this->hRes);
		}
	}

	void SDLDeviceScreen::setColorKey(int r, int g, int b) {

	}

	void SDLDeviceScreen::mergeIds(
			GingaWindowID destId, vector<GingaWindowID>* srcIds) {

	}

	void SDLDeviceScreen::blitScreen(ISurface* destination) {
		SDL_Surface* dest;

		dest = (SDL_Surface*)(destination->getSurfaceContent());
		if (dest == NULL) {
			dest = createUnderlyingSurface(wRes, hRes);
			destination->setSurfaceContent(dest);
		}

		blitScreen(dest);
	}

	void SDLDeviceScreen::blitScreen(string fileUri) {
		SDL_Surface* dest;

		dest = createUnderlyingSurface(wRes, hRes);
		blitScreen(dest);

		SDL_SaveBMP_RW(dest, SDL_RWFromFile(fileUri.c_str(), "wb"), 1);
	}

	void SDLDeviceScreen::blitScreen(SDL_Surface* dest) {
		pthread_mutex_lock(&uSurMutex);
		SDL_Surface* newUSur = NULL;

		uSurW             = wRes;
		uSurH             = hRes;
		uSur              = dest;
		uSurPendingAction = SPA_BLIT;
		waitSurfaceAction();

		pthread_mutex_unlock(&uSurMutex);
	}


	/* interfacing output */

	IWindow* SDLDeviceScreen::createWindow(int x, int y, int w, int h, int z) {
		IWindow* iWin;

		pthread_mutex_lock(&winMutex);
		iWin = new SDLWindow(NULL, NULL, id, x, y, w, h, z);
		windowPool.insert(iWin);
		renderMapInsertWindow(id, iWin, z);
		pthread_mutex_unlock(&winMutex);

		return iWin;
	}

	IWindow* SDLDeviceScreen::createWindowFrom(GingaWindowID underlyingWindow) {
		IWindow* iWin = NULL;

		if (underlyingWindow != NULL) {
			pthread_mutex_lock(&winMutex);
			iWin = new SDLWindow(underlyingWindow, NULL, id, 0, 0, 0, 0, 0);
			windowPool.insert(iWin);
			renderMapInsertWindow(id, iWin, 0);
			pthread_mutex_unlock(&winMutex);
		}

		return iWin;
	}

	bool SDLDeviceScreen::hasWindow(IWindow* win) {
		set<IWindow*>::iterator i;
		bool hasWin = false;

		pthread_mutex_lock(&winMutex);
		i = windowPool.find(win);
		if (i != windowPool.end()) {
			hasWin = true;
		}
		pthread_mutex_unlock(&winMutex);

		return hasWin;
	}

	void SDLDeviceScreen::releaseWindow(IWindow* win) {
		set<IWindow*>::iterator i;
		SDLWindow* iWin;
		SDL_Surface* uSur = NULL;
		SDL_Texture* uTex = NULL;

		pthread_mutex_lock(&winMutex);
		i = windowPool.find(win);
		if (i != windowPool.end()) {
			iWin = (SDLWindow*)(*i);

			renderMapRemoveWindow(id, iWin, iWin->getZ());
			windowPool.erase(i);

			uSur = (SDL_Surface*)(iWin->getContent());
			uTex = iWin->getTexture();

			iWin->clearContent();
			iWin->setTexture(NULL);

			createReleaseContainer(uSur, uTex, NULL);
		}

		pthread_mutex_unlock(&winMutex);
	}

	ISurface* SDLDeviceScreen::createSurface() {
		return createSurfaceFrom(NULL);
	}

	ISurface* SDLDeviceScreen::createSurface(int w, int h) {
		ISurface* iSur    = NULL;
		SDL_Surface* uSur = NULL;

		uSur = createUnderlyingSurface(w, h);

		pthread_mutex_lock(&surMutex);
		iSur = new SDLSurface(id, uSur);
		surfacePool.insert(iSur);
		pthread_mutex_unlock(&surMutex);

		return iSur;
	}

	ISurface* SDLDeviceScreen::createSurfaceFrom(void* uSur) {
		ISurface* iSur = NULL;

		pthread_mutex_lock(&surMutex);
		if (uSur != NULL) {
			iSur = new SDLSurface(id, uSur);

		} else {
			iSur = new SDLSurface(id);
		}

		surfacePool.insert(iSur);
		pthread_mutex_unlock(&surMutex);

		return iSur;
	}

	bool SDLDeviceScreen::hasSurface(ISurface* s) {
		set<ISurface*>::iterator i;
		bool hasSur = false;

		pthread_mutex_lock(&surMutex);
		i = surfacePool.find(s);
		if (i != surfacePool.end()) {
			hasSur = true;
		}
		pthread_mutex_unlock(&surMutex);

		return hasSur;
	}

	void SDLDeviceScreen::releaseSurface(ISurface* s) {
		set<ISurface*>::iterator i;
		SDL_Surface* uSur = NULL;

		pthread_mutex_lock(&surMutex);
		i = surfacePool.find(s);
		if (i != surfacePool.end()) {
			uSur = (SDL_Surface*)((*i)->getSurfaceContent());
			(*i)->setSurfaceContent(NULL);

			surfacePool.erase(i);

			createReleaseContainer(uSur, NULL, NULL);
		}
		pthread_mutex_unlock(&surMutex);
	}


	/* interfacing content */
	IContinuousMediaProvider* SDLDeviceScreen::createContinuousMediaProvider(
			const char* mrl, bool hasVisual, bool isRemote) {

		IContinuousMediaProvider* provider;
		string strSym;

		pthread_mutex_lock(&cmpMutex);
		if (hasVisual) {
			strSym = "SDLVideoProvider";

		} else {
			strSym = aSystem;
		}

#if HAVE_COMPSUPPORT
		provider = ((CMPCreator*)(cm->getObject(strSym)))(id, mrl);
		provider->setLoadSymbol(strSym);
#else
		provider = new SDLVideoProvider(id, mrl);
#endif

		cmpPool.insert(provider);

		pthread_mutex_unlock(&cmpMutex);
		return provider;
	}

	void SDLDeviceScreen::releaseContinuousMediaProvider(
			IContinuousMediaProvider* provider) {

		set<IContinuousMediaProvider*>::iterator i;
		IContinuousMediaProvider* cmp;

		pthread_mutex_lock(&cmpMutex);
		i = cmpPool.find(provider);
		if (i != cmpPool.end()) {
			cmp = (*i);
			cmpPool.erase(i);
			cmp->stop();

			createReleaseContainer(NULL, NULL, cmp);
		}
		pthread_mutex_unlock(&cmpMutex);
	}

	IFontProvider* SDLDeviceScreen::createFontProvider(
			const char* mrl, int fontSize) {

		IFontProvider* provider = NULL;

		pthread_mutex_lock(&cmpMutex);

#if HAVE_COMPSUPPORT
		provider = ((FontProviderCreator*)(cm->getObject("SDLFontProvider")))(
				id, mrl, fontSize);

#else
		provider = new SDLFontProvider(id, mrl, fontSize);
#endif

		dmpPool.insert(provider);
		pthread_mutex_unlock(&cmpMutex);

		return provider;
	}

	void SDLDeviceScreen::releaseFontProvider(IFontProvider* provider) {
		set<IDiscreteMediaProvider*>::iterator i;
		IDiscreteMediaProvider* dmp;

		pthread_mutex_lock(&dmpMutex);
		i = dmpPool.find(provider);
		if (i != dmpPool.end()) {
			dmp = (*i);
			dmpPool.erase(i);

			createReleaseContainer(NULL, NULL, dmp);
		}
		pthread_mutex_unlock(&dmpMutex);
	}

	IImageProvider* SDLDeviceScreen::createImageProvider(const char* mrl) {
		IImageProvider* provider = NULL;

		pthread_mutex_lock(&dmpMutex);
#if HAVE_COMPSUPPORT
		provider = ((ImageProviderCreator*)(cm->getObject(
				"SDLImageProvider")))(id, mrl);
#else
		provider = new SDLImageProvider(id, mrl);
#endif

		dmpPool.insert(provider);
		pthread_mutex_unlock(&dmpMutex);

		return provider;
	}

	void SDLDeviceScreen::releaseImageProvider(IImageProvider* provider) {
		set<IDiscreteMediaProvider*>::iterator i;
		IDiscreteMediaProvider* dmp;
		ReleaseContainer rc;

		pthread_mutex_lock(&dmpMutex);
		i = dmpPool.find(provider);
		if (i != dmpPool.end()) {
			dmp = (*i);
			dmpPool.erase(i);

			createReleaseContainer(NULL, NULL, dmp);
		}

		pthread_mutex_unlock(&dmpMutex);
	}

	ISurface* SDLDeviceScreen::createRenderedSurfaceFromImageFile(
			const char* mrl) {

		ISurface* iSur           = NULL;
		IImageProvider* provider = NULL;

		provider = createImageProvider(mrl);
		if (provider != NULL) {
			iSur = createSurfaceFrom(NULL);
			provider->playOver(iSur);
		}

		releaseImageProvider(provider);

		return iSur;
	}

	void SDLDeviceScreen::addCMPToRendererList(IContinuousMediaProvider* cmp) {
		pthread_mutex_lock(&mplMutex);
		cmpRenderList.insert(cmp);
		pthread_mutex_unlock(&mplMutex);
	}

	void SDLDeviceScreen::removeCMPToRendererList(
			IContinuousMediaProvider* cmp) {

		set<IContinuousMediaProvider*>::iterator i;

		pthread_mutex_lock(&mplMutex);
		i = cmpRenderList.find(cmp);
		if (i != cmpRenderList.end()) {
			cmpRenderList.erase(i);
		}
		pthread_mutex_unlock(&mplMutex);
	}

	void SDLDeviceScreen::addDMPToRendererList(IDiscreteMediaProvider* dmp) {
		pthread_mutex_lock(&mplMutex);
		dmpRenderList.insert(dmp);
		pthread_mutex_unlock(&mplMutex);
	}

	void SDLDeviceScreen::removeDMPToRendererList(IDiscreteMediaProvider* dmp) {
		set<IDiscreteMediaProvider*>::iterator i;

		pthread_mutex_lock(&mplMutex);
		i = dmpRenderList.find(dmp);
		if (i != dmpRenderList.end()) {
			dmpRenderList.erase(i);
		}
		pthread_mutex_unlock(&mplMutex);
	}

	void SDLDeviceScreen::createReleaseContainer(
			SDL_Surface* uSur,
			SDL_Texture* uTex,
			IMediaProvider* iDec) {

		ReleaseContainer rc;

		rc.iDec = iDec;
		rc.uSur = uSur;
		rc.uTex = uTex;

		pthread_mutex_lock(&rlMutex);
		releaseList.insert(&rc);
		pthread_mutex_unlock(&rlMutex);
	}

	void SDLDeviceScreen::refreshRC(SDLDeviceScreen* s) {
		set<ReleaseContainer*>::iterator i;
		string strSym;

		pthread_mutex_lock(&s->rlMutex);
		i = s->releaseList.begin();
		while (i != s->releaseList.end()) {
			if ((*i)->iDec != NULL) {
				strSym = (*i)->iDec->getLoadSymbol();

				delete (*i)->iDec;

#if HAVE_COMPSUPPORT
				cm->releaseComponentFromObject(strSym);
#endif
			}

			if ((*i)->uSur != NULL) {
				releaseUnderlyingSurface((*i)->uSur);
			}

			if ((*i)->uTex != NULL) {
				releaseTexture((*i)->uTex);
			}

			++i;
		}

		s->releaseList.clear();
		pthread_mutex_unlock(&s->rlMutex);
	}

	int SDLDeviceScreen::refreshCMP(SDLDeviceScreen* s) {
		set<IContinuousMediaProvider*>::iterator i;
		set<IContinuousMediaProvider*>::iterator j;

		int size;

		pthread_mutex_lock(&s->cmpMutex);
		pthread_mutex_lock(&mplMutex);
		size = cmpRenderList.size();
		i = cmpRenderList.begin();
		while (i != cmpRenderList.end()) {
			j = s->cmpPool.find(*i);
			if (j != s->cmpPool.end()) {
				if ((*i)->getHasVisual() &&
						(*i)->getProviderContent() == NULL) {

					initCMP(s, (*i));
				}
				(*i)->refreshDR();
			}
			++i;
		}
		pthread_mutex_unlock(&mplMutex);
		pthread_mutex_unlock(&s->cmpMutex);

		return size;
	}

	void SDLDeviceScreen::refreshDMP(SDLDeviceScreen* s) {
		set<IDiscreteMediaProvider*>::iterator i;
		set<IDiscreteMediaProvider*>::iterator j;
		IDiscreteMediaProvider* dmp;
		ISurface* surface;

		pthread_mutex_lock(&s->dmpMutex);
		pthread_mutex_lock(&mplMutex);
		i = dmpRenderList.begin();
		while (i != dmpRenderList.end()) {
			dmp = (*i);
			j = s->dmpPool.find(dmp);
			if (j != s->dmpPool.end()) {
				dmp->ntsPlayOver();
				dmpRenderList.erase(i);
				i = dmpRenderList.begin();

			} else {
				++i;
			}
		}
		pthread_mutex_unlock(&mplMutex);
		pthread_mutex_unlock(&s->dmpMutex);
	}

	void SDLDeviceScreen::refreshWin(SDLDeviceScreen* s) {
		SDL_Surface* uSur;
		SDL_Texture* uTex;
		SDLWindow* win;
		bool ownTex = false;

		map<GingaScreenID, map<int, set<IWindow*>*>*>::iterator i;
		map<int, set<IWindow*>*>::iterator j;
		set<IWindow*>::iterator k;

		pthread_mutex_lock(&s->winMutex);
		pthread_mutex_lock(&wrMutex);

		SDL_RenderClear(s->renderer);

		i = renderMap.find(s->id);
		if (i != renderMap.end()) {
			j = i->second->begin();
			while (j != i->second->end()) {
				k = j->second->begin();
				while (k != j->second->end()) {
					win = (SDLWindow*)(*k);

					if (s->windowPool.find(win) != s->windowPool.end() &&
							win->isVisible()) {

						uSur   = (SDL_Surface*)(win->getContent());

						if (uSur != NULL) {
							ownTex = false;
							uTex   = createTextureFromSurface(
									s->renderer, uSur);

						} else {
							ownTex = true;
							uTex   = win->getTexture();
						}

						drawWindow(s->renderer, uTex, win);
						if (uTex != NULL) {
							if (!ownTex) {
								releaseTexture(uTex);
								ownTex = false;
							}
							uTex = NULL;
						}

						win->rendered();
					}
					++k;
				}
				++j;
			}
		}
		SDL_RenderPresent(s->renderer);
		pthread_mutex_unlock(&wrMutex);
		pthread_mutex_unlock(&s->winMutex);
	}

	void* SDLDeviceScreen::rendererT(void* ptr) {
		map<SDLDeviceScreen*, short>::iterator i;
		SDL_Event event;
		int sleepTime, elapsedTime, decRate;
		bool shiftOn = false;
		bool capsOn  = false;
		SDLEventBuffer* eventBuffer = NULL;

		sleepTime = (int)(1000000/SDLDS_FPS);

		SDL_Init((Uint32)(
				SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE));

		while (hasRenderer) {
			elapsedTime = getCurrentTimeMillis();

		    while (SDL_PollEvent(&event)) {
		    	pthread_mutex_lock(&sMutex);

				if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_LSHIFT ||
							event.key.keysym.sym == SDLK_RSHIFT) {

						shiftOn = true;
					}

				} else  if (event.type == SDL_KEYUP) {
					if (event.key.keysym.sym == SDLK_CAPSLOCK) {
						capsOn = !capsOn;

					} else if (event.key.keysym.sym == SDLK_LSHIFT ||
							event.key.keysym.sym == SDLK_RSHIFT) {

						shiftOn = false;
					}
				}

				i = sdlScreens.begin();
				while (i != sdlScreens.end()) {
					if (i->first->im != NULL) {
						eventBuffer = (SDLEventBuffer*)(
								i->first->im->getEventBuffer());

						cout << "Event" << endl;
						if ((SDLEventBuffer::checkEvent(i->first->sdlId, event)
								|| checkEventFocus(i->first))) {

							eventBuffer->feed(event, capsOn, shiftOn);
						}
					}
					++i;
				}
				pthread_mutex_unlock(&sMutex);
	    	}

			pthread_mutex_lock(&sMutex);
			i = sdlScreens.begin();
			while (i != sdlScreens.end()) {
				switch (i->second) {
					case STP_NONE:
						surfaceAction(i->first);
						decRate = refreshCMP(i->first);
						refreshDMP(i->first);
						refreshWin(i->first);

						++i;
						break;

					case STP_INIT:
						initScreen(i->first);
						sdlScreens[i->first] = STP_NONE;
						i = sdlScreens.begin();
						break;

					case STP_CLEAR:
						clearScreen(i->first);
						sdlScreens[i->first] = STP_NONE;
						i = sdlScreens.begin();
						break;

					case STP_RELEASE:
						releaseScreen(i->first);
						sdlScreens.erase(i);
						i = sdlScreens.begin();
						break;

					default:
						++i;
						break;
				}
			}
			pthread_mutex_unlock(&sMutex);

			elapsedTime = (getCurrentTimeMillis() - elapsedTime) * 1000;

			if (elapsedTime < sleepTime) {
				if (decRate == 0) {
					SystemCompat::uSleep(sleepTime - elapsedTime);

				} else {
					SystemCompat::uSleep((sleepTime - elapsedTime) / (10 * decRate));
				}
			}
		}

		SDL_Quit();

		return NULL;
	}

	void SDLDeviceScreen::initScreen(SDLDeviceScreen* s) {
		SDL_Rect rect;
		int i, numOfDrivers, x, y;
		string title = "";

		SDL_GetDisplayBounds(0, &rect);

		if (s->mbSubSystem != "") {
			numOfDrivers = SDL_GetNumVideoDrivers();
			for (i = 0; i < numOfDrivers; i++) {
				if (strcmp(
						SDL_GetVideoDriver(i), s->mbSubSystem.c_str()) == 0) {

					SDL_VideoInit(s->mbSubSystem.c_str());
					break;
				}
			}
		}

		if (s->uParentId != NULL) {
			s->screen = SDL_CreateWindowFrom(s->uParentId);
			if (s->screen != NULL) {
				SDL_GetWindowSize(s->screen, &s->wRes, &s->hRes);
				s->sdlId = SDL_GetWindowID(s->screen);
				SDL_SetWindowGrab(s->screen, SDL_TRUE);
			}

		} else {
			title.assign((char*)VERSION);
			title = "Ginga v" + title;

			if (s->mbMode != "" && s->mbMode.find("x") != std::string::npos) {
				s->wRes = (int)stof(
						s->mbMode.substr(0, s->mbMode.find_first_of("x")));

				s->hRes = (int)stof(
						s->mbMode.substr(
								s->mbMode.find_first_of("x") + 1,
								(s->mbMode.length() -
										(s->mbMode.find_first_of("x")) + 1)));

			} else {
				s->wRes = 1280;
				s->hRes = 720;
			}

			if (s->wRes <= 0 || s->wRes > rect.w) {
				s->wRes = 0.9 * rect.w;
			}

			if (s->hRes <= 0 || s->hRes > rect.h) {
				s->hRes = 0.9 * rect.h;
			}

			x = (rect.w - s->wRes) / 2;
			y = (rect.h - s->hRes) / 2;

			s->screen = SDL_CreateWindow(
					title.c_str(), x, y, s->wRes, s->hRes, 0);

			s->sdlId = SDL_GetWindowID(s->screen);
		}

		if (s->screen != NULL) {
			s->renderer = SDL_CreateRenderer(
					s->screen, -1, SDL_RENDERER_ACCELERATED);

			if (s->renderer == NULL) {
				s->renderer = SDL_CreateRenderer(
						s->screen, -1, SDL_RENDERER_SOFTWARE);
			}
		}

		initCodeMaps();
		s->im = new InputManager(s->id);
		s->im->setAxisBoundaries(s->wRes, s->hRes, 0);
	}

	void SDLDeviceScreen::clearScreen(SDLDeviceScreen* s) {
		IWindow* iWin;
		ISurface* iSur;
		IContinuousMediaProvider* iCmp;
		IDiscreteMediaProvider* iDmp;

		set<IWindow*>* winClone;

		set<IWindow*>::iterator i;
		set<ISurface*>::iterator j;
		set<IContinuousMediaProvider*>::iterator k;
		set<IDiscreteMediaProvider*>::iterator l;

		clog << "SDLDeviceScreen::clearWidgetPools ";
		clog << "windowPool size = " << s->windowPool.size();
		clog << ", surfacePool size = " << s->surfacePool.size();
		clog << endl;

		//Releasing remaining Window objects in Window Pool
		pthread_mutex_lock(&s->winMutex);
		winClone = new set<IWindow*>(s->windowPool);
		s->windowPool.clear();
		pthread_mutex_unlock(&s->winMutex);

		i = winClone->begin();
		while (i != winClone->end()) {
			iWin = (*i);
			if (iWin != NULL) {
				delete iWin;
			}
			++i;
		}
		delete winClone;

		//Releasing remaining Surface objects in Surface Pool
		pthread_mutex_lock(&s->surMutex);
		j = s->surfacePool.begin();
		while (j != s->surfacePool.end()) {
			iSur = (*j);

			s->surfacePool.erase(j);
			if (iSur != NULL) {
				pthread_mutex_unlock(&s->surMutex);
				delete iSur;
				pthread_mutex_lock(&s->surMutex);
			}
			j = s->surfacePool.begin();
		}
		s->surfacePool.clear();
		pthread_mutex_unlock(&s->surMutex);

		//Releasing remaining CMP objects in CMP Pool
		pthread_mutex_lock(&s->cmpMutex);
		k = s->cmpPool.begin();
		while (k != s->cmpPool.end()) {
			iCmp = (*k);

			s->cmpPool.erase(k);
			if (iCmp != NULL) {
				pthread_mutex_unlock(&s->cmpMutex);
				delete iCmp;
				pthread_mutex_lock(&s->cmpMutex);
			}
			k = s->cmpPool.begin();
		}
		s->cmpPool.clear();
		pthread_mutex_unlock(&s->cmpMutex);

		//Releasing remaining DMP objects in DMP Pool
		pthread_mutex_lock(&s->dmpMutex);
		l = s->dmpPool.begin();
		while (l != s->dmpPool.end()) {
			iDmp = *l;

			s->dmpPool.erase(l);
			if (iDmp != NULL) {
				pthread_mutex_unlock(&s->dmpMutex);
				delete iDmp;
				pthread_mutex_lock(&s->dmpMutex);
			}
			l = s->dmpPool.begin();
		}
		s->dmpPool.clear();
		pthread_mutex_unlock(&s->dmpMutex);
	}

	void SDLDeviceScreen::releaseScreen(SDLDeviceScreen* s) {
		clearScreen(s);

		if (s->uParentId == NULL && s->screen != NULL) {
			SDL_HideWindow(s->screen);
		}

		if (s->renderer != NULL) {
			SDL_DestroyRenderer(s->renderer);
			s->renderer = NULL;
		}

		if (s->uParentId == NULL && s->screen != NULL) {
			SDL_DestroyWindow(s->screen);
			s->screen = NULL;
		}
	}

	bool SDLDeviceScreen::surfaceAction(SDLDeviceScreen* s) {
		bool hasAction = true;
		map<GingaScreenID, map<int, set<IWindow*>*>*>::iterator i;
		map<int, set<IWindow*>*>::iterator j;
		set<IWindow*>::iterator k;
		Uint32 rmask, gmask, bmask, amask;

		switch (s->uSurPendingAction) {
			case SPA_CREATE:
				getRGBAMask(24, &rmask, &gmask, &bmask, &amask);
				s->uSur = SDL_CreateRGBSurface(
						0, s->uSurW, s->uSurH, 24, rmask, gmask, bmask, amask);

				break;

			case SPA_BLIT:
				pthread_mutex_lock(&wrMutex);
				pthread_mutex_lock(&s->winMutex);
				i = renderMap.find(s->id);
				if (i != renderMap.end()) {
					j = i->second->begin();
					while (j != i->second->end()) {
						k = j->second->begin();
						while (k != j->second->end()) {
							blitFromWindow((*k), s->uSur);
							++k;
						}
						++j;
					}
				}
				pthread_mutex_unlock(&s->winMutex);
				pthread_mutex_unlock(&wrMutex);
				break;

			case SPA_NONE:
			default:
				hasAction = false;
				break;
		}

		if (hasAction) {
			s->uSurPendingAction = SPA_NONE;
			s->surfaceActionExecuted();
		}

		return hasAction;
	}

	void SDLDeviceScreen::initCMP(
			SDLDeviceScreen* s, IContinuousMediaProvider* cmp) {

		SDL_Texture* texture;
		int w, h;

		cmp->getOriginalResolution(&w, &h);

		clog << "SDLDeviceScreen::initCMP creating texture with w = '";
		clog << w << "' and h = '" << h << "'" << endl;

		texture = createTexture(s->renderer, w, h);
		cmp->setProviderContent((void*)texture);
	}

	void SDLDeviceScreen::blitFromWindow(IWindow* iWin, SDL_Surface* dest) {
		SDL_Surface* tmpSur;
		SDL_Texture* tmpTex;
		SDL_Rect rect;

		bool freeSurface = false;

		tmpTex = ((SDLWindow*)iWin)->getTexture();
		if (tmpTex != NULL) {
			tmpSur = createUnderlyingSurfaceFromTexture(tmpTex);
			freeSurface = true;

		} else {
			tmpSur = (SDL_Surface*)(iWin->getContent());
		}

		if (tmpSur != NULL) {
			rect.x = iWin->getX();
			rect.y = iWin->getY();
			rect.w = iWin->getW();
			rect.h = iWin->getH();

			SDL_UpperBlitScaled(tmpSur, NULL, dest, &rect);
		}

		if (freeSurface) {
			freeSurface = false;
			releaseUnderlyingSurface(tmpSur);
		}
	}


	/* interfacing input */

	IInputManager* SDLDeviceScreen::getInputManager() {
		/*
		 * im == NULL is an initial state. So pthread_cond_t
		 * is not necessary here.
		 */
		while (im == NULL) {
			SystemCompat::uSleep(1000000 / SDLDS_FPS);
		}
		return im;
	}

	IEventBuffer* SDLDeviceScreen::createEventBuffer() {
		return new SDLEventBuffer(id);
	}

	IInputEvent* SDLDeviceScreen::createInputEvent(
			void* event, const int symbol) {

		if (event != NULL) {
			return new SDLInputEvent(*(SDL_Event*)event);
		}

		if (symbol >= 0) {
			return new SDLInputEvent(symbol);
		}

		return NULL;
	}

	IInputEvent* SDLDeviceScreen::createApplicationEvent(int type, void* data) {
		return new SDLInputEvent(type, data);
	}

	int SDLDeviceScreen::fromMBToGinga(int keyCode) {
		map<int, int>::iterator i;
		int translated = CodeMap::KEY_NULL;

		pthread_mutex_lock(&ieMutex);
		i = sdlToGingaCodeMap.find(keyCode);
		if (i != sdlToGingaCodeMap.end()) {
			translated = i->second;
		}
		pthread_mutex_unlock(&ieMutex);

		return translated;
	}

	int SDLDeviceScreen::fromGingaToMB(int keyCode) {
		map<int, int>::iterator i;
		int translated = CodeMap::KEY_NULL;

		pthread_mutex_lock(&ieMutex);
		i = gingaToSDLCodeMap.find(keyCode);
		if (i != gingaToSDLCodeMap.end()) {
			translated = i->second;
		}
		pthread_mutex_unlock(&ieMutex);

		return translated;
	}


	/* interfacing underlying multimedia system */

	void* SDLDeviceScreen::getGfxRoot() {
		return renderer;
	}

	/* libgingaccmbsdl internal use*/

	/* input */
	void SDLDeviceScreen::initCodeMaps() {
		if (!gingaToSDLCodeMap.empty()) {
			return;
		}

		pthread_mutex_init(&ieMutex, NULL);

		gingaToSDLCodeMap[CodeMap::KEY_NULL]              = SDLK_UNKNOWN;
		gingaToSDLCodeMap[CodeMap::KEY_0]                 = SDLK_0;
		gingaToSDLCodeMap[CodeMap::KEY_1]                 = SDLK_1;
		gingaToSDLCodeMap[CodeMap::KEY_2]                 = SDLK_2;
		gingaToSDLCodeMap[CodeMap::KEY_3]                 = SDLK_3;
		gingaToSDLCodeMap[CodeMap::KEY_4]                 = SDLK_4;
		gingaToSDLCodeMap[CodeMap::KEY_5]                 = SDLK_5;
		gingaToSDLCodeMap[CodeMap::KEY_6]                 = SDLK_6;
		gingaToSDLCodeMap[CodeMap::KEY_7]                 = SDLK_7;
		gingaToSDLCodeMap[CodeMap::KEY_8]                 = SDLK_8;
		gingaToSDLCodeMap[CodeMap::KEY_9]                 = SDLK_9;

		gingaToSDLCodeMap[CodeMap::KEY_SMALL_A]           = SDLK_a;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_B]           = SDLK_b;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_C]           = SDLK_c;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_D]           = SDLK_d;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_E]           = SDLK_e;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_F]           = SDLK_f;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_G]           = SDLK_g;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_H]           = SDLK_h;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_I]           = SDLK_i;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_J]           = SDLK_j;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_K]           = SDLK_k;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_L]           = SDLK_l;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_M]           = SDLK_m;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_N]           = SDLK_n;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_O]           = SDLK_o;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_P]           = SDLK_p;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_Q]           = SDLK_q;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_R]           = SDLK_r;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_S]           = SDLK_s;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_T]           = SDLK_t;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_U]           = SDLK_u;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_V]           = SDLK_v;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_W]           = SDLK_w;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_X]           = SDLK_x;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_Y]           = SDLK_y;
		gingaToSDLCodeMap[CodeMap::KEY_SMALL_Z]           = SDLK_z;

		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_A]         = SDLK_a + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_B]         = SDLK_b + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_C]         = SDLK_c + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_D]         = SDLK_d + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_E]         = SDLK_e + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_F]         = SDLK_f + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_G]         = SDLK_g + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_H]         = SDLK_h + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_I]         = SDLK_i + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_J]         = SDLK_j + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_K]         = SDLK_k + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_L]         = SDLK_l + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_M]         = SDLK_m + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_N]         = SDLK_n + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_O]         = SDLK_o + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_P]         = SDLK_p + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_Q]         = SDLK_q + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_R]         = SDLK_r + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_S]         = SDLK_s + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_T]         = SDLK_t + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_U]         = SDLK_u + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_V]         = SDLK_v + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_W]         = SDLK_w + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_X]         = SDLK_x + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_Y]         = SDLK_y + 5000;
		gingaToSDLCodeMap[CodeMap::KEY_CAPITAL_Z]         = SDLK_z + 5000;

		gingaToSDLCodeMap[CodeMap::KEY_PAGE_DOWN]         = SDLK_PAGEDOWN;
		gingaToSDLCodeMap[CodeMap::KEY_PAGE_UP]           = SDLK_PAGEUP;

		gingaToSDLCodeMap[CodeMap::KEY_F1]                = SDLK_F1;
		gingaToSDLCodeMap[CodeMap::KEY_F2]                = SDLK_F2;
		gingaToSDLCodeMap[CodeMap::KEY_F3]                = SDLK_F3;
		gingaToSDLCodeMap[CodeMap::KEY_F4]                = SDLK_F4;
		gingaToSDLCodeMap[CodeMap::KEY_F5]                = SDLK_F5;
		gingaToSDLCodeMap[CodeMap::KEY_F6]                = SDLK_F6;
		gingaToSDLCodeMap[CodeMap::KEY_F7]                = SDLK_F7;
		gingaToSDLCodeMap[CodeMap::KEY_F8]                = SDLK_F8;
		gingaToSDLCodeMap[CodeMap::KEY_F9]                = SDLK_F9;
		gingaToSDLCodeMap[CodeMap::KEY_F10]               = SDLK_F10;
		gingaToSDLCodeMap[CodeMap::KEY_F11]               = SDLK_F11;
		gingaToSDLCodeMap[CodeMap::KEY_F12]               = SDLK_F12;

		gingaToSDLCodeMap[CodeMap::KEY_PLUS_SIGN]         = SDLK_PLUS;
		gingaToSDLCodeMap[CodeMap::KEY_MINUS_SIGN]        = SDLK_MINUS;

		gingaToSDLCodeMap[CodeMap::KEY_ASTERISK]          = SDLK_ASTERISK;
		gingaToSDLCodeMap[CodeMap::KEY_NUMBER_SIGN]       = SDLK_HASH;

		gingaToSDLCodeMap[CodeMap::KEY_PERIOD]            = SDLK_PERIOD;

		gingaToSDLCodeMap[CodeMap::KEY_SUPER]             = SDLK_CAPSLOCK;
		gingaToSDLCodeMap[CodeMap::KEY_PRINTSCREEN]       = SDLK_PRINTSCREEN;
		gingaToSDLCodeMap[CodeMap::KEY_MENU]              = SDLK_MENU;
		gingaToSDLCodeMap[CodeMap::KEY_INFO]              = SDLK_F14;
		gingaToSDLCodeMap[CodeMap::KEY_EPG]               = SDLK_QUESTION;

		gingaToSDLCodeMap[CodeMap::KEY_CURSOR_DOWN]       = SDLK_DOWN;
		gingaToSDLCodeMap[CodeMap::KEY_CURSOR_LEFT]       = SDLK_LEFT;
		gingaToSDLCodeMap[CodeMap::KEY_CURSOR_RIGHT]      = SDLK_RIGHT;
		gingaToSDLCodeMap[CodeMap::KEY_CURSOR_UP]         = SDLK_UP;

		gingaToSDLCodeMap[CodeMap::KEY_CHANNEL_DOWN]      = SDLK_F15;
		gingaToSDLCodeMap[CodeMap::KEY_CHANNEL_UP]        = SDLK_F16;

		gingaToSDLCodeMap[CodeMap::KEY_VOLUME_DOWN]       = SDLK_VOLUMEDOWN;
		gingaToSDLCodeMap[CodeMap::KEY_VOLUME_UP]         = SDLK_VOLUMEUP;

		gingaToSDLCodeMap[CodeMap::KEY_ENTER]             = SDLK_RETURN;
		gingaToSDLCodeMap[CodeMap::KEY_OK]                = SDLK_RETURN2;

		gingaToSDLCodeMap[CodeMap::KEY_RED]               = SDLK_F17;
		gingaToSDLCodeMap[CodeMap::KEY_GREEN]             = SDLK_F18;
		gingaToSDLCodeMap[CodeMap::KEY_YELLOW]            = SDLK_F19;
		gingaToSDLCodeMap[CodeMap::KEY_BLUE]              = SDLK_F20;

		gingaToSDLCodeMap[CodeMap::KEY_SPACE]             = SDLK_SPACE;
		gingaToSDLCodeMap[CodeMap::KEY_BACKSPACE]         = SDLK_BACKSPACE;
		gingaToSDLCodeMap[CodeMap::KEY_BACK]              = SDLK_AC_BACK;
		gingaToSDLCodeMap[CodeMap::KEY_ESCAPE]            = SDLK_ESCAPE;
		gingaToSDLCodeMap[CodeMap::KEY_EXIT]              = SDLK_OUT;

		gingaToSDLCodeMap[CodeMap::KEY_POWER]             = SDLK_POWER;
		gingaToSDLCodeMap[CodeMap::KEY_REWIND]            = SDLK_F21;
		gingaToSDLCodeMap[CodeMap::KEY_STOP]              = SDLK_STOP;
		gingaToSDLCodeMap[CodeMap::KEY_EJECT]             = SDLK_EJECT;
		gingaToSDLCodeMap[CodeMap::KEY_PLAY]              = SDLK_EXECUTE;
		gingaToSDLCodeMap[CodeMap::KEY_RECORD]            = SDLK_F22;
		gingaToSDLCodeMap[CodeMap::KEY_PAUSE]             = SDLK_PAUSE;

		gingaToSDLCodeMap[CodeMap::KEY_GREATER_THAN_SIGN] = SDLK_GREATER;
		gingaToSDLCodeMap[CodeMap::KEY_LESS_THAN_SIGN]    = SDLK_LESS;

		gingaToSDLCodeMap[CodeMap::KEY_TAB]               = SDLK_TAB;
		gingaToSDLCodeMap[CodeMap::KEY_TAP]               = SDLK_F23;

        map<int, int>::iterator i;
        i = gingaToSDLCodeMap.begin();
        while (i != gingaToSDLCodeMap.end()) {
		    sdlToGingaCodeMap[i->second] = i->first;
		    ++i;
        }
	}

	bool SDLDeviceScreen::checkEventFocus(SDLDeviceScreen* s) {
		int x, y;
		int winX, winY;
		bool hasFocus = false;

		if (s->uParentId != NULL) {
			x = s->im->getCurrentXAxisValue();
			y = s->im->getCurrentYAxisValue();

			SDL_GetWindowPosition(s->screen, &winX, &winY);

			cout << "SDLDeviceScreen::checkEventFocus ";
			cout << "mouseXY = '" << x << "," << y << "' and windowXYWH = '";
			cout << winX << "," << winY << "," << s->wRes << "," << s->hRes;
			cout << "'";
			cout << endl;
			if (x >= winX && x <= s->wRes &&
					y >= winY && y <= s->hRes) {

				hasFocus = true;
			}
		}

		return hasFocus;
	}


	/* output */
	void SDLDeviceScreen::renderMapInsertWindow(
			GingaScreenID screenId, IWindow* iWin, int z) {

		map<GingaScreenID, map<int, set<IWindow*>*>*>::iterator i;
		map<int, set<IWindow*>*>::iterator j;

		map<int, set<IWindow*>*>* sortedMap;
		set<IWindow*>* windows;

		pthread_mutex_lock(&wrMutex);
		i = renderMap.find(screenId);
		if (i != renderMap.end()) {
			sortedMap = i->second;
		} else {
			sortedMap = new map<int, set<IWindow*>*>;
			renderMap[screenId] = sortedMap;
		}

		j = sortedMap->find(z);
		if (j != sortedMap->end()) {
			windows = j->second;
		} else {
			windows = new set<IWindow*>;
			(*sortedMap)[z] = windows;
		}

		windows->insert(iWin);
		pthread_mutex_unlock(&wrMutex);
	}

	void SDLDeviceScreen::renderMapRemoveWindow(
			GingaScreenID screenId, IWindow* iWin, int z) {

		map<GingaScreenID, map<int, set<IWindow*>*>*>::iterator i;
		map<int, set<IWindow*>*>::iterator j;
		set<IWindow*>::iterator k;

		map<int, set<IWindow*>*>* sortedMap;
		set<IWindow*>* windows;

		pthread_mutex_lock(&wrMutex);
		i = renderMap.find(screenId);
		if (i != renderMap.end()) {
			sortedMap = i->second;
			j = sortedMap->find(z);
			if (j != sortedMap->end()) {
				windows = j->second;
				k = windows->find(iWin);
				if (k != windows->end()) {
					windows->erase(k);
				}
			}
		}
		pthread_mutex_unlock(&wrMutex);
	}

	/*void SDLDeviceScreen::updateWindowState(
			GingaScreenID screenId, IWindow* win, short state) {

		map<GingaScreenID, vector<IWindow*>*>::iterator i;
		vector<IWindow*>* wins;

		pthread_mutex_lock(&wrMutex);
		i = windowRenderMap.find(screenId);
		if (i != windowRenderMap.end()) {
			wins = i->second;
			updateWindowList(wins, win, state);

		} else {
			wins = new vector<IWindow*>;
			wins->push_back(win);
			windowRenderMap[screenId] = wins;
		}


		pthread_mutex_unlock(&wrMutex);
	}

	void SDLDeviceScreen::updateWindowList(
			vector<IWindow*>* windows, IWindow* win, short state) {

		switch (state) {
			case SUW_SHOW:
				windows->push_back(win);
				break;

			case SUW_HIDE:
				removeFromWindowList(windows, win);
				break;

			case SUW_RAISETOTOP:
				removeFromWindowList(windows, win);
				windows->push_back(win);
				break;

			case SUW_LOWERTOBOTTOM:
				removeFromWindowList(windows, win);
				windows->insert(windows->begin(), win);
				break;

			default:
				break;
		}
	}*/

	void SDLDeviceScreen::removeFromWindowList(
			vector<IWindow*>* windows, IWindow* win) {

		vector<IWindow*>::iterator i;

		i = windows->begin();
		while (i != windows->end()) {
			if ((*i) == win) {
				windows->erase(i);
				i = windows->begin();
			} else {
				++i;
			}
		}
	}

	SDL_Window* SDLDeviceScreen::getUnderlyingWindow(GingaWindowID winId) {
		SDL_Window* window = NULL;
		Uint32 wid;

		wid    = (Uint32)(unsigned long)winId;
		window = SDL_GetWindowFromID(wid);

		if (window == NULL) {
			clog << "SDLDeviceScreen::getUnderlyingWindow ";
			clog << "can't find id '" << wid;
			clog << "'" << endl;
		}

		return window;
	}

	void SDLDeviceScreen::drawWindow(
			SDL_Renderer* renderer,
			SDL_Texture* texture,
			IWindow* iWin) {

		SDL_Rect rect;
		IColor* bgColor;
		Uint8 rr, rg, rb, ra;
		int i, r, g, b, a, bw;
		int alpha = 0;

		bool addMode = false;

		DrawData* dd;
		SDL_Rect dr;
		vector<DrawData*>* drawData;
		vector<DrawData*>::iterator it;

	    if (iWin != NULL) {
	    	/* getting renderer previous state */
	    	SDL_GetRenderDrawColor(renderer, &rr, &rg, &rb, &ra);

	    	rect.x = iWin->getX();
	    	rect.y = iWin->getY();
	    	rect.w = iWin->getW();
	    	rect.h = iWin->getH();

	    	alpha = iWin->getTransparencyValue();
	    	if (texture != NULL) {
	    		SDL_SetTextureAlphaMod(texture, 255 - alpha);
	    	}

	    	/* setting window background */
	    	bgColor = iWin->getBgColor();
	    	if (bgColor != NULL) {
	    		if (alpha == 0) {
	    			alpha = 255 - bgColor->getAlpha();
	    		}

	    		SDL_SetRenderDrawColor(
	    				renderer,
	    				bgColor->getR(),
	    				bgColor->getG(),
	    				bgColor->getB(),
	    				255 - alpha);

	    		SDL_RenderFillRect(renderer, &rect);
	    	}

	    	/* geometric figures (lua only) */
	    	drawData = ((SDLWindow*)iWin)->createDrawDataList();
	    	if (drawData != NULL) {
	    		it = drawData->begin();
	    		while (it != drawData->end()) {
	    			dd = (*it);
		    		SDL_SetRenderDrawColor(
		    				renderer, dd->r, dd->g, dd->b, dd->a);

	    			switch (dd->dataType) {
						case SDLWindow::DDT_LINE:
							if ((dd->coord1 < rect.x) ||
									(dd->coord2 < rect.y) ||
									(dd->coord1 > rect.w) ||
									(dd->coord2 > rect.h) ||
									(dd->coord3 > rect.w) ||
									(dd->coord4 > rect.h)) {

								cout << "SDLDeviceScreen::drawWindow ";
								cout << "invalid line coords: " << endl;
								cout << dd->coord1 << ", ";
								cout << dd->coord2 << ", ";
								cout << dd->coord3 << ", ";
								cout << dd->coord4 << "'";
								cout << endl;
								cout << "Window rect coords: " << endl;
								cout << rect.x << ", ";
								cout << rect.y << ", ";
								cout << rect.w << ", ";
								cout << rect.h << "'";
								cout << endl;
								break;
							}

							SDL_RenderDrawLine(
									renderer,
									dd->coord1 + rect.x,
									dd->coord2 + rect.y,
									dd->coord3 + rect.x,
									dd->coord4 + rect.y);

							break;

						case SDLWindow::DDT_RECT:
							dr.x = dd->coord1 + rect.x;
							dr.y = dd->coord2 + rect.y;
							dr.w = dd->coord3;
							dr.h = dd->coord4;

							if ((dr.x > + rect.x + rect.w) ||
									(dr.y >  + rect.y + rect.h) ||
									(dd->coord1 + dr.w > rect.w) ||
									(dd->coord2 + dr.h > rect.h)) {

								cout << "SDLDeviceScreen::drawWindow ";
								cout << "invalid rect coords: " << endl;
								cout << dr.x << ", ";
								cout << dr.y << ", ";
								cout << dr.w << ", ";
								cout << dr.h << "'";
								cout << endl;
								cout << "Window rect coords: " << endl;
								cout << rect.x << ", ";
								cout << rect.y << ", ";
								cout << rect.w << ", ";
								cout << rect.h << "'";
								cout << endl;
								break;
							}

							if (dd->dataType == SDLWindow::DDT_RECT) {
								SDL_RenderDrawRect(renderer, &dr);

							} else {
								SDL_RenderFillRect(renderer, &dr);
							}
							break;
	    			}
	    			++it;
	    		}
	    		delete drawData;
	    	}

	    	/* window rendering */
	    	if (texture != NULL) {
	    		SDL_RenderCopy(renderer, texture, NULL, &rect);
	    	}

	    	/* window border */
	    	iWin->getBorder(&r, &g, &b, &a, &bw);
	    	if (bw != 0) {
	    		SDL_SetRenderDrawColor(renderer, r, g, b, a);

				i = 0;
				while (i != bw) {
					rect.x = iWin->getX() - i;
					rect.y = iWin->getY() - i;
					rect.w = iWin->getW() + 2*i;
					rect.h = iWin->getH() + 2*i;

					SDL_RenderDrawRect(renderer, &rect);

					if (bw < 0) {
						i--;
					} else {
						i++;
					}
				}
	    	}

	    	/* setting renderer previous state */
	    	SDL_SetRenderDrawColor(renderer, rr, rg, rb, ra);
	    }
	}

	SDL_Texture* SDLDeviceScreen::createTextureFromSurface(
			SDL_Renderer* renderer, SDL_Surface* surface) {

		SDL_Texture* texture;

		texture = SDL_CreateTextureFromSurface(renderer, surface);
	    if (!texture) {
	        clog << "SDLDeviceScreen::createTextureFromSurface";
	        clog << "Couldn't create texture: " << SDL_GetError();
	        clog << endl;
	        return NULL;
	    }

	    /* allowing alpha */
	    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		return texture;
	}

	SDL_Texture* SDLDeviceScreen::createTexture(
			SDL_Renderer* renderer, int w, int h) {

		SDL_Texture* texture;

		texture = SDL_CreateTexture(
				renderer,
				SDL_PIXELFORMAT_RGB24,
				SDL_TEXTUREACCESS_STREAMING,
				w, h);

	    /* allowing alpha */
	    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	    return texture;
	}

	void SDLDeviceScreen::releaseTexture(SDL_Texture* texture) {
		SDL_DestroyTexture(texture);
	}

	SDL_Surface* SDLDeviceScreen::createUnderlyingSurface(
			int width, int height) {

		pthread_mutex_lock(&uSurMutex);
		SDL_Surface* newUSur = NULL;

		uSurW             = width;
		uSurH             = height;
		uSur              = NULL;
		uSurPendingAction = SPA_CREATE;
		waitSurfaceAction();

		newUSur = uSur;
		pthread_mutex_unlock(&uSurMutex);

		return newUSur;
	}

	SDL_Surface* SDLDeviceScreen::createUnderlyingSurfaceFromTexture(
			SDL_Texture* texture) {

		SDL_Surface* uSur = NULL;
		void* pixels;
		int tpitch[3];
        Uint32 rmask, gmask, bmask, amask, format;
        int textureAccess, w, h;

        SDL_QueryTexture(texture, &format, &textureAccess, &w, &h);
		SDL_LockTexture(texture, NULL, &pixels, &tpitch[0]);
		getRGBAMask(24, &rmask, &gmask, &bmask, &amask);

		uSur = SDL_CreateRGBSurfaceFrom(
				pixels, w, h, 24, tpitch[0], rmask, gmask, bmask, amask);

		SDL_UnlockTexture(texture);

		return uSur;
	}

	void SDLDeviceScreen::releaseUnderlyingSurface(SDL_Surface* uSur) {
		SDL_FreeSurface(uSur);
	}

	void SDLDeviceScreen::getRGBAMask(
			int depth,
			Uint32* rmask,
			Uint32* gmask,
			Uint32* bmask,
			Uint32* amask) {

		switch (depth) {
			case 32:
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				*rmask = 0xff000000;
				*gmask = 0x00ff0000;
				*bmask = 0x0000ff00;
				*amask = 0x000000ff;
#else
				*rmask = 0x000000ff;
				*gmask = 0x0000ff00;
				*bmask = 0x00ff0000;
				*amask = 0xff000000;
#endif
				break;

			case 24:
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				*rmask = 0x00ff0000;
				*gmask = 0x0000ff00;
				*bmask = 0x000000ff;
				*amask = 0x00000000;
#else
				*rmask = 0x000000ff;
				*gmask = 0x0000ff00;
				*bmask = 0x00ff0000;
				*amask = 0x00000000;
#endif
				break;
		}
	}

	void SDLDeviceScreen::waitSurfaceAction() {
		waitingCreator = true;
		pthread_mutex_lock(&cMutex);
		pthread_cond_wait(&cond, &cMutex);
		waitingCreator = false;
		pthread_mutex_unlock(&cMutex);
	}

	bool SDLDeviceScreen::surfaceActionExecuted() {
		if (waitingCreator) {
			pthread_cond_signal(&cond);
			return true;
		}
		return false;
	}
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::core::mb::IDeviceScreen*
		createSDLScreen(
				int numArgs, char** args,
				GingaScreenID myId, GingaWindowID parentId) {

	return (new ::br::pucrio::telemidia::ginga::core::mb::
			SDLDeviceScreen(numArgs, args, myId, parentId));
}

extern "C" void destroySDLScreen(
		::br::pucrio::telemidia::ginga::core::mb::IDeviceScreen* ds) {

	delete ds;
}
