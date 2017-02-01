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
#include "Display.h"

#include "InputManager.h"
#include "AudioProvider.h"
#include "SDLEventBuffer.h"
#include "FontProvider.h"
#include "SDLInputEvent.h"
#include "SDLSurface.h"
#include "VideoProvider.h"
#include "SDLWindow.h"

GINGA_MB_BEGIN

// Global display; initialized by main().
Display *_Ginga_Display = NULL;

bool Display::mutexInit = false;
map<CodeMap::KeyCode, int> Display::gingaToSDLCodeMap;
map<int, CodeMap::KeyCode> Display::sdlToGingaCodeMap;
map<string, int> Display::sdlStrToSdlCode;
set<SDL_Texture *> Display::uTexPool;
set<SDL_Surface *> Display::uSurPool;

pthread_mutex_t Display::sdlMutex;
pthread_mutex_t Display::sieMutex;
pthread_mutex_t Display::renMutex;
pthread_mutex_t Display::scrMutex;
pthread_mutex_t Display::recMutex;
pthread_mutex_t Display::winMutex;
pthread_mutex_t Display::surMutex;
pthread_mutex_t Display::proMutex;
pthread_mutex_t Display::cstMutex;


// BEGIN SANITY ------------------------------------------------------------

// Display renderer job data.
typedef struct _Job
{
  guint64 id;
  DisplayJob func;
  void *data;
} Job;

// Compares the ids of two job data.
static gint
job_cmp_id (gconstpointer p1, gconstpointer p2)
{
  Job *j1 = deconst (Job *, p1);
  Job *j2 = deconst (Job *, p2);
  int id1 = j1->id;
  int id2 = j2->id;
  return (id1 < id2) ? -1 : (id1 > id2) ? 1 : 0;
}

// Deletes job data.
static void
job_delete (gpointer p)
{
  Job *job = (Job *) p;
  delete job;
}

// Compares the z-index of two windows.
static gint
win_cmp_z (gconstpointer p1, gconstpointer p2)
{
  SDLWindow *w1 = deconst (SDLWindow *, p1);
  SDLWindow *w2 = deconst (SDLWindow *, p2);
  double z1 = w1->getZ ();
  double z2 = w2->getZ ();
  return (z1 < z2) ? -1 : (z1 > z2) ? 1 : 0;
}

// Deletes window.
static void
win_delete (gpointer p)
{
  SDLWindow *win = (SDLWindow *) p;
  delete win;
}

// Deletes provider.
static void
prov_delete (gpointer p)
{
  IContinuousMediaProvider *prov = (IContinuousMediaProvider *) p;
  delete prov;
}


// Private methods.

void
Display::lock (void)
{
  g_rec_mutex_lock (&this->mutex);
}

void
Display::unlock (void)
{
  g_rec_mutex_unlock (&this->mutex);
}

gpointer
Display::add (GList **list, gpointer data)
{
  this->lock ();
  if (unlikely (g_list_find (*list, data)))
    {
      g_warning ("object %p already in list %p", data, *list);
      goto done;
    }
  *list = g_list_append (*list, data);
 done:
  this->unlock ();
  return data;
}

gpointer
Display::remove (GList **list, gpointer data)
{
  GList *elt;

  this->lock ();
  elt = g_list_find (*list, data);
  if (unlikely (elt == NULL))
    {
      g_warning ("object %p not in list %p", data, *list);
      goto done;
    }
  *list = g_list_remove_link (*list, elt);
 done:
  this->unlock ();
  return data;
}

gboolean
Display::find (GList *list, gconstpointer data)
{
  GList *elt;

  this->lock ();
  elt = g_list_find (list, data);
  this->unlock ();

  return elt != NULL;
}

gpointer
Display::renderThreadWrapper (gpointer data)
{
  g_assert_nonnull (data);
  ((Display *) data)->renderThread ();
  return NULL;
}

// FIXME:
//
// - We should expose the main window background color, e.g., via
//   command-line argument --background.
// - Window transparency attribute should be called alpha; 0.0 means
//   transparent and 1.0 opaque.
// - The alpha component of colors is inverted.
// - Alpha blending is not working.
// - Handle border width.
//
void
Display::renderThread ()
{
  guint flags;

  g_assert (!SDL_WasInit (0));
  if (unlikely (SDL_Init (0) != 0))
    g_critical ("cannot initialize SDL: %s", SDL_GetError ());

  SDL_SetHint (SDL_HINT_NO_SIGNAL_HANDLERS, "1");
  SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1");

  this->lock ();
  flags = SDL_WINDOW_SHOWN;
  if (this->fullscreen)
    flags |= SDL_WINDOW_FULLSCREEN;

  g_assert_null (this->screen);
  g_assert_null (this->renderer);
  SDLx_CreateWindowAndRenderer (this->width, this->height, flags,
                                &this->screen, &this->renderer);
  g_assert (!this->render_thread_ready);
  this->unlock ();

  g_mutex_lock (&this->render_thread_mutex);
  this->render_thread_ready = true;
  g_cond_signal (&this->render_thread_cond);
  g_mutex_unlock (&this->render_thread_mutex);

  while (!this->hasQuitted())   // render loop
    {
      SDL_Event evt;
      GList *l;

      while (SDL_PollEvent (&evt)) // handle input
        {
          switch (evt.type)
            {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
              if (evt.key.keysym.sym != SDLK_ESCAPE)
                break;
              // fall-through
            case SDL_QUIT:
              this->quit ();
              goto quit;
            default:
              break;
            }

          InputManager *im = this->getInputManager ();
          SDLEventBuffer *buf = im->getEventBuffer ();
          buf->feed (evt, false, false);
        }

      this->lock ();            //  update providers
      for (l = this->providers; l != NULL; l = l->next)
        {
          SDL_Texture *texture;
          IContinuousMediaProvider *prov;
          int width, height;

          prov = (IContinuousMediaProvider *) l->data;
          if (!prov->getHasVisual ())
            continue;               // nothing to do

          if (prov->getProviderContent () == NULL)
            {
              prov->getOriginalResolution (&width, &height);
              texture = createTexture (this->renderer, width, height);
              g_assert_nonnull (texture);
              prov->setProviderContent (texture);
            }
          prov->refreshDR (NULL);
        }
      this->unlock ();

      this->lock ();            // run jobs
      for (l = this->jobs; l != NULL; l = l->next)
      {
        Job *job;

        job = (Job *) l->data;
        g_assert_nonnull (job);
        job->func (this->renderer, job->data);
      }
      this->unlock ();

      this->lock ();            // redraw windows
      SDL_SetRenderDrawColor (this->renderer, 255, 0, 255, 255);
      SDL_RenderClear (this->renderer);
      this->windows = g_list_sort (this->windows, win_cmp_z);
      for (l = this->windows; l != NULL; l = l->next)
        {
          SDLWindow *win;

          win = (SDLWindow *) l->data;
          g_assert_nonnull (win);

          if (win->isVisible () && !win->isGhostWindow ())
            win->redraw ();
        }
      SDL_RenderPresent (this->renderer);
      this->unlock ();
    }

 quit:
  SDL_Quit ();
  exit (EXIT_SUCCESS);
}


// Public methods.

/**
 * Creates a display with the given dimensions.
 * If FULLSCREEN is true, enable full-screen mode.
 */
Display::Display (int width, int height, bool fullscreen)
{
  g_rec_mutex_init (&this->mutex);

  this->width = width;
  this->height = height;
  this->fullscreen = fullscreen;

  g_rec_mutex_init (&this->renderer_mutex);
  this->renderer = NULL;
  this->screen = NULL;
  this->im = NULL;

  this->_quit = false;
  this->render_thread = NULL;
  this->render_thread_ready = false;
  g_mutex_init (&this->render_thread_mutex);
  g_cond_init (&this->render_thread_cond);

  this->jobs = NULL;
  this->windows = NULL;
  this->providers = NULL;

  this->im = new InputManager ();
  g_assert_nonnull (this->im);
  this->im->setAxisBoundaries (this->width, this->height, 0);

  checkMutexInit ();            // FIXME
  initCodeMaps ();              // FIXME

  this->render_thread = g_thread_new ("render", renderThreadWrapper, this);
  g_assert_nonnull (this->render_thread);
  g_mutex_lock (&this->render_thread_mutex);
  while (!this->render_thread_ready)
    g_cond_wait (&this->render_thread_cond, &this->render_thread_mutex);
  g_mutex_unlock (&this->render_thread_mutex);
  g_assert (this->render_thread_ready);
}

/**
 * Destroys display.
 */
Display::~Display ()
{
  this->quit ();

  this->lock ();
  g_assert_null (g_thread_join (this->render_thread));
  g_mutex_clear (&this->render_thread_mutex);
  g_cond_clear (&this->render_thread_cond);
  SDL_DestroyRenderer (this->renderer);
  SDL_DestroyWindow (this->screen);
  delete im;
  g_list_free_full (this->jobs, job_delete);
  g_list_free_full (this->windows, win_delete);
  g_list_free_full (this->providers, prov_delete);
  this->unlock ();
  g_rec_mutex_clear (&this->mutex);
}

/**
 * Gets display size.
 */
void
Display::getSize (int *width, int *height)
{
  this->lock ();
  set_if_nonnull (width, this->width);
  set_if_nonnull (height, this->height);
  this->unlock ();
}

/**
 * Sets display size.
 */
void
Display::setSize (int width, int height)
{
  this->lock ();
  SDL_SetWindowSize (this->screen, width, height); // don't return a status
  this->width = width;
  this->height = height;
  this->unlock ();
}

/**
 * Gets display full-screen mode.
 */
bool
Display::getFullscreen ()
{
  bool fullscreen;

  this->lock ();
  fullscreen = this->fullscreen;
  this->unlock ();
  return fullscreen;
}

/**
 * Sets display full-screen mode.
 */
void
Display::setFullscreen (bool fullscreen)
{
  int status;
  int flags;

  this->lock ();
  flags = (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0;
  status = SDL_SetWindowFullscreen (this->screen, flags);
  if (unlikely (status != 0))
    {
      g_warning ("cannot change display full-screen mode to %s: %s",
                 (fullscreen) ? "true" : "false", SDL_GetError ());
      goto done;
    }
  this->fullscreen = fullscreen;
 done:
  this->unlock ();
}

/**
 * Quits display render thread.
 */
void
Display::quit ()
{
  this->lock ();
  this->_quit = true;
  this->unlock ();
}

/**
 * Returns true if display render thread has quitted.
 */
bool
Display::hasQuitted ()
{
  bool quit;

  this->lock ();
  quit = this->_quit;
  this->unlock ();

  return quit;
}

/**
 * Pushes a new job to renderer job list.
 * Returns the job id.
 */
DisplayJobId
Display::addJob (DisplayJob func, void *data)
{
  Job *job;
  static DisplayJobId last = 0;

  this->lock ();
  job = new Job;
  g_assert_nonnull (job);
  job->id = last++;
  job->func = func;
  job->data = data;
  this->add (&this->jobs, job);
  this->unlock ();

  return job->id;
}

/**
 * Removes from the renderer job list the job with the given id.
 * Returns true if job was removed.
 */
bool
Display::removeJob (DisplayJobId id)
{
  Job j;
  GList *elt;
  bool result;

  this->lock ();
  j.id = id;
  elt = g_list_find_custom (this->jobs, &j, job_cmp_id);
  if (elt == NULL)
    {
      result = false;
    }
  else
    {
      result = true;
      delete (Job *) elt->data;
    }

  this->unlock ();
  return result;
}

/**
 * Locks display renderer.
 */
void
Display::lockRenderer ()
{
  g_rec_mutex_lock (&this->renderer_mutex);
}

/**
 * Unlocks display renderer.
 */
void
Display::unlockRenderer ()
{
  g_rec_mutex_unlock (&this->renderer_mutex);
}

/**
 * Gets display renderer and locks it.
 */
SDL_Renderer *
Display::getLockedRenderer ()
{
  SDL_Renderer *renderer = this->getRenderer ();
  this->lockRenderer ();
  return renderer;
}

/**
 * Gets display renderer.
 */
SDL_Renderer *
Display::getRenderer ()
{
  SDL_Renderer *renderer;

  this->lock ();
  renderer = this->renderer;
  this->unlock ();
  g_assert_nonnull (renderer);
  return renderer;
}

/**
 * Creates managed window with the given position, dimensions, and z-index.
 */
SDLWindow *
Display::createWindow (int x, int y, int w, int h, int z)
{
  SDLWindow *win;

  win = new SDLWindow (x, y, z, w, h);
  g_assert_nonnull (win);
  this->add (&this->windows, win);

  return win;
}

/**
 * Tests whether window WIN managed by display.
 */
bool
Display::hasWindow (const SDLWindow *win)
{
  g_assert_nonnull (win);
  return this->find (this->windows, win);
}

/**
 * Destroys managed window.
 */
void
Display::destroyWindow (SDLWindow *win)
{
  g_assert_nonnull (win);
  this->remove (&this->windows, win);
  delete win;
}

/**
 * Creates managed continuous media provider to decode URI.
 */
IContinuousMediaProvider *
Display::createContinuousMediaProvider (const string &uri)
{
  IContinuousMediaProvider *prov;

  prov = new VideoProvider (uri);
  g_assert_nonnull (prov);
  this->add (&this->providers, prov);
  return prov;
}

/**
 * Destroys managed continuous media provider.
 */
void
Display::destroyContinuousMediaProvider (IContinuousMediaProvider *prov)
{
  g_assert_nonnull (prov);
  this->remove (&this->providers, prov);
  delete prov;
}


// END SANITY --------------------------------------------------------------

void
Display::checkMutexInit ()
{
  if (!mutexInit)
    {
      mutexInit = true;

      Thread::mutexInit (&sdlMutex, true);
      Thread::mutexInit (&sieMutex, true);
      Thread::mutexInit (&renMutex, true);
      Thread::mutexInit (&scrMutex, true);
      Thread::mutexInit (&recMutex, true);
      Thread::mutexInit (&winMutex, true);
      Thread::mutexInit (&surMutex, true);
      Thread::mutexInit (&proMutex, true);
      Thread::mutexInit (&cstMutex, true);
    }
}

void
Display::lockSDL ()
{
  checkMutexInit ();

  Thread::mutexLock (&sdlMutex);
}

void
Display::unlockSDL ()
{
  checkMutexInit ();

  Thread::mutexUnlock (&sdlMutex);
}

/* interfacing output */

SDLSurface *
Display::createSurface ()
{
  return createSurfaceFrom (NULL);
}

SDLSurface *
Display::createSurface (int w, int h)
{
  SDLSurface *iSur = NULL;
  SDL_Surface *uSur = NULL;

  lockSDL ();

  uSur = createUnderlyingSurface (w, h);

  iSur = new SDLSurface (uSur);

  unlockSDL ();

  Thread::mutexLock (&surMutex);
  surfacePool.insert (iSur);
  Thread::mutexUnlock (&surMutex);

  return iSur;
}

SDLSurface *
Display::createSurfaceFrom (void *uSur)
{
  SDLSurface *iSur = NULL;

  lockSDL ();
  if (uSur != NULL)
    {
      iSur = new SDLSurface (uSur);
    }
  else
    {
      iSur = new SDLSurface ();
    }
  unlockSDL ();

  Thread::mutexLock (&surMutex);
  surfacePool.insert (iSur);
  Thread::mutexUnlock (&surMutex);

  return iSur;
}

bool
Display::hasSurface (SDLSurface *s)
{
  set<SDLSurface *>::iterator i;
  bool hasSur = false;

  Thread::mutexLock (&surMutex);
  i = surfacePool.find (s);
  if (i != surfacePool.end ())
    {
      hasSur = true;
    }
  Thread::mutexUnlock (&surMutex);

  return hasSur;
}

bool
Display::releaseSurface (SDLSurface *s)
{
  set<SDLSurface *>::iterator i;
  bool released = false;

  Thread::mutexLock (&surMutex);
  i = surfacePool.find (s);
  if (i != surfacePool.end ())
    {
      surfacePool.erase (i);
      released = true;
    }
  Thread::mutexUnlock (&surMutex);

  return released;
}

/* interfacing content */

IFontProvider *
Display::createFontProvider (const char *mrl, int fontSize)
{
  IFontProvider *provider = NULL;

  lockSDL ();
  provider = new FontProvider (mrl, fontSize);
  unlockSDL ();

  Thread::mutexLock (&proMutex);
  dmpPool.insert (provider);
  Thread::mutexUnlock (&proMutex);

  return provider;
}

void
Display::releaseFontProvider (IFontProvider *provider)
{
  set<IDiscreteMediaProvider *>::iterator i;
  //IDiscreteMediaProvider *dmp;

  Thread::mutexLock (&proMutex);
  i = dmpPool.find (provider);
  if (i != dmpPool.end ())
    {
      dmpPool.erase (i);

      Thread::mutexUnlock (&proMutex);
    }
  else
    {
      Thread::mutexUnlock (&proMutex);
    }
}

SDLSurface *
Display::createRenderedSurfaceFromImageFile (const char *mrl)
{
  SDL_Surface *sfc;
  SDLSurface *surface;
  SDLWindow *window;

  surface = Ginga_Display->createSurfaceFrom (NULL);
  g_assert_nonnull (surface);

  sfc = IMG_Load (mrl);
  if (unlikely (sfc == NULL))
    g_error ("cannot load image file %s: %s", mrl, IMG_GetError ());

  g_assert_nonnull (surface);
  surface->setContent (sfc);
  Display::addUnderlyingSurface (sfc);

  window = surface->getParentWindow ();
  g_assert_null (window);

  return surface;
}

/* interfacing input */

InputManager *
Display::getInputManager ()
{
  return im;
}

SDLEventBuffer *
Display::createEventBuffer ()
{
  return new SDLEventBuffer ();
}

SDLInputEvent *
Display::createInputEvent (void *event, const int symbol)
{
  SDLInputEvent *ie = NULL;

  if (event != NULL)
    {
      ie = new SDLInputEvent (*(SDL_Event *)event);
    }

  if (symbol >= 0)
    {
      ie = new SDLInputEvent (symbol);
    }

  return ie;
}

SDLInputEvent *
Display::createApplicationEvent (int type, void *data)
{
  return new SDLInputEvent (type, data);
}

CodeMap::KeyCode
Display::fromMBToGinga (int keyCode)
{
  map<int, CodeMap::KeyCode>::iterator it;
  CodeMap::KeyCode translated;

  checkMutexInit ();

  Thread::mutexLock (&sieMutex);

  translated = CodeMap::KEY_NULL;
  it = sdlToGingaCodeMap.find (keyCode);
  if (it != sdlToGingaCodeMap.end ())
    {
      translated = it->second;
    }
  else
    {
      clog << "Display::fromMBToGinga can't find code '";
      clog << keyCode << "' returning KEY_NULL" << endl;
    }

  Thread::mutexUnlock (&sieMutex);

  return translated;
}

int
Display::fromGingaToMB (CodeMap::KeyCode keyCode)
{
  map<CodeMap::KeyCode, int>::iterator i;
  int translated;

  checkMutexInit ();

  Thread::mutexLock (&sieMutex);

  translated = CodeMap::KEY_NULL;
  i = gingaToSDLCodeMap.find (keyCode);
  if (i != gingaToSDLCodeMap.end ())
    {
      translated = i->second;
    }
  else
    {
      clog << "Display::fromGingaToMB can't find code '";
      clog << keyCode << "' returning KEY_NULL" << endl;
    }

  Thread::mutexUnlock (&sieMutex);

  return translated;
}

/* libgingaccmbsdl internal use*/

/* input */
int
Display::convertEventCodeStrToInt (const string &strEvent)
{
  int intEvent = -1;
  map<string, int>::iterator i;

  i = sdlStrToSdlCode.find (strEvent);
  if (i != sdlStrToSdlCode.end ())
    {
      intEvent = i->second;
    }

  return intEvent;
}

void
Display::initCodeMaps ()
{
  checkMutexInit ();

  Thread::mutexLock (&sieMutex);
  if (!gingaToSDLCodeMap.empty ())
    {
      Thread::mutexUnlock (&sieMutex);
      return;
    }

  // sdlStrToSdlCode
  sdlStrToSdlCode = {
    {"GIEK:QUIT", SDL_QUIT},
    {"GIEK:UNKNOWN", SDLK_UNKNOWN},
    {"GIEK:0", SDLK_0},
    {"GIEK:1", SDLK_1},
    {"GIEK:2", SDLK_2},
    {"GIEK:3", SDLK_3},
    {"GIEK:4", SDLK_4},
    {"GIEK:5", SDLK_5},
    {"GIEK:6", SDLK_6},
    {"GIEK:7", SDLK_7},
    {"GIEK:8", SDLK_8},
    {"GIEK:9", SDLK_9},

    {"GIEK:a", SDLK_a},
    {"GIEK:b", SDLK_b},
    {"GIEK:c", SDLK_c},
    {"GIEK:d", SDLK_d},
    {"GIEK:e", SDLK_e},
    {"GIEK:f", SDLK_f},
    {"GIEK:g", SDLK_g},
    {"GIEK:h", SDLK_h},
    {"GIEK:i", SDLK_i},
    {"GIEK:j", SDLK_j},
    {"GIEK:k", SDLK_k},
    {"GIEK:l", SDLK_l},
    {"GIEK:m", SDLK_m},
    {"GIEK:n", SDLK_n},
    {"GIEK:o", SDLK_o},
    {"GIEK:p", SDLK_p},
    {"GIEK:q", SDLK_q},
    {"GIEK:r", SDLK_r},
    {"GIEK:s", SDLK_s},
    {"GIEK:t", SDLK_t},
    {"GIEK:u", SDLK_u},
    {"GIEK:v", SDLK_v},
    {"GIEK:w", SDLK_w},
    {"GIEK:x", SDLK_x},
    {"GIEK:y", SDLK_y},
    {"GIEK:z", SDLK_z},

    {"GIEK:A", SDLK_a + 5000},
    {"GIEK:B", SDLK_b + 5000},
    {"GIEK:C", SDLK_c + 5000},
    {"GIEK:D", SDLK_d + 5000},
    {"GIEK:E", SDLK_e + 5000},
    {"GIEK:F", SDLK_f + 5000},
    {"GIEK:G", SDLK_g + 5000},
    {"GIEK:H", SDLK_h + 5000},
    {"GIEK:I", SDLK_i + 5000},
    {"GIEK:J", SDLK_j + 5000},
    {"GIEK:K", SDLK_k + 5000},
    {"GIEK:L", SDLK_l + 5000},
    {"GIEK:M", SDLK_m + 5000},
    {"GIEK:N", SDLK_n + 5000},
    {"GIEK:O", SDLK_o + 5000},
    {"GIEK:P", SDLK_p + 5000},
    {"GIEK:Q", SDLK_q + 5000},
    {"GIEK:R", SDLK_r + 5000},
    {"GIEK:S", SDLK_s + 5000},
    {"GIEK:T", SDLK_t + 5000},
    {"GIEK:U", SDLK_u + 5000},
    {"GIEK:V", SDLK_v + 5000},
    {"GIEK:W", SDLK_w + 5000},
    {"GIEK:X", SDLK_x + 5000},
    {"GIEK:Y", SDLK_y + 5000},
    {"GIEK:Z", SDLK_z + 5000},

    {"GIEK:PAGEDOWN", SDLK_PAGEDOWN},
    {"GIEK:PAGEUP", SDLK_PAGEUP},

    {"GIEK:F1", SDLK_F1},
    {"GIEK:F2", SDLK_F2},
    {"GIEK:F3", SDLK_F3},
    {"GIEK:F4", SDLK_F4},
    {"GIEK:F5", SDLK_F5},
    {"GIEK:F6", SDLK_F6},
    {"GIEK:F7", SDLK_F7},
    {"GIEK:F8", SDLK_F8},
    {"GIEK:F9", SDLK_F9},
    {"GIEK:F10", SDLK_F10},
    {"GIEK:F11", SDLK_F11},
    {"GIEK:F12", SDLK_F12},

    {"GIEK:PLUS", SDLK_PLUS},
    {"GIEK:MINUS", SDLK_MINUS},

    {"GIEK:ASTERISK", SDLK_ASTERISK},
    {"GIEK:HASH", SDLK_HASH},

    {"GIEK:PERIOD", SDLK_PERIOD},

    {"GIEK:CAPSLOCK", SDLK_CAPSLOCK},
    {"GIEK:PRINTSCREEN", SDLK_PRINTSCREEN},
    {"GIEK:MENU", SDLK_MENU},
    {"GIEK:F14", SDLK_F14},
    {"GIEK:QUESTION", SDLK_QUESTION},

    {"GIEK:DOWN", SDLK_DOWN},
    {"GIEK:LEFT", SDLK_LEFT},
    {"GIEK:RIGHT", SDLK_RIGHT},
    {"GIEK:UP", SDLK_UP},

    {"GIEK:F15", SDLK_F15},
    {"GIEK:F16", SDLK_F16},

    {"GIEK:VOLUMEDOWN", SDLK_VOLUMEDOWN},
    {"GIEK:VOLUMEUP", SDLK_VOLUMEUP},

    {"GIEK:RETURN", SDLK_RETURN},
    {"GIEK:RETURN2", SDLK_RETURN2},

    {"GIEK:F17", SDLK_F17},
    {"GIEK:F18", SDLK_F18},
    {"GIEK:F19", SDLK_F19},
    {"GIEK:F20", SDLK_F20},

    {"GIEK:SPACE", SDLK_SPACE},
    {"GIEK:BACKSPACE", SDLK_BACKSPACE},
    {"GIEK:AC_BACK", SDLK_AC_BACK},
    {"GIEK:ESCAPE", SDLK_ESCAPE},
    {"GIEK:OUT", SDLK_OUT},

    {"GIEK:POWER", SDLK_POWER},
    {"GIEK:F21", SDLK_F21},
    {"GIEK:STOP", SDLK_STOP},
    {"GIEK:EJECT", SDLK_EJECT},
    {"GIEK:EXECUTE", SDLK_EXECUTE},
    {"GIEK:F22", SDLK_F22},
    {"GIEK:PAUSE", SDLK_PAUSE},

    {"GIEK:GREATER", SDLK_GREATER},
    {"GIEK:LESS", SDLK_LESS},

    {"GIEK:TAB", SDLK_TAB},
    {"GIEK:F23", SDLK_F23}
  };

  // gingaToSDLCodeMap
  gingaToSDLCodeMap = {
    {CodeMap::KEY_QUIT, SDL_QUIT},
    {CodeMap::KEY_NULL, SDLK_UNKNOWN},
    {CodeMap::KEY_0, SDLK_0},
    {CodeMap::KEY_1, SDLK_1},
    {CodeMap::KEY_2, SDLK_2},
    {CodeMap::KEY_3, SDLK_3},
    {CodeMap::KEY_4, SDLK_4},
    {CodeMap::KEY_5, SDLK_5},
    {CodeMap::KEY_6, SDLK_6},
    {CodeMap::KEY_7, SDLK_7},
    {CodeMap::KEY_8, SDLK_8},
    {CodeMap::KEY_9, SDLK_9},

    {CodeMap::KEY_SMALL_A, SDLK_a},
    {CodeMap::KEY_SMALL_B, SDLK_b},
    {CodeMap::KEY_SMALL_C, SDLK_c},
    {CodeMap::KEY_SMALL_D, SDLK_d},
    {CodeMap::KEY_SMALL_E, SDLK_e},
    {CodeMap::KEY_SMALL_F, SDLK_f},
    {CodeMap::KEY_SMALL_G, SDLK_g},
    {CodeMap::KEY_SMALL_H, SDLK_h},
    {CodeMap::KEY_SMALL_I, SDLK_i},
    {CodeMap::KEY_SMALL_J, SDLK_j},
    {CodeMap::KEY_SMALL_K, SDLK_k},
    {CodeMap::KEY_SMALL_L, SDLK_l},
    {CodeMap::KEY_SMALL_M, SDLK_m},
    {CodeMap::KEY_SMALL_N, SDLK_n},
    {CodeMap::KEY_SMALL_O, SDLK_o},
    {CodeMap::KEY_SMALL_P, SDLK_p},
    {CodeMap::KEY_SMALL_Q, SDLK_q},
    {CodeMap::KEY_SMALL_R, SDLK_r},
    {CodeMap::KEY_SMALL_S, SDLK_s},
    {CodeMap::KEY_SMALL_T, SDLK_t},
    {CodeMap::KEY_SMALL_U, SDLK_u},
    {CodeMap::KEY_SMALL_V, SDLK_v},
    {CodeMap::KEY_SMALL_W, SDLK_w},
    {CodeMap::KEY_SMALL_X, SDLK_x},
    {CodeMap::KEY_SMALL_Y, SDLK_y},
    {CodeMap::KEY_SMALL_Z, SDLK_z},

    {CodeMap::KEY_CAPITAL_A, SDLK_a + 5000},
    {CodeMap::KEY_CAPITAL_B, SDLK_b + 5000},
    {CodeMap::KEY_CAPITAL_C, SDLK_c + 5000},
    {CodeMap::KEY_CAPITAL_D, SDLK_d + 5000},
    {CodeMap::KEY_CAPITAL_E, SDLK_e + 5000},
    {CodeMap::KEY_CAPITAL_F, SDLK_f + 5000},
    {CodeMap::KEY_CAPITAL_G, SDLK_g + 5000},
    {CodeMap::KEY_CAPITAL_H, SDLK_h + 5000},
    {CodeMap::KEY_CAPITAL_I, SDLK_i + 5000},
    {CodeMap::KEY_CAPITAL_J, SDLK_j + 5000},
    {CodeMap::KEY_CAPITAL_K, SDLK_k + 5000},
    {CodeMap::KEY_CAPITAL_L, SDLK_l + 5000},
    {CodeMap::KEY_CAPITAL_M, SDLK_m + 5000},
    {CodeMap::KEY_CAPITAL_N, SDLK_n + 5000},
    {CodeMap::KEY_CAPITAL_O, SDLK_o + 5000},
    {CodeMap::KEY_CAPITAL_P, SDLK_p + 5000},
    {CodeMap::KEY_CAPITAL_Q, SDLK_q + 5000},
    {CodeMap::KEY_CAPITAL_R, SDLK_r + 5000},
    {CodeMap::KEY_CAPITAL_S, SDLK_s + 5000},
    {CodeMap::KEY_CAPITAL_T, SDLK_t + 5000},
    {CodeMap::KEY_CAPITAL_U, SDLK_u + 5000},
    {CodeMap::KEY_CAPITAL_V, SDLK_v + 5000},
    {CodeMap::KEY_CAPITAL_W, SDLK_w + 5000},
    {CodeMap::KEY_CAPITAL_X, SDLK_x + 5000},
    {CodeMap::KEY_CAPITAL_Y, SDLK_y + 5000},
    {CodeMap::KEY_CAPITAL_Z, SDLK_z + 5000},

    {CodeMap::KEY_PAGE_DOWN, SDLK_PAGEDOWN},
    {CodeMap::KEY_PAGE_UP, SDLK_PAGEUP},

    {CodeMap::KEY_F1, SDLK_F1},
    {CodeMap::KEY_F2, SDLK_F2},
    {CodeMap::KEY_F3, SDLK_F3},
    {CodeMap::KEY_F4, SDLK_F4},
    {CodeMap::KEY_F5, SDLK_F5},
    {CodeMap::KEY_F6, SDLK_F6},
    {CodeMap::KEY_F7, SDLK_F7},
    {CodeMap::KEY_F8, SDLK_F8},
    {CodeMap::KEY_F9, SDLK_F9},
    {CodeMap::KEY_F10, SDLK_F10},
    {CodeMap::KEY_F11, SDLK_F11},
    {CodeMap::KEY_F12, SDLK_F12},

    {CodeMap::KEY_PLUS_SIGN, SDLK_PLUS},
    {CodeMap::KEY_MINUS_SIGN, SDLK_MINUS},

    {CodeMap::KEY_ASTERISK, SDLK_ASTERISK},
    {CodeMap::KEY_NUMBER_SIGN, SDLK_HASH},

    {CodeMap::KEY_PERIOD, SDLK_PERIOD},

    {CodeMap::KEY_SUPER, SDLK_CAPSLOCK},
    {CodeMap::KEY_PRINTSCREEN, SDLK_PRINTSCREEN},
    {CodeMap::KEY_MENU, SDLK_MENU},
    {CodeMap::KEY_INFO, SDLK_F14},
    {CodeMap::KEY_EPG, SDLK_QUESTION},

    {CodeMap::KEY_CURSOR_DOWN, SDLK_DOWN},
    {CodeMap::KEY_CURSOR_LEFT, SDLK_LEFT},
    {CodeMap::KEY_CURSOR_RIGHT, SDLK_RIGHT},
    {CodeMap::KEY_CURSOR_UP, SDLK_UP},

    {CodeMap::KEY_CHANNEL_DOWN, SDLK_F15},
    {CodeMap::KEY_CHANNEL_UP, SDLK_F16},

    {CodeMap::KEY_VOLUME_DOWN, SDLK_VOLUMEDOWN},
    {CodeMap::KEY_VOLUME_UP, SDLK_VOLUMEUP},

    {CodeMap::KEY_ENTER, SDLK_RETURN},

    {CodeMap::KEY_RED, SDLK_F17},
    {CodeMap::KEY_GREEN, SDLK_F18},
    {CodeMap::KEY_YELLOW, SDLK_F19},
    {CodeMap::KEY_BLUE, SDLK_F20},

    {CodeMap::KEY_SPACE, SDLK_SPACE},
    {CodeMap::KEY_BACKSPACE, SDLK_BACKSPACE},
    {CodeMap::KEY_BACK, SDLK_AC_BACK},
    {CodeMap::KEY_ESCAPE, SDLK_ESCAPE},
    {CodeMap::KEY_EXIT, SDLK_OUT},

    {CodeMap::KEY_POWER, SDLK_POWER},
    {CodeMap::KEY_REWIND, SDLK_F21},
    {CodeMap::KEY_STOP, SDLK_STOP},
    {CodeMap::KEY_EJECT, SDLK_EJECT},
    {CodeMap::KEY_PLAY, SDLK_EXECUTE},
    {CodeMap::KEY_RECORD, SDLK_F22},
    {CodeMap::KEY_PAUSE, SDLK_PAUSE},

    {CodeMap::KEY_GREATER_THAN_SIGN, SDLK_GREATER},
    {CodeMap::KEY_LESS_THAN_SIGN, SDLK_LESS},

    {CodeMap::KEY_TAB, SDLK_TAB},
    {CodeMap::KEY_TAP, SDLK_F23}
  };

  // sdlToGingaCodeMap
  map<CodeMap::KeyCode, int>::iterator it;
  it = gingaToSDLCodeMap.begin ();
  while (it != gingaToSDLCodeMap.end ())
    {
      sdlToGingaCodeMap[it->second] = it->first;
      ++it;
    }

  Thread::mutexUnlock (&sieMutex);
}

SDL_Texture *
Display::createTextureFromSurface (SDL_Renderer *renderer,
                                           SDL_Surface *surface)
{
  SDL_Texture *texture = NULL;

  checkMutexInit ();

  lockSDL ();
  Thread::mutexLock (&surMutex);

  if (Display::hasUnderlyingSurface (surface))
    {
      g_assert_nonnull (surface);
      texture = SDL_CreateTextureFromSurface (renderer, surface);
      if (unlikely (texture == NULL))
        {
          g_error ("cannot create texture for surface %p: %s",
                   surface, SDL_GetError ());
        }
      g_assert_nonnull (texture);
      uTexPool.insert (texture);

      /* allowing alpha */
      g_assert (SDL_SetTextureBlendMode (texture, SDL_BLENDMODE_BLEND) == 0);
    }

  Thread::mutexUnlock (&surMutex);
  unlockSDL ();

  return texture;
}

SDL_Texture *
Display::createTexture (SDL_Renderer *renderer, int w, int h)
{
  SDL_Texture *texture;

  lockSDL ();

  texture = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_ARGB32,
                               SDL_TEXTUREACCESS_STREAMING, w, h);

  // w > maxW || h > maxH || format is not supported
  assert (texture != NULL);

  /* allowing alpha */
  g_assert (SDL_SetTextureBlendMode (texture, SDL_BLENDMODE_BLEND) == 0);

  uTexPool.insert (texture);

  unlockSDL ();

  return texture;
}

bool
Display::hasTexture (SDL_Texture *uTex)
{
  set<SDL_Texture *>::iterator i;
  bool hasIt = false;

  checkMutexInit ();

  lockSDL ();
  i = uTexPool.find (uTex);
  if (i != uTexPool.end ())
    {
      hasIt = true;
    }
  unlockSDL ();

  return hasIt;
}

void
Display::releaseTexture (SDL_Texture *texture)
{
  set<SDL_Texture *>::iterator i;

  checkMutexInit ();

  lockSDL ();
  i = uTexPool.find (texture);
  if (i != uTexPool.end ())
    {
      uTexPool.erase (i);
      SDL_DestroyTexture (texture);
    }
  unlockSDL ();
}

void
Display::addUnderlyingSurface (SDL_Surface *uSur)
{
  checkMutexInit ();

  Thread::mutexLock (&surMutex);
  uSurPool.insert (uSur);
  Thread::mutexUnlock (&surMutex);
}

SDL_Surface *
Display::createUnderlyingSurface (int width, int height)
{
  SDL_Surface *newUSur = NULL;
  Uint32 rmask, gmask, bmask, amask;
  int bpp;

  checkMutexInit ();

  lockSDL ();

  SDL_PixelFormatEnumToMasks (SDL_PIXELFORMAT_ARGB32, &bpp, &rmask, &gmask, &bmask,
                              &amask);

  newUSur = SDL_CreateRGBSurface (0, width, height, bpp, rmask, gmask,
                                  bmask, amask);

  SDL_SetColorKey (newUSur, 1, *((Uint8 *)newUSur->pixels));
  unlockSDL ();

  Thread::mutexLock (&surMutex);
  if (newUSur != NULL)
    {
      uSurPool.insert (newUSur);
    }
  else
    {
      clog << "Display::createUnderlyingSurface SDL error: '";
      clog << SDL_GetError () << "'" << endl;
    }
  Thread::mutexUnlock (&surMutex);

  return newUSur;
}

SDL_Surface *
Display::createUnderlyingSurfaceFromTexture (SDL_Texture *texture)
{
  SDL_Surface *uSur = NULL;
  void *pixels;
  int tpitch[3];
  Uint32 rmask, gmask, bmask, amask, format;
  int textureAccess, w, h, bpp;

  lockSDL ();

  SDL_QueryTexture (texture, &format, &textureAccess, &w, &h);
  if (textureAccess & SDL_TEXTUREACCESS_STREAMING)
    {
      bool locked = true;

      // trying to lock texture
      if (SDL_LockTexture (texture, NULL, &pixels, &tpitch[0]) != 0)
        {
          locked = false;
        }

      SDL_PixelFormatEnumToMasks (SDL_PIXELFORMAT_ARGB32, &bpp, &rmask, &gmask,
                                  &bmask, &amask);

      uSur = SDL_CreateRGBSurfaceFrom (pixels, w, h, bpp, tpitch[0], rmask,
                                       gmask, bmask, amask);

      if (locked)
        {
          SDL_UnlockTexture (texture);
        }
    }

  unlockSDL ();

  Thread::mutexLock (&surMutex);
  if (uSur != NULL)
    {
      uSurPool.insert (uSur);
    }
  Thread::mutexUnlock (&surMutex);

  return uSur;
}

bool
Display::hasUnderlyingSurface (SDL_Surface *uSur)
{
  set<SDL_Surface *>::iterator i;
  bool hasIt = false;

  checkMutexInit ();

  Thread::mutexLock (&surMutex);
  i = uSurPool.find (uSur);
  if (i != uSurPool.end ())
    {
      hasIt = true;
    }
  Thread::mutexUnlock (&surMutex);

  return hasIt;
}

void
Display::releaseUnderlyingSurface (SDL_Surface *uSur)
{
  set<SDL_Surface *>::iterator i;

  checkMutexInit ();

  lockSDL ();
  Thread::mutexLock (&surMutex);

  i = uSurPool.find (uSur);
  if (i != uSurPool.end ())
    {
      uSurPool.erase (i);

      SDL_FreeSurface (uSur);
    }

  Thread::mutexUnlock (&surMutex);
  unlockSDL ();
}

GINGA_MB_END
