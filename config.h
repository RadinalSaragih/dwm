/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 10; /* snap pixel */
static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor,  >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor */
static const int showsystray = 1; /* 0 = no systray */
static const int showbar = 1; /* 0 = no bar */
static const int topbar = 1; /* 0 = bottom bar */
static const char *fonts[] = { "Hack Regular:size=11:antialias=true:autohint=true" };
static const char dmenufont[] = "Hack Regular:size=11:antialias=true:autohint=true";

/* colors */
static const char col_grey[] = "#262626";
static const char col_black[] = "#181818";
static const char col_white[] = "#ebdbb2";
static const char col_green[] = "#4b6e50";
static const char *colors[][3] = {
  /*                   fg            bg      border */
  [SchemeNorm]     = { col_white, col_grey, col_black },
  [SchemeSel]      = { col_white, col_green, col_white },
};

/* defining some programs */
#define TERM "st", "-g", "115x40"
#define FM "/usr/bin/ranger"
#define SHELL "/usr/bin/zsh"
#define CAL "/usr/bin/calcurse"
#define MPLAYER "/usr/bin/cmus"

/* scratchpads */
typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = { TERM, "-n", "sp-1", SHELL, NULL };
const char *spcmd2[] = { TERM, "-n", "sp-2", FM, NULL };
const char *spcmd3[] = { TERM, "-n", "sp-3", CAL, NULL };
const char *spcmd4[] = { TERM, "-n", "sp-4", MPLAYER, NULL };
const char *spcmd5[] = { "thunderbird", NULL };

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
   * WM_NAME(STRING) = title
   */
  /* class           instance      title    tags mask   iscentered isfloating monitor */
  { "VirtualBox Manager", NULL,    NULL,    1 << 3,         0,         0,      -1 },
  { NULL,           "freetube",    NULL,    1 << 3,         0,         0,      -1 },
  { "firefox",      "Navigator",   NULL,    1 << 4,         0,         0,      -1 },
  { "Chromium",      NULL,         NULL,    1 << 4,         0,         0,      -1 },
  { NULL,            "libreoffice",NULL,    1 << 5,         0,         0,      -1 },
  { "kdenlive",      NULL,         NULL,    1 << 5,         0,         0,      -1 },
  { "Gimp", NULL, "GNU Image Manipulation Program", 1 << 5, 0,         0,      -1 },
  { "Zoom",          NULL,         NULL,    1 << 6,         1,         1,      -1 },
  { NULL,            NULL,         "0 A.D.",1 << 6,         0,         0,      -1 },
  { "Steam",         NULL,         NULL,    1 << 6,         0,         0,      -1 },
  { NULL,   NULL,     "Picture in picture", 0,              1,         1,      -1 },
  { "password",      NULL,         NULL,    0,              1,         1,      -1 },
  { "Pavucontrol",   NULL,         NULL,    0,              1,         1,      -1 },
  { "Pcmanfm",       NULL,         NULL,    0,              1,         1,      -1 },
  {  NULL,           "sp-1",       NULL,    SPTAG(0),       1,         1,      -1 },
  {  NULL,           "sp-2",       NULL,    SPTAG(1),       1,         1,      -1 },
  {  NULL,           "sp-3",       NULL,    SPTAG(2),       1,         1,      -1 },
  {  NULL,           "sp-4",       NULL,    SPTAG(3),       1,         1,      -1 },
  { "Thunderbird",   "Mail",       NULL,    SPTAG(4),       1,         1,      -1 },
};

/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints = 1; /* 1 = respect size hints in tiled resizals */
static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "TTT",      bstack },
  { "===",      bstackhoriz }, 
  { "|M|",      monocle },
};
/* To use floating mode add the following lines to the layouts and keybindings section. */
//{ "><>",      NULL },
//{ MODKEY|ShiftMask,             XK_s,      setlayout,      {.v = &layouts[4]} },

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* define media keys */
#include <X11/XF86keysym.h>

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont,
                                  "-nb", col_grey, "-nf", col_white, "-sb", col_green,
                                  "-sf", col_white, "-p", "Choose a Program: ", NULL };

static const char *termcmd[] = { "st", SHELL, NULL };

/* keybindings */
static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
  { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },

  { MODKEY,                       XK_b,      togglebar,      {0} },

  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

  { MODKEY|ControlMask,           XK_bracketright, incnmaster, {.i = +1 } },
  { MODKEY|ControlMask,           XK_bracketleft, incnmaster, {.i = -1 } },

  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },

  { MODKEY|ShiftMask,             XK_q,      killclient,     {0} },

  { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_y,      setlayout,      {.v = &layouts[1]} },
  { MODKEY,                       XK_u,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[3]} },
  
  { MODKEY|ControlMask,           XK_space,  setlayout,      {0} },
  
  { MODKEY,                       XK_s,      togglefloating, {0} },
  { MODKEY,                       XK_f,      togglefullscr,  {0} },
  { MODKEY,                       XK_x,      togglesticky,   {0} },

  { MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
  { MODKEY|ShiftMask,             XK_k,      pushup,         {0} },

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

  { MODKEY,                       XK_n,      zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },

  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ControlMask,           XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ControlMask,           XK_period, tagmon,         {.i = +1 } },

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
  { MODKEY,                       XK_grave,  togglescratch,  {.ui = 2 } },

  { MODKEY|Mod1Mask|ControlMask,  XK_Escape, quit,           {0} }, // quit WM
  { MODKEY|Mod1Mask,              XK_r,      quit,           {1} }, // reload WM

  { MODKEY|Mod1Mask,              XK_Escape, spawn,          SHCMD("/usr/local/share/scripts/powermenu") },
  { 0,                            XK_Print,  spawn,          SHCMD("/usr/local/share/scripts/screenshotMenu") },
  { MODKEY,                       XK_F3,     spawn,          SHCMD("/usr/local/share/scripts/cmus-raiseAudio")},
  { MODKEY,                       XK_F2,     spawn,          SHCMD("/usr/local/share/scripts/cmus-lowerAudio")},
  { MODKEY,                       XK_F5,     spawn,          SHCMD("/usr/local/share/scripts/lowerBrightness") },
  { MODKEY,                       XK_F6,     spawn,          SHCMD("/usr/local/share/scripts/raiseBrightness") },
  { MODKEY,                       XK_F12,    spawn,          SHCMD("/usr/local/share/scripts/dm-pdf") },

  /* media keys */
  { 0, XF86XK_Mail,               togglescratch, {.ui = 4 } },
  { 0, XF86XK_Tools,              togglescratch, {.ui = 3 } },
  { 0, XF86XK_Explorer,           spawn, SHCMD("pcmanfm") },
  { 0, XF86XK_HomePage,           spawn, SHCMD("/usr/local/share/scripts/browser-menu") },
  { 0, XF86XK_Favorites,          spawn, SHCMD("/usr/local/share/scripts/vpn-menu") },
  { 0, XF86XK_AudioPlay,          spawn, SHCMD("/usr/local/share/scripts/cmus-play") },
  { 0, XF86XK_AudioNext,          spawn, SHCMD("/usr/local/share/scripts/cmus-next") },
  { 0, XF86XK_AudioPrev,          spawn, SHCMD("/usr/local/share/scripts/cmus-prev") },
  { 0, XF86XK_AudioStop,          spawn, SHCMD("/usr/local/share/scripts/cmus-stop") },
  { 0, XF86XK_AudioMute,          spawn, SHCMD("/usr/bin/pulseaudio-ctl mute && pkill -RTMIN+10 dwmblocks") },
  { 0, XF86XK_AudioRaiseVolume,   spawn, SHCMD("/usr/bin/pulseaudio-ctl up && pkill -RTMIN+10 dwmblocks") },
  { 0, XF86XK_AudioLowerVolume,   spawn, SHCMD("/usr/bin/pulseaudio-ctl down && pkill -RTMIN+10 dwmblocks") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click          event mask        button          function        argument */
  { ClkLtSymbol,    0,                Button1,        setlayout,      {0} },
  { ClkLtSymbol,    0,                Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,    0,                Button2,        zoom,           {0} },
  { ClkStatusText,  0,                Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,   MODKEY,           Button1,        movemouse,      {0} },
  { ClkClientWin,   MODKEY,           Button2,        togglefloating, {0} },
  { ClkClientWin,   MODKEY|ShiftMask, Button1,        resizemouse,    {0} },
  { ClkTagBar,      0,                Button1,        view,           {0} },
  { ClkTagBar,      0,                Button3,        toggleview,     {0} },
  { ClkTagBar,      MODKEY,           Button1,        tag,            {0} },
  { ClkTagBar,      MODKEY,           Button3,        toggletag,      {0} },
};
