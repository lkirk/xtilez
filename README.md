# xtilez
XTilez is a tiling window manager inspired by xmonad and dwm. There are some things that I didn't like about xmonad and dwm, so I decided to write my own WM. It was easier to inspect and understand the code for those two, rather than try to build a wayland WM.

# Building
```
make
```

# Debugging
You must have xephyr installed
```
make debug
```

# Testing the interface
To test the user interface, you can run x11 xephyr and play with the WM inside of your current x session
```
make manual-test
```
