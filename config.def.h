/* appearance */
static unsigned int snap = 10; /* snap pixel */
static unsigned int borderpx = 1; /* border pixel of windows */
static unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor,  >0: pin systray to monitor X */
static unsigned int systrayspacing = 2; /* systray spacing */
static unsigned int systrayonleft  = 0; /* systray on the left of status text */
static int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor */
static int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */
static int focusonwheel = 0;
static int showsystray = 0; /* 0 = no systray */
static int showbar = 1; /* 0 = no bar */
static int topbar = 1; /* 0 = bottom bar */
static int noborder = 1; /* 1 = hide the bar if only a single window is opened. */
static char font[] = "monospace:size=11";
static const char *fonts[] = { font };

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

/* static char scratchpad1[] = "/bin/ranger"; */
/* static char scratchpad2[] = "/bin/newsboat"; */
/* static char scratchpad3[] = "/bin/cmus"; */
/* static char scratchpad4[] = "/bin/elinks"; */

/* scratchpads */
typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd0[] = { "st", "-n", "sp-0", NULL };
const char *spcmd1[] = { "st", "-n", "sp-1", NULL };
const char *spcmd2[] = { "st", "-n", "sp-2", NULL };
const char *spcmd3[] = { "st", "-n", "sp-3", NULL };
const char *spcmd4[] = { "st", "-n", "sp-4", NULL };

static Sp scratchpads[] = {
    /* name      cmd  */
    {"sp-0",    spcmd0},
    {"sp-1",    spcmd1},
    {"sp-2",    spcmd2},
    {"sp-3",    spcmd3},
    {"sp-4",    spcmd4},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
    /* xprop(1):
     * WM_CLASS(STRING) = instance, class
     * WM_NAME(STRING) = title */
    /* class          instance      title    tags mask   iscentered isfloating monitor */
    { NULL,           "sp-0",       NULL,     SPTAG(0),       1,         1,      -1 },
    { NULL,           "sp-1",       NULL,     SPTAG(1),       1,         1,      -1 },
    { NULL,           "sp-2",       NULL,     SPTAG(2),       1,         1,      -1 },
    { NULL,           "sp-3",       NULL,     SPTAG(3),       1,         1,      -1 },
    { NULL,           "sp-4",       NULL,     SPTAG(4),       1,         1,      -1 },
};

/* layout(s) */
static float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static int nmaster = 1; /* number of clients in master area */
static int resizehints = 0; /* 1 = respect size hints in tiled resizals */
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "TILED-R",      tile        }, /* first entry is default */
    { "TILED-L",      left_stack  },
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
#include "modules/view_adjacent.c"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-p", ">", NULL };
static const char *termcmd[] = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "font",                    STRING,  &font },
    { "normbgcolor",             STRING,  &normbgcolor },
    { "normbordercolor",         STRING,  &normbordercolor },
    { "normfgcolor",             STRING,  &normfgcolor },
    { "selbgcolor",              STRING,  &selbgcolor },
    { "selbordercolor",          STRING,  &selbordercolor },
    { "selfgcolor",              STRING,  &selfgcolor },
    { "borderpx",          	     INTEGER, &borderpx },
    { "snap",          	         INTEGER, &snap },
    { "showbar",          	     INTEGER, &showbar },
    { "topbar",          	     INTEGER, &topbar },
    { "nmaster",          	     INTEGER, &nmaster },
    { "resizehints",       	     INTEGER, &resizehints },
    { "mfact",      	         FLOAT,   &mfact },
    { "focusonwheel",            INTEGER, &focusonwheel},
    { "lockfullscreen",          INTEGER, &lockfullscreen},
    { "showsystray",             INTEGER, &showsystray },
    { "systraypinning",          INTEGER, &systraypinning },
    { "systrayspacing",          INTEGER, &systrayspacing },
    { "systrayonleft",           INTEGER, &systrayonleft },
    { "systraypinningfailfirst", INTEGER, &systraypinningfailfirst },
    /* { "scratchpad1",             STRING,  &scratchpad1 }, */
    /* { "scratchpad2",             STRING,  &scratchpad2 }, */
    /* { "scratchpad3",             STRING,  &scratchpad3 }, */
    /* { "scratchpad4",             STRING,  &scratchpad4 }, */
    { "noborder",                INTEGER, &noborder },
};


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
    { MODKEY,                       XK_e,      togglescratch,  {.ui = 2 } },
    { MODKEY,                       XK_grave,  togglescratch,  {.ui = 3 } },
    { MODKEY|ShiftMask,             XK_grave,  togglescratch,  {.ui = 4 } },

    { MODKEY|ShiftMask,             XK_o,      spawn,          SHCMD("$BROWSER") },
    { MODKEY|Mod1Mask|ControlMask,  XK_Escape, quit,           {0} }, // quit WM
    { MODKEY|Mod1Mask,              XK_r,      quit,           {1} }, // reload WM
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
