# dwm - dynamic window manager
Patched build of Suckless's Dynamic Window Manager. "dwm" is an extremely fast, small, and dynamic window manager for X. 

## Installation
* A basic "make install" would do the job.
```sh
sudo make install
```
## Patches
* actualfullscreen
* attachaside
* autostart
* center
* decorhints
* deck
* uselessgaps
* exitmenu
* focusonclick
* focusmaster
* leftstack
* moveresize
* noborderfloatingfix 
* pertag 
* preserveonrestart 
* push_no_master
* restartsig 
* savefloats
* scratchpads
* sticky
* swallow
* systray
* warp
* xresources
* view_adjacent

## Build Dependencies
* Debian Based Distros
```sh
sudo apt install build-essential libgdk-pixbuf-xlib-2.0-0 libxft-dev \
	libxinerama-dev libxcb1-dev libx11-xcb-dev libxcb-res0-dev
```
Note: libxft >= 2.3.5 (to use colored fonts)
