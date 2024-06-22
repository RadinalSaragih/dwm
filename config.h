// vim: set ft=c;
/* clang-format off */

#include <stdbool.h>
#include <stddef.h>

/* appearance */
static unsigned int snap            = 10; /* snap pixel */
static unsigned int borderpx        = 1; /* border pixel of windows */
static bool systraypinning          = false; /* 0: sloppy systray follows selected monitor,  >0: pin systray to monitor X */
static unsigned int systrayspacing  = 2; /* systray spacing */
static bool systrayonleft           = false; /* systray on the left of status text */
static bool systraypinningfailfirst = true; /* 1: if pinning fails, display systray on the first monitor, false: display systray on the last monitor */
static bool swallowfloating         = false; /* 1 means swallow floating windows by default */
static bool lockfullscreen          = false; /* 1 will force focus on the fullscreen window */
static bool focusonwheel            = false;
static bool showsystray             = false; /* 0 = no systray */
static bool showbar                 = true; /* 0 = no bar */
static bool topbar                  = true; /* 0 = bottom bar */
static bool inverse_statusbar       = false;
static int gappx                    = 5;  /* gaps between windows */
static bool noborder                = true; /* 1 = hide the bar if only a single window is opened. */
static bool showtitle               = false; /* 1 = hide the bar if only a single window is opened. */
static char font[]                  = "Liberation Mono:style=Regular:pixelsize=10";
static char wmname[]                = "dwm";

static const char *fonts[] = { font };

/* max number of character that one block command can output */
#define CMDLENGTH 80

/* inverse the order of the blocks */
static bool block_inversed = false;

/* colors */
static char normbg[]     = "#222222";
static char normborder[] = "#444444";
static char normfg[]     = "#bbbbbb";
static char selfg[]      = "#eeeeee";
static char selborder[]  = "#005577";
static char selbg[]      = "#005577";
static char normstickyborder[] = "#719611";
static char selstickyborder[]  = "#aa4450";
static char normfloatborder[]  = "#719611";
static char selfloatborder[]   = "#aa4450";

static char statusbg[]   = "#222222";
static char statusfg_1[] = "#005577";
static char statusfg_2[] = "#005577";
static char statusfg_3[] = "#005577";
static char statusfg_4[] = "#005577";
static char statusfg_5[] = "#005577";

static char *colors[][5] = {
	/* 		            fg	    bg	    border		sticky border     float border  */
	[SchemeNorm]    = { normfg, normbg, normborder, normstickyborder, normfloatborder },
	[SchemeSel]     = { selfg,  selbg,  selborder,  selstickyborder, selfloatborder },
	[SchemeStatus]  = { normfg,  statusbg },
};

/* status bar */
static const Block blocks[] = {
	/* f           command	                        interval	signal */
	{ statusfg_1, "date '+%a, %d %b %y - %I:%M%p'",	30, 0 },
	{ statusfg_2, "dwm_status_memory_usage",	    60,	1 },
	{ statusfg_3, "dwm_status_volume_levels",	    60, 2 },
	{ statusfg_4, "dwm_status_battery_stat",	    10, 3 },
	{ statusfg_5, "dwm_status_cmus_status",		    60,	2 },
};

/* delimeter between blocks commands. NULL character ('\0') means no delimeter. */
static char delimiter[] = " | ";

/* scratchpads */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd0[] = { "st", "-A", "1", "-g", "150x35", "-T", "SP-0", "-n", "sp-0", NULL };
const char *spcmd1[] = { "st", "-A", "1", "-g", "150x35", "-T", "SP-1", "-n", "sp-1", NULL };
const char *spcmd2[] = { "st", "-A", "1", "-g", "150x35", "-T", "SP-2", "-n", "sp-2", NULL };
const char *spcmd3[] = { "st", "-A", "1", "-g", "150x35", "-T", "SP-3", "-n", "sp-3", NULL };

static Sp scratchpads[] = {
	/* name 	cmd  */
	{ "sp-0", 	spcmd0 },
	{ "sp-1", 	spcmd1 },
	{ "sp-2", 	spcmd2 },
	{ "sp-3", 	spcmd3 },
};

#define TAG(n) 1 << (n-1)
#define ANYTAG 0

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };
static const Rule rules[] = {
	/* xprop(1):
	 * WM_CLASS(STRING) = instance, class
	 * WM_NAME(STRING) = title */
	/* class           instance title  tags mask centered floating terminal noswallow monitor */
	{ "firefox",   	   NULL,    NULL,  TAG(9),   false,   false,   false,   true, 	-1 },
	{ "Firefox-esr",   NULL,    NULL,  TAG(9),   false,   false,   false,   true, 	-1 },
	{ "Brave-browser", NULL,    NULL,  TAG(9),   false,   false,   false,   true, 	-1 },
	{ "qutebrowser",   NULL,    NULL,  TAG(9),   false,   false,   false,   true, 	-1 },
	{ "Steam",         NULL,    NULL,  TAG(9),   false,   false,   false,   true, 	-1 },

	{ "Inkscape",      NULL,    NULL,  TAG(8),   false,   false,   false,   true, 	-1 },
	{ "Gimp",          NULL,    NULL,  TAG(7),   false,   false,   false,   true, 	-1 },

	{ "lmms",          NULL,    NULL,  TAG(6),   false,   false,   false,   true, 	-1 },
	{ "kdenlive",      NULL,    NULL,  TAG(6),   false,   false,   false,   true, 	-1 },

	{ "Godot",         NULL,    NULL,  TAG(5),   false,   false,   false,   true, 	-1 },

	{ "QtCreator",     NULL,    NULL,  TAG(3),   false,   false,   false,   true, 	-1 },

	{ "st-256color",   NULL,    NULL,  ANYTAG,   false,   false,   true,    false,	-1 },
	{ "Zathura",       NULL,    NULL,  ANYTAG,   false,   false,   false,   true,   -1 },
	{ NULL, 	   "sp-0",  "SP-0",  SPTAG(0), true,    true,    true,    false,  -1 },
	{ NULL, 	   "sp-1",  "SP-1",  SPTAG(1), true,    true,    true,    false,  -1 },
	{ NULL, 	   "sp-2",  "SP-2",  SPTAG(2), true,    true,    true,    false,  -1 },
	{ NULL, 	   "sp-3",  "SP-3",  SPTAG(3), true,    true,    true,    false,	-1 },
	{ NULL, 	   NULL,    "Event Tester", ANYTAG, false,  false,   false,   true, -1 },
};

#undef TAG
#undef ANYTAG

/* layout(s) */
static float mfact = 0.50f; /* factor of master area size [0.05..0.95] */
static int nmaster = 1; /* number of clients in master area */
static bool resizehints = false; /* 1 = respect size hints in tiled resizals */
static const Layout layouts[] = {
	/* first entry is default */
	/* symbol 	arrange function */
	{ "T",		tile	},
	{ "M",		monocle },
 	{ "[D]",	deck	},
	{ "F",		NULL	},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG)                                                \
{ MODKEY,			KEY,	view,		{.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,		KEY,	toggleview, 	{.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,		KEY,	tag,		{.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,	toggletag,	{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* mediakey's keycodes */
#include <X11/XF86keysym.h>

/* commands */
static const char *dmenucmd[]	        = { "dmenu_run", "-g", "3", "-l", "15", "-i", "-p", "RUN:", NULL };
static const char *termcmd[]	        = { "st", NULL };
static const char cmus_decvol[]         = "cmus-remote -v -1%; kill -37 $(pidof dwm)";
static const char cmus_incvol[]         = "cmus-remote -v +1%; kill -37 $(pidof dwm)";
static const char cmus_pause[]	        = "cmus-remote -u; kill -37 $(pidof dwm)";
static const char cmus_stop[]	        = "cmus-remote -s; kill -37 $(pidof dwm)";
static const char cmus_next[]	        = "cmus-remote -n; kill -37 $(pidof dwm)";
static const char cmus_prev[]	        = "cmus-remote -r; kill -37 $(pidof dwm)";
static const char cmus_repeat[]         = "cmus-remote -R; kill -37 $(pidof dwm)";
static const char vol_dec[]	            = "pactl set-sink-volume @DEFAULT_SINK@ -1%; kill -37 $(pidof dwm)";
static const char vol_inc[]	            = "pactl set-sink-volume @DEFAULT_SINK@ +1%; kill -37 $(pidof dwm)";
static const char vol_mute[]	        = "pactl set-sink-mute @DEFAULT_SINK@ toggle; kill -37 $(pidof dwm)";
static const char mic_decvol[]	        = "pactl set-source-volume @DEFAULT_SOURCE@ -1%; kill -37 $(pidof dwm)";
static const char mic_incvol[]	        = "pactl set-source-volume @DEFAULT_SOURCE@ +1%; kill -37 $(pidof dwm)";
static const char mic_mute[]	        = "pactl set-source-mute @DEFAULT_SOURCE@ toggle; kill -37 $(pidof dwm)";
static const char screenshot[]	        = "dm-screenshot";
static const char browser[]	            = "qutebrowser";
static const char powermenu[]	        = "dm-power";
static const char winmenu[]	            = "dm-winmenu";
static const char xmouseless[]	        = "xmouseless";
static const char launch_tmux[]	        = "st -e tmux_start";
static const char launch_dm_menu[]      = "dm-menu";
static const char dunstctl_show_all[]   = "dunstctl history-pop";
static const char dunstctl_close_all[]  = "dunstctl close-all";
static const char dunstctl_close_top[]  = "dunstctl close";
static const char backlight_up[]        = "brightnessctl set +1%";
static const char backlight_down[]      = "brightnessctl set 1-%";

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
	{ "font", 			         STRING,    &font },
	{ "wmname", 			     STRING,    &wmname },
	{ "normbg",			         RGBHEX,    &normbg },
	{ "normborder", 		     RGBHEX,    &normborder },
	{ "normfg",			         RGBHEX,    &normfg },
	{ "selbg",			         RGBHEX,    &selbg },
	{ "selborder",		         RGBHEX,    &selborder },
	{ "selfg",			         RGBHEX,    &selfg },
	{ "selstickyborder", 		 RGBHEX,    &selstickyborder },
	{ "normstickyborder",		 RGBHEX,    &normstickyborder },
	{ "selfloatborder", 		 RGBHEX,    &selfloatborder },
	{ "normfloatborder",		 RGBHEX,    &normfloatborder },
	{ "statusfg_1",			     RGBHEX,	&statusfg_1 },
	{ "statusfg_2",			     RGBHEX,	&statusfg_2 },
	{ "statusfg_3",			     RGBHEX,	&statusfg_3 },
	{ "statusfg_4",			     RGBHEX,	&statusfg_4 },
	{ "statusfg_5",			     RGBHEX,	&statusfg_5 },
	{ "statusbg",			     RGBHEX,	&statusbg   },
	{ "borderpx", 			     INTEGER,	&borderpx },
	{ "snap", 			         INTEGER,	&snap },
	{ "nmaster", 			     INTEGER,	&nmaster },
	{ "systrayspacing", 	     INTEGER, 	&systrayspacing },
	{ "gaps",			         INTEGER,	&gappx },
	{ "showbar", 			     BOOLEAN,	&showbar },
	{ "topbar", 			     BOOLEAN,	&topbar },
	{ "resizehints", 		     BOOLEAN,	&resizehints },
	{ "focusonwheel", 		     BOOLEAN, 	&focusonwheel},
	{ "lockfullscreen",          BOOLEAN, 	&lockfullscreen},
	{ "showsystray", 		     BOOLEAN, 	&showsystray },
	{ "systraypinning",          BOOLEAN, 	&systraypinning },
	{ "systrayonleft", 		     BOOLEAN, 	&systrayonleft },
	{ "systraypinningfailfirst", BOOLEAN, 	&systraypinningfailfirst },
	{ "noborder", 			     BOOLEAN, 	&noborder },
	{ "showtitle", 			     BOOLEAN, 	&showtitle },
	{ "swallowfloating", 		 BOOLEAN, 	&swallowfloating },
	{ "block_inversed",          BOOLEAN,   &block_inversed },
	{ "status_color_inversed",	 BOOLEAN,	&inverse_statusbar},
	{ "mfact", 			         FLOAT,		&mfact },
	{ "block_delimiter",         CHAR,		&delimiter[1] },
};

/* keybindings */
static const Key keys[] = {
	/* modifier		key 			            function	argument */
	{ MODKEY,		        XK_space,		    spawn,		{.v = dmenucmd } },
	{ MODKEY,		        XK_Return,		    spawn,		{.v = termcmd } },

	{ MODKEY,		        XK_b,			    togglebar,	{0} },

	{ MODKEY,		        XK_j,			    focusstack,	{.i = +1 } },
	{ MODKEY,		        XK_k,			    focusstack,	{.i = -1 } },

	{ MODKEY|ControlMask,	XK_equal,		    incnmaster,	{.i = +1 } },
	{ MODKEY|ControlMask,	XK_minus,		    incnmaster,	{.i = -1 } },

	{ MODKEY,		        XK_h,			    setmfact,	{.f = -0.05} },
	{ MODKEY,		        XK_l,			    setmfact,	{.f = +0.05} },

	{ MODKEY|ShiftMask,	    XK_c,			    killclient,	{0} },

	{ MODKEY,		        XK_bracketleft,	    setlayout,	{.v = &layouts[0]} },
	{ MODKEY|ShiftMask,	    XK_bracketleft,	    setlayout,	{.v = &layouts[1]} },
	{ MODKEY,		        XK_bracketright,    setlayout,	{.v = &layouts[2]} },
	{ MODKEY|ShiftMask,     XK_bracketright,    setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ControlMask,   XK_space, 		    setlayout, 	{0} },

	{ MODKEY,		        XK_s,               togglefloating,	{0} },
	{ MODKEY,		        XK_f, 			    togglefullscr, 	{0} },
	{ MODKEY,		        XK_x, 			    togglesticky,	{0} },

	{ MODKEY|ShiftMask,	    XK_j, 			    pushdown,	{0} },
	{ MODKEY|ShiftMask,	    XK_k, 			    pushup,		{0} },
	{ MODKEY|ShiftMask,	    XK_h, 			    view_adjacent, 	{.i = -1 } },
	{ MODKEY|ShiftMask,	    XK_l, 			    view_adjacent, 	{.i = +1 } },

	{ MODKEY,		        XK_Down, 		    moveresize,	{.v = "0x 25y 0w 0h" } },
	{ MODKEY,		        XK_Up,			    moveresize,	{.v = "0x -25y 0w 0h" } },
	{ MODKEY,		        XK_Right, 		    moveresize,	{.v = "25x 0y 0w 0h" } },
	{ MODKEY,		        XK_Left,		    moveresize,	{.v = "-25x 0y 0w 0h" } },

	{ MODKEY|ShiftMask,	    XK_Down,		    moveresize,	{.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,	    XK_Up,			    moveresize,	{.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,	    XK_Right, 		    moveresize,	{.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,	    XK_Left,		    moveresize,	{.v = "0x 0y -25w 0h" } },

	{ MODKEY|ControlMask,	XK_Up,			    moveresizeedge,	{.v = "t"} },
	{ MODKEY|ControlMask,	XK_Down,		    moveresizeedge,	{.v = "b"} },
	{ MODKEY|ControlMask,	XK_Left,		    moveresizeedge,	{.v = "l"} },
	{ MODKEY|ControlMask,	XK_Right, 		    moveresizeedge,	{.v = "r"} },

	{ MODKEY|ControlMask|ShiftMask,	XK_Up,	    moveresizeedge,	{.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask,	XK_Down,    moveresizeedge,	{.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask,	XK_Left,    moveresizeedge,	{.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask,	XK_Right,moveresizeedge,	{.v = "R"} },

	{ MODKEY,		        XK_n, 			    focusmaster,	{0} },
	{ MODKEY|ShiftMask,	    XK_n,			    zoom,		    {0} },
	{ MODKEY,		        XK_Tab, 		    view,		    {0} },

	{ MODKEY,		        XK_0, 			    view,		    {.ui = ~0 } },
	{ MODKEY|ShiftMask,	    XK_0, 			    tag, 		    {.ui = ~0 } },

	{ MODKEY,		        XK_comma, 		    focusmon,	    {.i = -1 } },
	{ MODKEY,		        XK_period, 		    focusmon,	    {.i = +1 } },

	{ MODKEY|ShiftMask,	    XK_comma, 		    tagmon,	 	    {.i = -1 } },
	{ MODKEY|ShiftMask,	    XK_period, 		    tagmon,	 	    {.i = +1 } },

	TAGKEYS( 		XK_1, 					0 )
	TAGKEYS( 		XK_2, 					1 )
	TAGKEYS( 		XK_3, 					2 )
	TAGKEYS( 		XK_4, 					3 )
	TAGKEYS( 		XK_5, 					4 )
	TAGKEYS( 		XK_6, 					5 )
	TAGKEYS( 		XK_7, 					6 )
	TAGKEYS( 		XK_8, 					7 )
	TAGKEYS( 		XK_9, 					8 )

	{ MODKEY,	 	    XK_u, 			togglescratch, 	{.ui = 0 } },
	{ MODKEY,	 	    XK_i, 			togglescratch, 	{.ui = 1 } },
	{ MODKEY,	 	    XK_o, 			togglescratch, 	{.ui = 2 } },
	{ MODKEY,	 	    XK_p, 			togglescratch, 	{.ui = 3 } },

	{ MODKEY|Mod1Mask|ControlMask, 	XK_Escape,	quit,		{0} },	// quit WM
	{ MODKEY|Mod1Mask, 	XK_r, 			quit,		{1} },	// reload WM
	{ MODKEY|Mod1Mask,	XK_Escape,		spawn, 	 	SHCMD(powermenu) }, // powermenu

	{ MODKEY|ShiftMask,	XK_space, 	 	spawn, 	 	SHCMD(winmenu) },

	{ MODKEY|ShiftMask,	XK_o, 		 	spawn, 	 	SHCMD(browser) },
	{ MODKEY,		    XK_Print, 	 	spawn, 	 	SHCMD(screenshot) },
	{ MODKEY,		    XK_m, 	 		spawn, 	 	SHCMD(xmouseless) },

	{ MODKEY|ShiftMask,	XK_Page_Down,   spawn, 	    SHCMD(cmus_decvol) },
	{ MODKEY|ShiftMask,	XK_Page_Up,		spawn, 		SHCMD(cmus_incvol) },
	{ MODKEY|ShiftMask,	XK_End,			spawn, 		SHCMD(cmus_prev) },
	{ MODKEY|ShiftMask,	XK_Home,		spawn, 		SHCMD(cmus_next) },
	{ MODKEY|ShiftMask,	XK_Delete,		spawn, 		SHCMD(cmus_pause) },
	{ MODKEY|ShiftMask,	XK_Insert,		spawn, 		SHCMD(cmus_repeat) },

	{ MODKEY|ShiftMask, XK_minus,		spawn, 		SHCMD(vol_dec) },
	{ MODKEY|ShiftMask,	XK_equal,		spawn, 		SHCMD(vol_inc) },
	{ MODKEY|ShiftMask,	XK_BackSpace,	spawn, 		SHCMD(vol_mute) },

	{ 0,    XF86XK_AudioLowerVolume,    spawn, 		SHCMD(vol_dec) },
	{ 0,    XF86XK_AudioRaiseVolume,    spawn, 		SHCMD(vol_inc) },
	{ 0,    XF86XK_AudioMute,	        spawn, 		SHCMD(vol_mute) },

	{ MODKEY|ControlMask, 	XK_End,     spawn, 		SHCMD(mic_decvol) },
	{ MODKEY|ControlMask, 	XK_Home,	spawn, 		SHCMD(mic_incvol) },
	{ MODKEY|ControlMask, 	XK_Delete,	spawn, 		SHCMD(mic_mute) },
	{ 0,    XF86XK_AudioMicMute,	    spawn, 		SHCMD(mic_mute) },

	{ 0,    XF86XK_LaunchA,	        	spawn, 		SHCMD(launch_dm_menu) },
	{ MODKEY|ShiftMask,	XK_F11,			spawn, 		SHCMD(launch_dm_menu) },

	{ MODKEY|ShiftMask,	XK_F6,			spawn, 		SHCMD(backlight_up) },
	{ MODKEY|ShiftMask,	XK_F5,			spawn,		SHCMD(backlight_down) },
	{ 0,    XF86XK_MonBrightnessUp,	    spawn, 		SHCMD(backlight_up) },
	{ 0,    XF86XK_MonBrightnessDown,   spawn, 	    SHCMD(backlight_down) },

	{ MODKEY,		    XK_F11,	        spawn, 		SHCMD(dunstctl_show_all) },
	{ MODKEY,		    XK_F12,	        spawn, 		SHCMD(dunstctl_close_top) },
	{ MODKEY|ShiftMask,	XK_F12,			spawn, 		SHCMD(dunstctl_close_all) },

	{ MODKEY|ShiftMask,	XK_Return,		spawn,		SHCMD(launch_tmux) },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click event 		mask 			    button 		function 		argument */
	{ ClkLtSymbol,		0,			        Button1,    setlayout,		{0} },
	{ ClkLtSymbol,		0,			        Button3,    setlayout,		{.v = &layouts[2]} },
	{ ClkWinTitle,		0,			        Button2,    zoom,			{0} },
	{ ClkStatusText,	0,			        Button2,    spawn,			{.v = termcmd } },
	{ ClkClientWin,		MODKEY,		        Button1,    movemouse,		{0} },
	{ ClkClientWin,		MODKEY,		        Button2,    togglefloating, {0} },
	{ ClkClientWin,		MODKEY|ShiftMask,   Button1,    resizemouse,	{0} },

	{ ClkStatusText,    0,                  Button1,    sendstatusbar,  {.i = 1 } },
	{ ClkStatusText,    0,                  Button2,    sendstatusbar,  {.i = 2 } },
	{ ClkStatusText,    0,                  Button3,    sendstatusbar,  {.i = 3 } },
	{ ClkStatusText,    0,                  Button4,    sendstatusbar,  {.i = 4 } },
	{ ClkStatusText,    0,                  Button5,    sendstatusbar,  {.i = 5 } },
	{ ClkStatusText,    ShiftMask,          Button1,    sendstatusbar,  {.i = 6 } },

	{ ClkTagBar,		0,			        Button1,    view,			{0} },
	{ ClkTagBar,		0,			        Button3,    toggleview,		{0} },
	{ ClkTagBar,		MODKEY,			    Button1,    tag,			{0} },
	{ ClkTagBar,		MODKEY,			    Button3,    toggletag,		{0} },
};
/* clang-format on */
