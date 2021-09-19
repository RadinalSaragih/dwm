/* appearance */
static const unsigned int snap = 10; /* snap pixel */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor,  >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const unsigned int systrayonleft  = 0; /* systray on the left of status text */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor */
static const int focusonwheel = 0;
static const int showsystray = 1; /* 0 = no systray */
static const int showbar = 1; /* 0 = no bar */
static const int topbar = 1; /* 0 = bottom bar */
static const char *fonts[] = { "monospace:size=11" };

/* colors */
static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#005577";
static char selbgcolor[]      = "#005577";

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* defining some programs */
#define SP_TERM "st", "-g", "115x45"
#define FM "/usr/bin/ranger"
#define RSS "/usr/bin/newsboat"
#define MPLAYER "/usr/bin/cmus"
#define BRWSR "/usr/bin/elinks", "-no-connect", "https://lite.duckduckgo.com/lite"

/* scratchpads */
typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = { SP_TERM, "-n", "sp-1", NULL };
const char *spcmd2[] = { SP_TERM, "-n", "sp-2", "-e", FM, NULL };
const char *spcmd3[] = { SP_TERM, "-n", "sp-3", "-e", RSS, NULL };
const char *spcmd4[] = { SP_TERM, "-n", "sp-4", "-e", MPLAYER, NULL };
const char *spcmd5[] = { SP_TERM, "-n", "sp-5", "-e", BRWSR, NULL };


static Sp scratchpads[] = {
/* name      cmd  */
{"sp-1",    spcmd1},
{"sp-2",    spcmd2},
{"sp-3",    spcmd3},
{"sp-4",    spcmd4},
{"sp-5",    spcmd5},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
/* xprop(1):
 * WM_CLASS(STRING) = instance, class
 * WM_NAME(STRING) = title */
/* class          instance      title    tags mask   iscentered isfloating monitor */
{ "Steam",        NULL,         NULL,     1 << 8,         0,         0,      -1 },
{ "VirtualBox Manager", NULL,    NULL,    1 << 7,         0,         0,      -1 },
{ "Inkscape",     "org.inkscape.Inkscape", NULL, 1 << 6, 0,          0,      -1 },
{ "Gimp", NULL,   "GNU Image Manipulation Program", 1 << 5,  0,      0,      -1 },
{ NULL,           "libreoffice",NULL,     1 << 5,         0,         0,      -1 },
{ "kdenlive",     NULL,         NULL,     1 << 5,         0,         0,      -1 },
{ NULL,           "Navigator",  NULL,     1 << 4,         0,         0,      -1 },
{ NULL,           "freetube",   NULL,     1 << 3,         0,         0,      -1 },
{ NULL,           "sp-1",       NULL,     SPTAG(0),       1,         1,      -1 },
{ NULL,           "sp-2",       NULL,     SPTAG(1),       1,         1,      -1 },
{ NULL,           "sp-3",       NULL,     SPTAG(2),       1,         1,      -1 },
{ NULL,           "sp-4",       NULL,     SPTAG(3),       1,         1,      -1 },
{ NULL,           "sp-5",       NULL,     SPTAG(4),       1,         1,      -1 },
};

/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints = 0; /* 1 = respect size hints in tiled resizals */
static const Layout layouts[] = {
  /* symbol     arrange function */
  { "TILED-R",      tile        }, /* first entry is default */
  { "TILED-L",      left_stack    },
  { "FLOAT",        NULL        },
  { "MONOCLE",      monocle     },
  /* { "BSTACK",       bstack      }, */
  /* { "BSTACK-HORIZ", bstackhoriz }, */
};

/* key definitions */
#define MODKEY Mod4Mask
#define HYPER Mod3Mask
#define TAGKEYS(KEY,TAG)                                                \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* mediakey's keycodes */
//#include <X11/XF86keysym.h>

/* define view_adjacent */
#include "view_adjacent.c"

/* extra commands */
#define TERM "st"
//#define EMACS "emacsclient", "-nc"
#define TABTERM "tabbed", "-f","-r", "2", TERM, "-w", "''"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-p", ">", NULL };
static const char *termCMD[] = { TERM, NULL };
static const char *tabTermCMD[] = { TABTERM, NULL };
static const char *editorCMD[] = { TERM, "nvim", NULL };
//static const char *brwsrcmd[] = { "qutebrowser", NULL };

/* keybindings */
static Key keys[] = {
/* modifier                     key        function        argument */
{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
{ MODKEY,                       XK_Return, spawn,          {.v = termCMD } },
{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = tabTermCMD } },
{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = editorCMD } },

{ MODKEY,                       XK_b,      togglebar,      {0} },

{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

{ MODKEY|ControlMask,           XK_bracketright, incnmaster, {.i = +1 } },
{ MODKEY|ControlMask,           XK_bracketleft, incnmaster, {.i = -1 } },

{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },

{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },

{ MODKEY,                       XK_backslash,    setlayout, {.v = &layouts[0]} },
{ MODKEY,                       XK_bracketright, setlayout, {.v = &layouts[1]} },
{ MODKEY|ShiftMask,             XK_bracketright, setlayout, {.v = &layouts[2]} },
{ MODKEY|ShiftMask,             XK_backslash,    setlayout, {.v = &layouts[3]} },

{ MODKEY|ControlMask,           XK_space,  setlayout,      {0} },

{ MODKEY,                       XK_s,      togglefloating, {0} },
{ MODKEY,                       XK_f,      togglefullscr,  {0} },
{ MODKEY,                       XK_x,      togglesticky,   {0} },

{ MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
{ MODKEY|ShiftMask,             XK_k,      pushup,         {0} },
{ MODKEY|ShiftMask,             XK_h,      view_adjacent,  {.i = -1 } },
{ MODKEY|ShiftMask,             XK_l,      view_adjacent,  {.i = +1 } },

{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },

{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },

{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },

{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },

{ HYPER,                        XK_f,      moveresize,     {.v = "0x 25y 0w 0h" } },
{ HYPER,                        XK_d,      moveresize,     {.v = "0x -25y 0w 0h" } },
{ HYPER,                        XK_g,      moveresize,     {.v = "25x 0y 0w 0h" } },
{ HYPER,                        XK_s,      moveresize,     {.v = "-25x 0y 0w 0h" } },

{ HYPER|ShiftMask,              XK_f,      moveresize,     {.v = "0x 0y 0w 25h" } },
{ HYPER|ShiftMask,              XK_d,      moveresize,     {.v = "0x 0y 0w -25h" } },
{ HYPER|ShiftMask,              XK_g,      moveresize,     {.v = "0x 0y 25w 0h" } },
{ HYPER|ShiftMask,              XK_s,      moveresize,     {.v = "0x 0y -25w 0h" } },

{ HYPER|ControlMask,            XK_f,      moveresizeedge, {.v = "b"} },
{ HYPER|ControlMask,            XK_d,      moveresizeedge, {.v = "t"} },
{ HYPER|ControlMask,            XK_g,      moveresizeedge, {.v = "r"} },
{ HYPER|ControlMask,            XK_s,      moveresizeedge, {.v = "l"} },

{ HYPER|ControlMask|ShiftMask,  XK_f,      moveresizeedge, {.v = "B"} },
{ HYPER|ControlMask|ShiftMask,  XK_d,      moveresizeedge, {.v = "T"} },
{ HYPER|ControlMask|ShiftMask,  XK_g,      moveresizeedge, {.v = "R"} },
{ HYPER|ControlMask|ShiftMask,  XK_s,      moveresizeedge, {.v = "L"} },

{ MODKEY,                       XK_n,      zoom,           {0} },
{ MODKEY,                       XK_Tab,    view,           {0} },

{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

{ MODKEY|ControlMask,           XK_j,      focusmon,       {.i = -1 } },
{ MODKEY|ControlMask,           XK_k,      focusmon,       {.i = +1 } },

{ MODKEY|ControlMask|ShiftMask, XK_j,      tagmon,         {.i = -1 } },
{ MODKEY|ControlMask|ShiftMask, XK_k,      tagmon,         {.i = +1 } },

TAGKEYS(                        XK_1,                      0)
TAGKEYS(                        XK_2,                      1)
TAGKEYS(                        XK_3,                      2)
TAGKEYS(                        XK_4,                      3)
TAGKEYS(                        XK_5,                      4)
TAGKEYS(                        XK_6,                      5)
TAGKEYS(                        XK_7,                      6)
TAGKEYS(                        XK_8,                      7)
TAGKEYS(                        XK_9,                      8)

{ MODKEY,                       XK_q,      togglescratch,  {.ui = 0 } },
{ MODKEY,                       XK_w,      togglescratch,  {.ui = 1 } },
{ MODKEY|ShiftMask,             XK_grave,  togglescratch,  {.ui = 2 } },
{ MODKEY,                       XK_grave,  togglescratch,  {.ui = 3 } },
{ MODKEY,                       XK_e,      togglescratch,  {.ui = 4 } },

{ MODKEY|ShiftMask,             XK_o,      spawn,          {.v = brwsrcmd } },
{ MODKEY,                       XK_F12,    spawn,          SHCMD("$HOME/.local/bin/networkmanager_dmenu") },

/* { MODKEY,                       XK_F5,     spawn,          SHCMD("brightnessctl -c backlight s 50-") }, */
/* { MODKEY,                       XK_F6,     spawn,          SHCMD("brightnessctl -c backlight s 50+") }, */

{ HYPER,                        XK_F12,    spawn,          SHCMD("$HOME/.local/share/dwm/scripts/dm-pdf") },
{ MODKEY|ShiftMask,             XK_F12,    spawn,          SHCMD("$HOME/.local/share/dwm/scripts/dm-gutenberg") },
{ MODKEY|Mod1Mask,              XK_Escape, spawn,          SHCMD("$HOME/.local/share/dwm/scripts/dm-SysMenu") },
{ 0,                            XK_Print,  spawn,          SHCMD("$HOME/.local/share/dwm/scripts/dm-screenshot") },

{ MODKEY,                       XK_F3,     spawn,          SHCMD("cmus-remote -R; pkill -RTMIN+5 dwmblocks") },
{ MODKEY,                       XK_F4,     spawn,          SHCMD("cmus-remote -S; pkill -RTMIN+5 dwmblocks") },
{ HYPER,                        XK_F7,     spawn,          SHCMD("cmus-remote -u; pkill -RTMIN+5 dwmblocks") },
{ HYPER,                        XK_F8,     spawn,          SHCMD("cmus-remote -s; pkill -RTMIN+5 dwmblocks") },
{ HYPER,                        XK_F6,     spawn,          SHCMD("cmus-remote -n; pkill -RTMIN+5 dwmblocks") },
{ HYPER,                        XK_F5,     spawn,          SHCMD("cmus-remote -r; pkill -RTMIN+5 dwmblocks") },

{ MODKEY,                       XK_F1,     spawn,          SHCMD("cmus-remote -v -1%; pkill -RTMIN+10 dwmblocks") },
{ MODKEY,                       XK_F2,     spawn,          SHCMD("cmus-remote -v +1%; pkill -RTMIN+10 dwmblocks") },

{ HYPER,                        XK_F3,     spawn,          SHCMD("pactl set-sink-mute $(pactl info | grep 'Default Sink' | awk '{print $3}') toggle ; pkill -RTMIN+10 dwmblocks") },
{ HYPER,                        XK_F1,     spawn,          SHCMD("pactl set-sink-volume $(pactl info | grep 'Default Sink' | awk '{print $3}') -1% ; pkill -RTMIN+10 dwmblocks") },
{ HYPER,                        XK_F2,     spawn,          SHCMD("pactl set-sink-volume $(pactl info | grep 'Default Sink' | awk '{print $3}') +1% ; pkill -RTMIN+10 dwmblocks") },

{ MODKEY|Mod1Mask|ControlMask,  XK_Escape, quit,           {0} }, // quit WM
{ MODKEY|Mod1Mask,              XK_r,      quit,           {1} }, // reload WM
{ MODKEY|ControlMask,           XK_x,      xrdb,           {.v = NULL } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click          event mask        button          function        argument */
  { ClkLtSymbol,    0,                Button1,        setlayout,      {0} },
  { ClkLtSymbol,    0,                Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,    0,                Button2,        zoom,           {0} },
  { ClkStatusText,  0,                Button2,        spawn,          {.v = termCMD } },
  { ClkClientWin,   MODKEY,           Button1,        movemouse,      {0} },
  { ClkClientWin,   MODKEY,           Button2,        togglefloating, {0} },
  { ClkClientWin,   MODKEY|ShiftMask, Button1,        resizemouse,    {0} },
  { ClkTagBar,      0,                Button1,        view,           {0} },
  { ClkTagBar,      0,                Button3,        toggleview,     {0} },
  { ClkTagBar,      MODKEY,           Button1,        tag,            {0} },
  { ClkTagBar,      MODKEY,           Button3,        toggletag,      {0} },
};
