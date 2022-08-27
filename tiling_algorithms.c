#include "xtilez.h"

void tile_window(Display *display, Workspace *workspace, Monitor *monitor,
                 TilingFunc tiling_func) {
  tiling_func(workspace, monitor);

  for (unsigned int i = 0; i < workspace->n_clients; i++) {
    XMoveResizeWindow(display, workspace->clients[i]->window,
                      workspace->clients[i]->x, workspace->clients[i]->y,
                      workspace->clients[i]->width,
                      workspace->clients[i]->height);
  }
}

void vertical_tile(Workspace *workspace, Monitor *monitor) {
  if (workspace->n_clients == 1) {
    workspace->clients[0]->x = 0;
    workspace->clients[0]->y = 0;
    workspace->clients[0]->width = monitor->width;
    workspace->clients[0]->height = monitor->height;
    // TODO: border width?
    return;
  }
  float main_win_width = monitor->width * (1 - workspace->split_ratio);
  float stack_win_width = monitor->width * workspace->split_ratio;
  float stack_win_height =
      (float)monitor->height / (workspace->n_clients - workspace->n_main);
  float main_win_height = (float)monitor->height / workspace->n_main;

  int stack_count = 0;
  for (unsigned int i = 0; i < workspace->n_clients; i++) {
    if (i < workspace->n_main) {
      workspace->clients[i]->x = workspace->main_side ? main_win_width : 0;
      workspace->clients[i]->y = 0; // TODO n_main > 1
      workspace->clients[i]->width = main_win_width;
      workspace->clients[i]->height = main_win_height;
    } else {
      workspace->clients[i]->x = workspace->main_side ? 0 : main_win_width;
      workspace->clients[i]->y = stack_count * stack_win_height;
      workspace->clients[i]->width = stack_win_width;
      workspace->clients[i]->height = stack_win_height;
      stack_count++;
    }
  }
}
