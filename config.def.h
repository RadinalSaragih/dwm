/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 10; /* snap pixel */
static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor,  >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */
static const int showsystray = 1; /* 0 = no systray */
static const int showbar = 1; /* 0 = no bar */
static const int topbar = 1; /* 0 = bottom bar */
static const char *fonts[] = { "monospace:size=11" };
static const char dmenufont[] = "monospace:size=11";

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
#define TERM "st", "-g", "115x40"
#define FM "/usr/bin/ranger"
#define MPLAYER "/usr/bin/cmus"

/* scratchpads */
typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = { TERM, "-n", "sp-1", SHELL, NULL };
const char *spcmd2[] = { TERM, "-n", "sp-2", FM, NULL };
const char *spcmd4[] = { TERM, "-n", "sp-3", MPLAYER, NULL };

static Sp scratchpads[] = {
  /* name      cmd  */
  {"sp-1",    spcmd1},
  {"sp-2",    spcmd2},
  {"sp-3",    spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
  /* xprop(1):
   * WM_CLASS(STRING) = instance, class
   * WM_NAME(STRING) = title
   */
  /* class           instance      title   tags mask  iscentered isfloating monitor */
  { "firefox",      "Navigator",   NULL,    1 << 2,         0,         0,      -1 },
  {  NULL,           "sp-1",       NULL,    SPTAG(0),       1,         1,      -1 },
  {  NULL,           "sp-2",       NULL,    SPTAG(1),       1,         1,      -1 },
  {  NULL,           "sp-3",       NULL,    SPTAG(2),       1,         1,      -1 },
  {  NULL,           "sp-4",       NULL,    SPTAG(3),       1,         1,      -1 },
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
// To use floating mode add the following lines to the layouts and keybindings section.
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
//#include <X11/XF86keysym.h>

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont,
                                  "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor,
                                  "-sf", selfgcolor, "-p", "Choose a Program: ", NULL };

static const char *termcmd[] = { TERM, NULL };

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

  { MODKEY|ControlMask,           XK_h,      viewtoleft,     {0} },
  { MODKEY|ControlMask,           XK_l,      viewtoright,    {0} },

  { MODKEY|ShiftMask,             XK_h,      tagtoleft,      {0} },
  { MODKEY|ShiftMask,             XK_l,      tagtoright,     {0} },

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

  { MODKEY|Mod1Mask,              XK_grave,  spawn,          SHCMD("passmenu") },

  { MODKEY,                       XK_F5,     spawn,          SHCMD("brightnessctl -c backlight s 50-") },
  { MODKEY,                       XK_F6,     spawn,          SHCMD("brightnessctl -c backlight s 50+") },

  { SUPER,                        XK_F12,    spawn,          SHCMD("/usr/local/share/scripts/dm-pdf") },
  { MODKEY|Mod1Mask,              XK_Escape, spawn,          SHCMD("/usr/local/share/scripts/dm-SysMenu") },
  { 0,                            XK_Print,  spawn,          SHCMD("/usr/local/share/scripts/dm-screenshot") },

  { MODKEY,                       XK_F3,     spawn,          SHCMD("cmus-remote -R; pkill -RTMIN+5 dwmblocks") },
  { MODKEY,                       XK_F4,     spawn,          SHCMD("cmus-remote -S; pkill -RTMIN+5 dwmblocks") },
  { SUPER,                        XK_F7,     spawn,          SHCMD("cmus-remote -u; pkill -RTMIN+5 dwmblocks") },
  { SUPER,                        XK_F8,     spawn,          SHCMD("cmus-remote -s; pkill -RTMIN+5 dwmblocks") },
  { SUPER,                        XK_F6,     spawn,          SHCMD("cmus-remote -n; pkill -RTMIN+5 dwmblocks") },
  { SUPER,                        XK_F5,     spawn,          SHCMD("cmus-remote -r; pkill -RTMIN+5 dwmblocks") },

  { MODKEY,                       XK_F1,     spawn,          SHCMD("cmus-remote -v -5%; pkill -RTMIN+4 dwmblocks") },
  { MODKEY,                       XK_F2,     spawn,          SHCMD("cmus-remote -v +5%; pkill -RTMIN+4 dwmblocks") },

  { SUPER,                        XK_F3,     spawn,          SHCMD("pamixer -t; pkill -RTMIN+10 dwmblocks") },
  { SUPER,                        XK_F1,     spawn,          SHCMD("pamixer --allow-boost -d 5; pkill -RTMIN+10 dwmblocks") },
  { SUPER,                        XK_F2,     spawn,          SHCMD("pamixer --allow-boost -i 5; pkill -RTMIN+10 dwmblocks") },

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
  { ClkStatusText,  0,                Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,   MODKEY,           Button1,        movemouse,      {0} },
  { ClkClientWin,   MODKEY,           Button2,        togglefloating, {0} },
  { ClkClientWin,   MODKEY|ShiftMask, Button1,        resizemouse,    {0} },
  { ClkTagBar,      0,                Button1,        view,           {0} },
  { ClkTagBar,      0,                Button3,        toggleview,     {0} },
  { ClkTagBar,      MODKEY,           Button1,        tag,            {0} },
  { ClkTagBar,      MODKEY,           Button3,        toggletag,      {0} },
};
