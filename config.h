#include "xtilez.h"

#ifndef XTILEZ_CONFIG_H
#define XTILEZ_CONFIG_H

#define MODKEY Mod1Mask
#define BORDER_WIDTH_PIXELS 0 // TODO: border width adjustments for window sizes

static Keybinding KeyBindings[] = {{
    MODKEY | ShiftMask,
    XK_Return,
    exec_process,
    (KeybindingArgs){.cmd = {"xterm", NULL}},
}};

#endif // XTILEZ_CONFIG_H
