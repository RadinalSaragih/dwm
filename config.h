/* appearance */
static unsigned int snap = 10; /* snap pixel */
static unsigned int borderpx = 1; /* border pixel of windows */
static unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor,  >0: pin systray to monitor X */
static unsigned int systrayspacing = 2; /* systray spacing */
static unsigned int systrayonleft  = 0; /* systray on the left of status text */
static int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor */
static int swallowfloating = 0; /* 1 means swallow floating windows by default */
static int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */
static int focusonwheel = 0;
static int showsystray = 0; /* 0 = no systray */
static int showbar = 1; /* 0 = no bar */
static int topbar = 1; /* 0 = bottom bar */
static int noborder = 1; /* 1 = hide the bar if only a single window is opened. */
static int showtitle = 0; /* 1 = hide the bar if only a single window is opened. */
static char font[] = "Liberation Mono:style=Regular:pixelsize=10";
static const char *fonts[] = { font };

/* colors */
static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#005577";
static char selbgcolor[]      = "#005577";
static char normspcbordercolor[] = "#719611";
static char selspcbordercolor[]  = "#aa4450";
static char *colors[][4] = {
	/* 		    fg 		bg 		border 		SPC_BORDER*/
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor, normspcbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor,  selspcbordercolor },
};

/* scratchpads */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd0[] = { "st", "-T", "SP-0", "-n", "sp-0", NULL };
const char *spcmd1[] = { "st", "-T", "SP-1", "-n", "sp-1", NULL };
const char *spcmd2[] = { "st", "-T", "SP-2", "-n", "sp-2", NULL };
const char *spcmd3[] = { "st", "-T", "SP-3", "-n", "sp-3", NULL };

static Sp scratchpads[] = {
	/* name 	cmd  */
	{ "sp-0", 	spcmd0 },
	{ "sp-1", 	spcmd1 },
	{ "sp-2", 	spcmd2 },
	{ "sp-3", 	spcmd3 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 * WM_CLASS(STRING) = instance, class
	 * WM_NAME(STRING) = title */
	/* class          instance      title 	tags mask   centered floating terminal noswallow monitor */
	{ "Steam",        NULL, 	NULL, 	1 << 8, 	0, 	0, 	0, 	1, 	-1 },
	{ "retroarch",    NULL, 	NULL, 	1 << 8, 	0, 	0, 	0, 	1, 	-1 },
	{ "dolphin-emu",  NULL, 	NULL, 	1 << 8, 	0, 	0, 	0, 	1, 	-1 },
	{ "PPSSPPSDL", 	  NULL, 	NULL, 	1 << 8, 	0, 	0, 	0, 	1, 	-1 },
	{ "Pcsx2", 	  NULL, 	NULL, 	1 << 8, 	0, 	0, 	0, 	1, 	-1 },
	{ "pyrogenesis",  NULL, 	NULL, 	1 << 8, 	0, 	0, 	0, 	1, 	-1 },
	{ "Freeciv-gtk3.22",  NULL, 	NULL, 	1 << 8, 	0, 	0, 	0, 	1, 	-1 },
	{ "Lutris",  	  NULL, 	NULL, 	1 << 8, 	0, 	0, 	0, 	1, 	-1 },
	{ NULL,  	  NULL, 	"WineDesktop - Wine desktop", 	1 << 8, 	0, 	0, 	0, 	1, 	-1 },

	{ "Godot",        NULL, 	NULL, 	1 << 7, 	0, 	0, 	0, 	1, 	-1 },
	{ "Inkscape",     NULL, 	NULL, 	1 << 7, 	0, 	0, 	0, 	1, 	-1 },
	{ "Gimp",         NULL, 	NULL, 	1 << 7, 	0, 	0, 	0, 	1, 	-1 },
	{ "lmms",         NULL, 	NULL, 	1 << 7, 	0, 	0, 	0, 	1, 	-1 },
	{ "kdenlive",     NULL, 	NULL, 	1 << 7, 	0, 	0, 	0, 	1, 	-1 },

	{ "Firefox-esr",  NULL, 	NULL, 	1 << 6, 	0, 	0, 	0, 	1, 	-1 },
	{ "qutebrowser",  NULL, 	NULL, 	1 << 6, 	0, 	0, 	0, 	1, 	-1 },

	{ "Virt-manager", NULL, 	NULL, 	1 << 5, 	0, 	0, 	0, 	1, 	-1 },

	{ "St", NULL, NULL, 0, 0, 0, 1, 0, -1 },
	{ NULL, NULL, "Event Tester", 0, 0, 0, 0, 1, -1 },
	{ NULL, "sp-0", NULL, SPTAG(0), 1, 1, 1, 0, -1 },
	{ NULL, "sp-1", NULL, SPTAG(1), 1, 1, 1, 0, -1 },
	{ NULL, "sp-2", NULL, SPTAG(2), 1, 1, 1, 0, -1 },
	{ NULL, "sp-3", NULL, SPTAG(3), 1, 1, 1, 0, -1 },
};

/* layout(s) */
static float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static int nmaster = 1; /* number of clients in master area */
static int resizehints = 0; /* 1 = respect size hints in tiled resizals */
static const Layout layouts[] = {
	/* first entry is default */
	/* symbol 	arrange function */
	{ "T",		tile 	}, 
	{ "M",		monocle },
	{ "F",		NULL 	},
};

/* key definitions */
#define MODKEY Mod4Mask
#define HYPER Mod3Mask
#define TAGKEYS(KEY,TAG)                                                \
{ MODKEY,			KEY,	view,		{.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,		KEY,	toggleview, 	{.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,		KEY,	tag,		{.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,	toggletag,	{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* mediakey's keycodes */
/* #include <X11/XF86keysym.h> */

/* define view_adjacent */
#include "modules/view_adjacent.c"

/* extra variables */
static const char cmus_decvol[] = "cmus-remote -v -1%; pkill -RTMIN+5 dwmblocks";
static const char cmus_incvol[] = "cmus-remote -v +1%; pkill -RTMIN+5 dwmblocks";
static const char cmus_pause[] = "cmus-remote -u; pkill -RTMIN+5 dwmblocks";
static const char cmus_stop[] = "cmus-remote -s; pkill -RTMIN+5 dwmblocks";
static const char cmus_next[] = "cmus-remote -n; pkill -RTMIN+5 dwmblocks";
static const char cmus_prev[] = "cmus-remote -r; pkill -RTMIN+5 dwmblocks";
static const char cmus_repeat[] = "cmus-remote -R; pkill -RTMIN+5 dwmblocks";
static const char cmus_shuffle[] = "cmus-remote -S; pkill -RTMIN+5 dwmblocks";
static const char vol_dec[] = "pactl set-sink-volume $(pactl info | awk '/Default Sink/ {print $3}') -1%; pkill -RTMIN+10 dwmblocks";
static const char vol_inc[] = "pactl set-sink-volume $(pactl info | awk '/Default Sink/ {print $3}') +1%; pkill -RTMIN+10 dwmblocks";
static const char vol_mute[] = "pactl set-sink-mute $(pactl info | awk '/Default Sink/ {print $3}') toggle; pkill -RTMIN+10 dwmblocks";
static const char mic_decvol[] = "pactl set-source-volume $(pactl info | awk '/Default Source/ {print $3}') -1%; pkill -RTMIN+12 dwmblocks";
static const char mic_incvol[] = "pactl set-source-volume $(pactl info | awk '/Default Source/ {print $3}') +1%; pkill -RTMIN+12 dwmblocks";
static const char mic_mute[] = "pactl set-source-mute $(pactl info | awk '/Default Source/ {print $3}') toggle; pkill -RTMIN+12 dwmblocks";
static const char powermenu[] = "echo 'xsecurelock\n' 'pkill -15 Xorg\n' 'systemctl reboot\n' 'systemctl poweroff\n' | dmenu -m -1 | /bin/sh";
static const char screenshot[] = "maim -u -f png -m 1 $HOME/Pictures/Screenshot/screenshot-$(date '+%d-%m-%y@%h:%m:%s').png";

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-p", ">", NULL };
static const char *termcmd[] = { "st", NULL };

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
	{ "fonts", 			STRING, 	&font },
	{ "normbgcolor", 		STRING, 	&normbgcolor },
	{ "normbordercolor", 		STRING, 	&normbordercolor },
	{ "normfgcolor",  		STRING, 	&normfgcolor },
	{ "selbgcolor", 		STRING, 	&selbgcolor },
	{ "selbordercolor", 		STRING, 	&selbordercolor },
	{ "selfgcolor", 		STRING, 	&selfgcolor },
	{ "selspcbordercolor", 		STRING, 	&selspcbordercolor },
	{ "normspcbordercolor", 	STRING, 	&normspcbordercolor },
	{ "borderpx", 			INTEGER,	&borderpx },
	{ "snap", 			INTEGER,	&snap },
	{ "showbar", 			INTEGER,	&showbar },
	{ "topbar", 			INTEGER,	&topbar },
	{ "nmaster", 			INTEGER,	&nmaster },
	{ "resizehints", 		INTEGER,	&resizehints },
	{ "mfact", 			FLOAT,		&mfact },
	{ "focusonwheel", 		INTEGER, 	&focusonwheel},
	{ "lockfullscreen", 		INTEGER, 	&lockfullscreen},
	{ "showsystray", 		INTEGER, 	&showsystray },
	{ "systraypinning", 		INTEGER, 	&systraypinning },
	{ "systrayspacing", 		INTEGER, 	&systrayspacing },
	{ "systrayonleft", 		INTEGER, 	&systrayonleft },
	{ "systraypinningfailfirst", 	INTEGER, 	&systraypinningfailfirst },
	{ "noborder", 			INTEGER, 	&noborder },
	{ "showtitle", 			INTEGER, 	&showtitle },
	{ "swallowfloating", 		INTEGER, 	&swallowfloating }, 
};

/* keybindings */
static Key keys[] = {
	/* modifier		key 			function	argument */
	{ MODKEY,		XK_space,		spawn,		{.v = dmenucmd } },
	{ MODKEY,		XK_Return,		spawn,		{.v = termcmd } },

	{ MODKEY,		XK_b,			togglebar,	{0} },

	{ MODKEY,		XK_j,			focusstack,	{.i = +1 } },
	{ MODKEY,		XK_k,			focusstack,	{.i = -1 } },

	{ MODKEY|ControlMask,	XK_bracketright,	incnmaster,	{.i = +1 } },
	{ MODKEY|ControlMask,	XK_bracketleft,	 	incnmaster,	{.i = -1 } },

	{ MODKEY,		XK_h,			setmfact,	{.f = -0.05} },
	{ MODKEY,		XK_l,			setmfact,	{.f = +0.05} },

	{ MODKEY|ShiftMask,	XK_q,			killclient,	{0} },

	{ MODKEY,		XK_bracketleft,		setlayout,	{.v = &layouts[0]} },
	{ MODKEY,		XK_bracketright,	setlayout,	{.v = &layouts[1]} },
	{ MODKEY,		XK_backslash,		setlayout,	{.v = &layouts[2]} },
	{ MODKEY|ControlMask,	XK_space, 		setlayout, 	{0} },

	{ MODKEY,		XK_s, 			togglefloating,	{0} },
	{ MODKEY,		XK_f, 			togglefullscr, 	{0} },
	{ MODKEY,		XK_x, 			togglesticky,	{0} },

	{ MODKEY|ShiftMask,	XK_j, 			pushdown,	{0} },
	{ MODKEY|ShiftMask,	XK_k, 			pushup,		{0} },
	{ MODKEY|ShiftMask,	XK_h, 			view_adjacent, 	{.i = -1 } },
	{ MODKEY|ShiftMask,	XK_l, 			view_adjacent, 	{.i = +1 } },

	{ MODKEY,		XK_Down, 		moveresize,	{.v = "0x 25y 0w 0h" } },
	{ MODKEY,		XK_Up,			moveresize,	{.v = "0x -25y 0w 0h" } },
	{ MODKEY,		XK_Right, 		moveresize,	{.v = "25x 0y 0w 0h" } },
	{ MODKEY,		XK_Left,		moveresize,	{.v = "-25x 0y 0w 0h" } },

	{ MODKEY|ShiftMask,	XK_Down,		moveresize,	{.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,	XK_Up,			moveresize,	{.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,	XK_Right, 		moveresize,	{.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,	XK_Left,		moveresize,	{.v = "0x 0y -25w 0h" } },

	{ MODKEY|ControlMask,	XK_Up,			moveresizeedge,	{.v = "t"} },
	{ MODKEY|ControlMask,	XK_Down,		moveresizeedge,	{.v = "b"} },
	{ MODKEY|ControlMask,	XK_Left,		moveresizeedge,	{.v = "l"} },
	{ MODKEY|ControlMask,	XK_Right, 		moveresizeedge,	{.v = "r"} },

	{ MODKEY|ControlMask|ShiftMask,	XK_Up,		moveresizeedge,	{.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask,	XK_Down,	moveresizeedge,	{.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask,	XK_Left,	moveresizeedge,	{.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask,	XK_Right, 	moveresizeedge,	{.v = "R"} },

	{ HYPER, 		XK_d, 			moveresize,	{.v = "0x 25y 0w 0h" } },
	{ HYPER, 		XK_s, 			moveresize,	{.v = "0x -25y 0w 0h" } },
	{ HYPER, 		XK_f, 			moveresize,	{.v = "25x 0y 0w 0h" } },
	{ HYPER, 		XK_a, 			moveresize,	{.v = "-25x 0y 0w 0h" } },

	{ HYPER|ShiftMask, 	XK_d, 			moveresize,	{.v = "0x 0y 0w 25h" } },
	{ HYPER|ShiftMask, 	XK_s, 			moveresize,	{.v = "0x 0y 0w -25h" } },
	{ HYPER|ShiftMask, 	XK_f, 			moveresize,	{.v = "0x 0y 25w 0h" } },
	{ HYPER|ShiftMask, 	XK_a, 			moveresize,	{.v = "0x 0y -25w 0h" } },

	{ HYPER|ControlMask, 	XK_d, 			moveresizeedge,	{.v = "b"} },
	{ HYPER|ControlMask, 	XK_s, 			moveresizeedge,	{.v = "t"} },
	{ HYPER|ControlMask, 	XK_f, 			moveresizeedge,	{.v = "r"} },
	{ HYPER|ControlMask, 	XK_a, 			moveresizeedge,	{.v = "l"} },

	{ HYPER|ControlMask|ShiftMask, 	XK_d, 		moveresizeedge,	{.v = "B"} },
	{ HYPER|ControlMask|ShiftMask, 	XK_s, 		moveresizeedge,	{.v = "T"} },
	{ HYPER|ControlMask|ShiftMask, 	XK_f, 		moveresizeedge,	{.v = "R"} },
	{ HYPER|ControlMask|ShiftMask, 	XK_a, 		moveresizeedge,	{.v = "L"} },

	{ MODKEY,		XK_n, 			zoom,		{0} },
	{ MODKEY,		XK_Tab, 		view,		{0} },

	{ MODKEY,		XK_0, 			view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,	XK_0, 			tag, 		{.ui = ~0 } },

	{ MODKEY|ControlMask,	XK_j, 			focusmon,	{.i = -1 } },
	{ MODKEY|ControlMask,	XK_k, 			focusmon,	{.i = +1 } },

	{ MODKEY|ControlMask|ShiftMask,	XK_j, 		tagmon,	 	{.i = -1 } },
	{ MODKEY|ControlMask|ShiftMask,	XK_k, 		tagmon,	 	{.i = +1 } },

	TAGKEYS( 		XK_1, 					0 )
	TAGKEYS( 		XK_2, 					1 )
	TAGKEYS( 		XK_3, 					2 )
	TAGKEYS( 		XK_4, 					3 )
	TAGKEYS( 		XK_5, 					4 )
	TAGKEYS( 		XK_6, 					5 )
	TAGKEYS( 		XK_7, 					6 )
	TAGKEYS( 		XK_8, 					7 )
	TAGKEYS( 		XK_9, 					8 )

	{ MODKEY,	 	XK_F9, 			togglescratch, 	{.ui = 0 } },
	{ MODKEY,	 	XK_F10, 		togglescratch, 	{.ui = 1 } },
	{ MODKEY,	 	XK_F11, 		togglescratch, 	{.ui = 2 } },
	{ MODKEY,	 	XK_F12, 		togglescratch, 	{.ui = 3 } },

	{ MODKEY|Mod1Mask|ControlMask, 	XK_Escape,	quit,		{0} },	// quit WM
	{ MODKEY|Mod1Mask, 	XK_r, 			quit,		{1} },	// reload WM

	{ MODKEY|ShiftMask,	XK_o, 		 	spawn, 	 	SHCMD("$BROWSER") },
	{ MODKEY|Mod1Mask, 	XK_Escape,	 	spawn, 	 	SHCMD(powermenu) },
	{ MODKEY,		XK_Print, 	 	spawn, 	 	SHCMD(screenshot) },

	{ HYPER, 		XK_F1,			spawn, 		SHCMD(cmus_decvol) },
	{ HYPER, 		XK_F2,			spawn, 		SHCMD(cmus_incvol) },
	{ HYPER, 		XK_F3,			spawn, 		SHCMD(cmus_pause) },
	{ HYPER, 		XK_F4,			spawn, 		SHCMD(cmus_stop) },
	{ HYPER, 		XK_F5,			spawn, 		SHCMD(cmus_prev) },
	{ HYPER, 		XK_F6,			spawn, 		SHCMD(cmus_next) },
	{ HYPER, 		XK_F7,			spawn, 		SHCMD(cmus_repeat) },
	{ HYPER, 		XK_F8,			spawn, 		SHCMD(cmus_shuffle) },

	{ MODKEY, 		XK_F1,			spawn, 		SHCMD(vol_dec) },
	{ MODKEY,		XK_F2,			spawn, 		SHCMD(vol_inc) },
	{ MODKEY,		XK_F3,			spawn, 		SHCMD(vol_mute) },
	{ MODKEY|ShiftMask,	XK_F1, 			spawn, 		SHCMD(mic_decvol) },
	{ MODKEY|ShiftMask,	XK_F2,			spawn, 		SHCMD(mic_incvol) },
	{ MODKEY|ShiftMask,	XK_F3,			spawn, 		SHCMD(mic_mute) },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click event 		mask 			button 		function 		argument */
	{ ClkLtSymbol,		0,			Button1,	setlayout,		{0} },
	{ ClkLtSymbol,		0,			Button3,	setlayout,		{.v = &layouts[2]} },
	{ ClkWinTitle,		0,			Button2,	zoom,			{0} },
	{ ClkStatusText,	0,			Button2,	spawn,			{.v = termcmd } },
	{ ClkClientWin,		MODKEY,			Button1,	movemouse,		{0} },
	{ ClkClientWin,		MODKEY,			Button2,	togglefloating,		{0} },
	{ ClkClientWin,		MODKEY|ShiftMask,	Button1,	resizemouse,		{0} },
	{ ClkTagBar,		0,			Button1,	view,			{0} },
	{ ClkTagBar,		0,			Button3,	toggleview,		{0} },
	{ ClkTagBar,		MODKEY,			Button1,	tag,			{0} },
	{ ClkTagBar,		MODKEY,			Button3,	toggletag,		{0} },
};
