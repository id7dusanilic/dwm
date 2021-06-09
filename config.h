/* See LICENSE file for copyright and license details. */

/* includes */
#include <X11/XF86keysym.h> /* For audio and background settings */

/* constants */
#define TERMINAL    "${TERM_EMULATOR}"          /* These environment variables need to be defined. */
#define BROWSER     "${BROWSER}"                /* Otherwise, hardcode preffered applications. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int fixedtags = 5;        /* number of tags to show even if vacant */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "Mononoki Nerd Font:size=13" };
static const char dmenufont[]       = "Mononoki Nerd Font:size=13";

/* colors */
static const char norm_fg_col[]     = "#eeeeee";
static const char norm_bg_col[]     = "#282828";
static const char norm_border_col[] = "#282828";
static const char sel_fg_col[]      = "#eeeeee";
static const char sel_bg_col[]      = "#7c7c7c";
static const char sel_border_col[]  = "#550000";

enum { SchemeNorm, SchemeSel }; /* color schemes */

static const char *colors[][3]      = {
	/*                fg            bg              border */
	[SchemeNorm]  = { norm_fg_col,  norm_bg_col,    norm_border_col },
	[SchemeSel]   = { sel_fg_col,   sel_bg_col,     sel_border_col },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
// static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
// static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class        instance             title  tags mask  isfloating  monitor */
	{ "Gimp",       NULL,                NULL,  0,         1,          -1 },
	{ "Firefox",    NULL,                NULL,  1 << 8,    0,          -1 },
	{ NULL,         "FloatingTerminal",  NULL,  0,         1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol	arrange function */
	{ "ﰧ",       tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
	{ "ﰦ",      bstack},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, NULL };

static Key keys[] = {
	/* modifier            key                        function        argument */

	/* Spawning */
	{ MODKEY,              XK_p,                      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,    XK_Return,                 spawn,          SHCMD(TERMINAL) },

	/* Quiting dwm */
	{ MODKEY,              XK_Escape,                 quit,           {0} },
	{ MODKEY|ShiftMask,    XK_Escape,                 quit,           {1} },

	/* Layout */
	{ MODKEY,              XK_t,                      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,              XK_f,                      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,              XK_m,                      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,              XK_u,                      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,              XK_0,                      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,                      tag,            {.ui = ~0 } },
	{ MODKEY,              XK_j,                      focusstack,     {.i = +1 } },
	{ MODKEY,              XK_k,                      focusstack,     {.i = -1 } },
	{ MODKEY,              XK_i,                      incnmaster,     {.i = +1 } },
	{ MODKEY,              XK_d,                      incnmaster,     {.i = -1 } },
	{ MODKEY,              XK_h,                      setmfact,       {.f = -0.05} },
	{ MODKEY,              XK_l,                      setmfact,       {.f = +0.05} },
	{ MODKEY,              XK_Return,                 zoom,           {0} },
	{ MODKEY,              XK_Tab,                    view,           {0} },
	{ MODKEY,              XK_q,                      killclient,     {0} },
	{ MODKEY|ShiftMask,    XK_space,                  togglefloating, {0} },
	{ MODKEY|ShiftMask,    XK_s,                      togglesticky,   {0} },
	{ MODKEY,              XK_b,                      togglebar,      {0} },

	/* Focus a different monitor */
	{ MODKEY,              XK_comma,                  focusmon,       {.i = -1 } },
	{ MODKEY,              XK_period,                 focusmon,       {.i = +1 } },

	/* Move client to a different monitor */
	{ MODKEY|ShiftMask,    XK_comma,                  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_period,                 tagmon,         {.i = +1 } },

	/* Moving and resizing floating windows */
	{ MODKEY,              XK_Down,                   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,              XK_Up,                     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,              XK_Right,                  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,              XK_Left,                   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,    XK_Down,                   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,    XK_Up,                     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,    XK_Right,                  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,    XK_Left,                   moveresize,     {.v = "0x 0y -25w 0h" } },

	/* Tags */
	TAGKEYS(               XK_1,                                      0)
	TAGKEYS(               XK_2,                                      1)
	TAGKEYS(               XK_3,                                      2)
	TAGKEYS(               XK_4,                                      3)
	TAGKEYS(               XK_5,                                      4)
	TAGKEYS(               XK_6,                                      5)
	TAGKEYS(               XK_7,                                      6)
	TAGKEYS(               XK_8,                                      7)
	TAGKEYS(               XK_9,                                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
	{ ClkStatusText,        ShiftMask,      Button3,        sigdwmblocks,   {.i = 7} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

