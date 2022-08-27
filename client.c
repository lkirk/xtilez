#include <stdio.h>

#include "config.h"
#include "xtilez.h"

void window_manage(Display *display, Workspace *workspace, Monitor *monitor,
                   Window window, XWindowAttributes *window_attrs) {
  if (workspace->n_clients == (MAX_CLIENTS - 1)) {
    XT_LOG_FATAL("reached max clients on workspace");
  }

  // Copy data from map request into our client
  Client *client = malloc(sizeof(Client));
  client->window = window;
  client->x = client->orig_x = window_attrs->x;
  client->y = client->orig_y = window_attrs->y;
  client->width = client->orig_width = window_attrs->width;
  client->height = client->orig_height = window_attrs->height;
  client->border_width = client->orig_border_width = window_attrs->border_width;
  client->border_width = BORDER_WIDTH_PIXELS;

  // Store the client on the workspace
  // NB: must bump n clients and add new client before tiling
  workspace->clients[workspace->n_clients] = client;
  workspace->n_clients++;

  // Run window tiling algo, which modifies the client obj
  tile_window(display, workspace, monitor, vertical_tile);

  XMapWindow(display, client->window);
}

void window_unmanage(Display *display, Workspace *workspace, Monitor *monitor,
                     Window window) {
  unsigned int deleted = 0;
  unsigned int n_clients = workspace->n_clients; // need to create a copy
  for (unsigned int i = 0; i < n_clients; i++) {
    Client *client;
    if ((client = workspace->clients[i])->window == window) {
      free(client);

      // Shift the clients down the client stack
      // We subtract 1 from n_clients, because if we're deleting the last item,
      // then we don't want to do anything.
      for (unsigned int j = i; j < n_clients - 1; j++) {
        workspace->clients[j] = workspace->clients[j + 1];
      }
      workspace->n_clients--;
      workspace->clients[workspace->n_clients] = NULL;
      deleted = 1;
      break;
    }
  }
  if (!deleted) {
    XT_LOG_WARN("did not delete %lu\n", window);
  }

  tile_window(display, workspace, monitor, vertical_tile);
}
