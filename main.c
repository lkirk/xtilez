#include <locale.h>
#include <stdio.h>

#include "config.h"
#include "xtilez.h"

int xerror_start(Display *display, XErrorEvent *err_event) {
  (void)display;   // unused param
  (void)err_event; // unused param

  XT_LOG_FATAL("Another WM is already running");
  return -1;
}

int xerror(Display *display, XErrorEvent *err_event) {
  char error_text[BUFSIZ];
  char number[32];
  char request_text[BUFSIZ];

  XGetErrorText(display, err_event->error_code, error_text, BUFSIZ);

  if (err_event->request_code < 128) {
    snprintf(number, sizeof(number), "%d", err_event->request_code);
    XGetErrorDatabaseText(display, "XRequest", number, "", request_text,
                          BUFSIZ);
  }

  XT_LOG_ERROR("request=%d (%s) error=%d (%s)", err_event->request_code,
               request_text, err_event->error_code, error_text);

  return 0; // TODO: handle errors
}

static void (*EventHandlers[LASTEvent])(TilezWM *, XEvent *) = {
    [KeyPress] = handle_key_press,
    [DestroyNotify] = handle_destroy_notify,
    [ConfigureRequest] = handle_configure_request,
    [MapRequest] = handle_map_request,
};

void grab_keys(Display *display, Window root) {
  unsigned int modifiers[] = {0, Mod2Mask, LockMask};

  KeyCode code;
  for (unsigned int key_i = 0; key_i < LENGTH(KeyBindings); key_i++) {
    if ((code = XKeysymToKeycode(display, KeyBindings[key_i].keysym))) {
      for (unsigned int mod_i = 0; mod_i < LENGTH(modifiers); mod_i++) {
        unsigned int mods = KeyBindings[key_i].modifier | modifiers[mod_i];
        XGrabKey(display, code, mods, root, True, GrabModeAsync, GrabModeAsync);
      }
    }
  }
}

int main() {
  XT_LOG_INFO("Running");

  if (!setlocale(LC_CTYPE, "") || !XSupportsLocale()) {
    XT_LOG_FATAL("no locale support");
  }

  Display *display;
  if (!(display = XOpenDisplay(NULL))) {
    XT_LOG_FATAL("cannot open display");
  }

  // Check if another WM is running
  XSetErrorHandler(xerror_start);
  XSelectInput(display, DefaultRootWindow(display), SubstructureRedirectMask);
  XSync(display, False);
  XSetErrorHandler(xerror);
  XSync(display, False);

  // unsigned int default_screen_idx = 0;
  int n_screens = ScreenCount(display);
  XT_LOG_INFO("found %d screens", n_screens);
  Monitor *monitors = calloc(n_screens, sizeof(Monitor));

  for (int i = 0; i < n_screens; i++) {
    Screen *screen = ScreenOfDisplay(display, i);
    XT_LOG_INFO("found a screen with root=%lu dimensions=%dx%d", screen->root,
                screen->width, screen->height);

    monitors[i] = (Monitor){
        .width = screen->width,
        .height = screen->height,
        .root = RootWindow(display, i),
        .selected_workspace = i,
        .graphics_context = XCreateGC(display, RootWindow(display, i), 0, NULL),
    };

    XSetLineAttributes(display, monitors[i].graphics_context, 1, LineSolid,
                       CapButt, JoinMiter);

    XSelectInput(display, monitors[i].root,
                 SubstructureRedirectMask | SubstructureNotifyMask |
                     ButtonPressMask | PointerMotionMask | EnterWindowMask |
                     LeaveWindowMask | StructureNotifyMask |
                     PropertyChangeMask);

    grab_keys(display, monitors[i].root);
  }

  TilezWM *wm = malloc(sizeof(TilezWM));
  wm->display = display;
  for (int i = 0; i < NUM_WORKSPACES; i++) {
    Workspace *workspace = malloc(sizeof(Workspace));
    snprintf(workspace->name, MAX_NAME_LEN, "%d", i);
    workspace->n_clients = 0;
    workspace->n_main = 1;
    workspace->split_ratio = .5;
    workspace->main_side = 0;
    wm->workspaces[i] = *workspace;
  }
  wm->monitors = monitors;
  wm->selected_monitor = 0;

  // Main loop
  XEvent event;
  while (!XNextEvent(display, &event)) {
    XT_LOG_DEBUG("Event type: %i (%s)", event.type,
                 event_type_to_str(event.type));
    EventHandler handler;
    if ((handler = EventHandlers[event.type])) { // if we have a handler for
                                                 // this type
      // https://tronche.com/gui/x/xlib/events/processing-overview.html
      EventHandlers[event.type](wm, &event);
    }
  }

  XT_LOG_INFO("Exiting");
}
