#include <stdlib.h>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#ifndef XTILEZ_H
#define XTILEZ_H

#define MAX_NAME_LEN 256
#define MAX_CLIENTS 1024
#define MAX_CMD_LEN 256
#define NUM_WORKSPACES 9

#define LENGTH(X) (sizeof X / sizeof X[0])

//
// Window manager data -----------------------------------------------
//

// Represents a connected monitor
typedef struct {
  // screen size
  int width;
  int height;
  int root;

  unsigned int selected_workspace;
  GC graphics_context;
} Monitor;

// Client for each window to be managed
typedef struct {
  char name[MAX_NAME_LEN];
  int x;
  int y;
  int width;
  int height;
  int border_width;

  // TODO: will use original fields for floating
  int orig_x;
  int orig_y;
  int orig_width;
  int orig_height;
  int orig_border_width;
  Window window;
} Client;

// Virtual workspaces to tile windows on
typedef struct {
  char name[MAX_NAME_LEN];
  unsigned int n_clients;
  unsigned int n_main; // Number of windows in main split
  float split_ratio;
  Client *clients[MAX_CLIENTS];
  unsigned int main_side; // 0 for left, 1 for right
} Workspace;

// Store the x11 metadata, workspaces and monitors
typedef struct {
  Display *display;
  Workspace workspaces[NUM_WORKSPACES];
  Monitor *monitors;
  int selected_monitor;
} TilezWM;

#define SELECTED_MONITOR(wm) (wm)->monitors[(wm)->selected_monitor]
#define SELECTED_WORKSPACE(wm)                                                 \
  (wm)->workspaces[(wm)->monitors[(wm)->selected_monitor].selected_workspace]

// Keybindings bind function calls to X11 key events

typedef union {
  char *cmd[MAX_CMD_LEN];
} KeybindingArgs;

typedef struct {
  unsigned int modifier;
  KeySym keysym;
  void (*func)(KeybindingArgs *);
  KeybindingArgs args;
} Keybinding;

//
// debug.c -----------------------------------------------------------
//

const char *event_type_to_str(int);

//
// log.c -------------------------------------------------------------
//

#define XT_LOG_LEVEL_DEBUG 0
#define XT_LOG_LEVEL_INFO 1
#define XT_LOG_LEVEL_WARN 2
#define XT_LOG_LEVEL_ERROR 3
#define XT_LOG_LEVEL_FATAL 4

void xt_log(unsigned int, const char *, ...);

#define DEBUG 1

#define XT_LOG_WARN(...) xt_log(XT_LOG_LEVEL_WARN, __VA_ARGS__)
#define XT_LOG_INFO(...) xt_log(XT_LOG_LEVEL_INFO, __VA_ARGS__)
#define XT_LOG_ERROR(...) xt_log(XT_LOG_LEVEL_ERROR, __VA_ARGS__)
#define XT_LOG_FATAL(...)                                                      \
  do {                                                                         \
    xt_log(XT_LOG_LEVEL_FATAL, __VA_ARGS__);                                   \
    exit(1);                                                                   \
  } while (0)

#if DEBUG
#define XT_LOG_DEBUG(...) xt_log(XT_LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define XT_LOG_DEBUG(...)                                                      \
  do {                                                                         \
  } while (0)
#endif

//
// client.c ----------------------------------------------------------
//

void window_manage(Display *, Workspace *, Monitor *, Window,
                   XWindowAttributes *);

void window_unmanage(Display *, Workspace *, Monitor *, Window);

//
// tiling_algorithms.c -----------------------------------------------
//

// Generic window tiling function signature
typedef void (*TilingFunc)(Workspace *, Monitor *);

// Applies the altorithm
void tile_window(Display *, Workspace *, Monitor *, TilingFunc);

// Tiling algorithms
void vertical_tile(Workspace *, Monitor *);

//
// handlers.c --------------------------------------------------------
//

// Generic event handling function signature
typedef void (*EventHandler)(TilezWM *, XEvent *);

// Event handlers
void handle_map_request(TilezWM *, XEvent *);
void handle_destroy_notify(TilezWM *, XEvent *);
void handle_key_press(TilezWM *, XEvent *);
void handle_configure_request(TilezWM *, XEvent *);

//
// exec.c ------------------------------------------------------------
//
void exec_process(KeybindingArgs *);

#endif // XTILEZ_H
