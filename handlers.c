#include "config.h"
#include "xtilez.h"

void handle_map_request(TilezWM *wm, XEvent *event) {
  XWindowAttributes window_attrs;
  XMapRequestEvent *map_request = &event->xmaprequest;

  if (!XGetWindowAttributes(wm->display, map_request->window, &window_attrs)) {
    return;
  }
  if (window_attrs.override_redirect) {
    return;
  }

  // TODO: only do this if we haven't already managed the window
  window_manage(wm->display, &SELECTED_WORKSPACE(wm), &SELECTED_MONITOR(wm),
                map_request->window, &window_attrs);
  // TODO: update window type
  // TODO: update size hints
}

void handle_destroy_notify(TilezWM *wm, XEvent *event) {
  XDestroyWindowEvent *destroy_event = &event->xdestroywindow;
  window_unmanage(wm->display, &SELECTED_WORKSPACE(wm), &SELECTED_MONITOR(wm),
                  destroy_event->window);
}

void handle_key_press(TilezWM *wm, XEvent *event) {
  XKeyEvent *xk_event = &event->xkey;
  KeySym keysym = XkbKeycodeToKeysym(wm->display, xk_event->keycode, 0,
                                     xk_event->state & ShiftMask ? 1 : 0);
  for (unsigned int i = 0; i < LENGTH(KeyBindings); i++) {
    if (keysym == KeyBindings[i].keysym && KeyBindings[i].func) {
      KeyBindings[i].func(&KeyBindings[i].args);
    }
  }
}

void handle_configure_request(TilezWM *wm, XEvent *event) {
  XConfigureRequestEvent *conf_request = &event->xconfigurerequest;

  XWindowChanges window_changes;
  window_changes.x = conf_request->x;
  window_changes.y = conf_request->y;
  window_changes.width = conf_request->width;
  window_changes.height = conf_request->height;
  window_changes.border_width = conf_request->border_width;
  window_changes.sibling = conf_request->above;
  window_changes.stack_mode = conf_request->detail;
  XConfigureWindow(wm->display, conf_request->window, conf_request->value_mask,
                   &window_changes);
  XSync(wm->display, False);
}
