/* See LICENSE file for copyright and license details. */

#include <time.h>
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Ubuntu Mono Nerd Font:size=9" };
static const char dmenufont[]       = "Ubuntu Mono Nerd Font:size=9";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_main[]        = "#540303";
static const char col_bg[]          = "#1A0303";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_main,  col_main  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Java",     "java",     "Eclipse",  0,            1,           -1 }, // eclipse splash screen
};

/* layout(s) */
static const float mfact     = 0.65; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */

void
spawncmd(const char *cmd[])
{
	const Arg arg = {.v = cmd };
	spawn(&arg);
}

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]  = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_main, "-sf", col_gray4, NULL };
static const char *termcmd[]   = { "urxvt", NULL };
static const char *qpdfcmd[]   = { "qpdfview", NULL };
static const char *chromecmd[] = { "google-chrome-stable", NULL };

static const char *screenshotroot[] = { "bash", "-c", "mkdir -p /tmp/screenshot && scrot /tmp/screenshot/%Y-%m-%d-%H-%M-%S.png", NULL };
static const char *screenshotselection[] = { "bash", "-c", "sleep 0.2 && mkdir -p /tmp/screenshot && scrot /tmp/screenshot/%Y-%m-%d-%H-%M-%S.png --select", NULL };

static const char *upvol[]   = { "amixer", "set", "Master", "1%+", NULL };
static const char *downvol[] = { "amixer", "set", "Master", "1%-", NULL };
static const char *mute[]    = { "amixer", "set", "Master", "toggle", NULL };

static const char *uplight[]   = { "light", "-A", "5", NULL };
static const char *downlight[] = { "light", "-U", "5", NULL };

static const char *lockscreen[] = { "lock-screen", NULL };

static const char *playerplay[] = { "playerctl", "play-pause", NULL };
static const char *playernext[] = { "playerctl", "next", NULL };
static const char *playerprev[] = { "playerctl", "previous", NULL };
static const char *playerstop[] = { "playerctl", "stop", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ALT,                   XK_p,      spawn,          {.v = qpdfcmd } },
	{ MODKEY|ALT,                   XK_g,      spawn,          {.v = chromecmd } },
	{ MODKEY|ControlMask,           XK_s,      spawn,          {.v = screenshotroot} },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = screenshotselection} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask,           XK_l,      spawn,          {.v = lockscreen } },
	{ 0,            XF86XK_AudioRaiseVolume,   spawn,          {.v = upvol } },
	{ 0,            XF86XK_AudioLowerVolume,   spawn,          {.v = downvol } },
	{ 0,            XF86XK_AudioMute,          spawn,          {.v = mute } },
	{ 0,            XF86XK_MonBrightnessUp,    spawn,          {.v = uplight } },
	{ 0,            XF86XK_MonBrightnessDown,  spawn,          {.v = downlight } },
	{ 0,            XF86XK_AudioPlay,          spawn,          {.v = playerplay } },
	{ 0,            XF86XK_AudioNext,          spawn,          {.v = playernext } },
	{ 0,            XF86XK_AudioPrev,          spawn,          {.v = playerprev } },
	{ 0,            XF86XK_AudioStop,          spawn,          {.v = playerstop } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           XK_Left,   shiftview,      {.i = -1 } },
	{ MODKEY|ControlMask,           XK_Right,  shiftview,      {.i = +1 } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

// vim: set noexpandtab :
