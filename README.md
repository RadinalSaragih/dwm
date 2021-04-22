# dwm - dynamic window manager
This is my bloated build of the Suckless's Dynamic Window Manager
dwm is an extremely fast, small, and dynamic window manager for X.


I have configured this build to add a .desktop file at installation so it less annoying when using a display manager like lightdm, etc. 
I also made it to add a script folder at the /usr/local/bin directory, scripts that are needed for some keybinding.

patches applied:
+ actualfullscreen -> Actually toggles fullscreen
+ attachaside -> Make new clients get attached and focused in the stacking area instead of always becoming the new master.
+ bottomstack -> Adds the bstack (bottom stack) and bstackhoriz (horizontal bottom stack) layout.
+ center -> Adds iscentered rule to automatically center clients.
+ moveresize -> allows moving and resizing dwm's clients using keyboard bindings.
+ noborderfloatingfix -> Remove the border when there is only one window visible.
+ pertag -> Changes such as layouts, are only applied to the selected tags.
+ push no master -> Push up or down  clients, except the master client.
+ restartsig -> Restart dwm with a keybinding.
+ savefloats -> Saves size and position of every floating window before forced into tiled mode.
+ scratchpads -> Enables multiple scratchpads, each with one asigned window.
+ sticky -> Makes a client "sticky", and is visible to all tags
+ systray -> Adds a systray.

- For an Arch or Arch-based distro a simple make install would be sufficient. But for debian-based distro, there would be a few library that need to be installed. 

> sudo apt install libx-dev libxft-dev libxinerama-dev
