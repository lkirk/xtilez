#include <X11/Xlib.h>

#include "xtilez.h"

const char *event_type_to_str(int event_type) {
  switch (event_type) {
  case KeyPress:
    return "KeyPress";
  case KeyRelease:
    return "KeyRelease";
  case ButtonPress:
    return "ButtonPress";
  case ButtonRelease:
    return "ButtonRelease";
  case MotionNotify:
    return "MotionNotify";
  case EnterNotify:
    return "EnterNotify";
  case LeaveNotify:
    return "LeaveNotify";
  case FocusIn:
    return "FocusIn";
  case FocusOut:
    return "FocusOut";
  case KeymapNotify:
    return "KeymapNotify";
  case Expose:
    return "Expose";
  case GraphicsExpose:
    return "GraphicsExpose";
  case NoExpose:
    return "NoExpose";
  case VisibilityNotify:
    return "VisibilityNotify";
  case CreateNotify:
    return "CreateNotify";
  case DestroyNotify:
    return "DestroyNotify";
  case UnmapNotify:
    return "UnmapNotify";
  case MapNotify:
    return "MapNotify";
  case MapRequest:
    return "MapRequest";
  case ReparentNotify:
    return "ReparentNotify";
  case ConfigureNotify:
    return "ConfigureNotify";
  case ConfigureRequest:
    return "ConfigureRequest";
  case GravityNotify:
    return "GravityNotify";
  case ResizeRequest:
    return "ResizeRequest";
  case CirculateNotify:
    return "CirculateNotify";
  case CirculateRequest:
    return "CirculateRequest";
  case PropertyNotify:
    return "PropertyNotify";
  case SelectionClear:
    return "SelectionClear";
  case SelectionRequest:
    return "SelectionRequest";
  case SelectionNotify:
    return "SelectionNotify";
  case ColormapNotify:
    return "ColormapNotify";
  case ClientMessage:
    return "ClientMessage";
  case MappingNotify:
    return "MappingNotify";
  case GenericEvent:
    return "GenericEvent";
  case LASTEvent:
    return "LASTEvent";
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
    // This line exits 1
    XT_LOG_FATAL("unknown event type: %d", event_type);
  }
}
#pragma clang diagnostic pop
