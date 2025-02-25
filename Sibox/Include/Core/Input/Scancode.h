#pragma once

using Scancode = enum Scancode
{
	SIBOX_KEY_UNKNOWN = 0,

	/**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
	/* @{ */

	SIBOX_KEY_A = 4,
	SIBOX_KEY_B = 5,
	SIBOX_KEY_C = 6,
	SIBOX_KEY_D = 7,
	SIBOX_KEY_E = 8,
	SIBOX_KEY_F = 9,
	SIBOX_KEY_G = 10,
	SIBOX_KEY_H = 11,
	SIBOX_KEY_I = 12,
	SIBOX_KEY_J = 13,
	SIBOX_KEY_K = 14,
	SIBOX_KEY_L = 15,
	SIBOX_KEY_M = 16,
	SIBOX_KEY_N = 17,
	SIBOX_KEY_O = 18,
	SIBOX_KEY_P = 19,
	SIBOX_KEY_Q = 20,
	SIBOX_KEY_R = 21,
	SIBOX_KEY_S = 22,
	SIBOX_KEY_T = 23,
	SIBOX_KEY_U = 24,
	SIBOX_KEY_V = 25,
	SIBOX_KEY_W = 26,
	SIBOX_KEY_X = 27,
	SIBOX_KEY_Y = 28,
	SIBOX_KEY_Z = 29,

	SIBOX_KEY_1 = 30,
	SIBOX_KEY_2 = 31,
	SIBOX_KEY_3 = 32,
	SIBOX_KEY_4 = 33,
	SIBOX_KEY_5 = 34,
	SIBOX_KEY_6 = 35,
	SIBOX_KEY_7 = 36,
	SIBOX_KEY_8 = 37,
	SIBOX_KEY_9 = 38,
	SIBOX_KEY_0 = 39,

	SIBOX_KEY_RETURN    = 40,
	SIBOX_KEY_ESCAPE    = 41,
	SIBOX_KEY_BACKSPACE = 42,
	SIBOX_KEY_TAB       = 43,
	SIBOX_KEY_SPACE     = 44,

	SIBOX_KEY_MINUS        = 45,
	SIBOX_KEY_EQUALS       = 46,
	SIBOX_KEY_LEFTBRACKET  = 47,
	SIBOX_KEY_RIGHTBRACKET = 48,
	SIBOX_KEY_BACKSLASH    = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
	SIBOX_KEY_NONUSHASH    = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate SIBOX_KEY_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
	SIBOX_KEY_SEMICOLON    = 51,
	SIBOX_KEY_APOSTROPHE   = 52,
	SIBOX_KEY_GRAVE        = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
	SIBOX_KEY_COMMA        = 54,
	SIBOX_KEY_PERIOD       = 55,
	SIBOX_KEY_SLASH        = 56,

	SIBOX_KEY_CAPSLOCK = 57,

	SIBOX_KEY_F1  = 58,
	SIBOX_KEY_F2  = 59,
	SIBOX_KEY_F3  = 60,
	SIBOX_KEY_F4  = 61,
	SIBOX_KEY_F5  = 62,
	SIBOX_KEY_F6  = 63,
	SIBOX_KEY_F7  = 64,
	SIBOX_KEY_F8  = 65,
	SIBOX_KEY_F9  = 66,
	SIBOX_KEY_F10 = 67,
	SIBOX_KEY_F11 = 68,
	SIBOX_KEY_F12 = 69,

	SIBOX_KEY_PRINTSCREEN = 70,
	SIBOX_KEY_SCROLLLOCK  = 71,
	SIBOX_KEY_PAUSE       = 72,
	SIBOX_KEY_INSERT      = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
	SIBOX_KEY_HOME        = 74,
	SIBOX_KEY_PAGEUP      = 75,
	SIBOX_KEY_DELETE      = 76,
	SIBOX_KEY_END         = 77,
	SIBOX_KEY_PAGEDOWN    = 78,
	SIBOX_KEY_RIGHT       = 79,
	SIBOX_KEY_LEFT        = 80,
	SIBOX_KEY_DOWN        = 81,
	SIBOX_KEY_UP          = 82,

	SIBOX_KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
	SIBOX_KEY_KP_DIVIDE    = 84,
	SIBOX_KEY_KP_MULTIPLY  = 85,
	SIBOX_KEY_KP_MINUS     = 86,
	SIBOX_KEY_KP_PLUS      = 87,
	SIBOX_KEY_KP_ENTER     = 88,
	SIBOX_KEY_KP_1         = 89,
	SIBOX_KEY_KP_2         = 90,
	SIBOX_KEY_KP_3         = 91,
	SIBOX_KEY_KP_4         = 92,
	SIBOX_KEY_KP_5         = 93,
	SIBOX_KEY_KP_6         = 94,
	SIBOX_KEY_KP_7         = 95,
	SIBOX_KEY_KP_8         = 96,
	SIBOX_KEY_KP_9         = 97,
	SIBOX_KEY_KP_0         = 98,
	SIBOX_KEY_KP_PERIOD    = 99,

	SIBOX_KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
	SIBOX_KEY_APPLICATION    = 101, /**< windows contextual menu, compose */
	SIBOX_KEY_POWER          = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
	SIBOX_KEY_KP_EQUALS      = 103,
	SIBOX_KEY_F13            = 104,
	SIBOX_KEY_F14            = 105,
	SIBOX_KEY_F15            = 106,
	SIBOX_KEY_F16            = 107,
	SIBOX_KEY_F17            = 108,
	SIBOX_KEY_F18            = 109,
	SIBOX_KEY_F19            = 110,
	SIBOX_KEY_F20            = 111,
	SIBOX_KEY_F21            = 112,
	SIBOX_KEY_F22            = 113,
	SIBOX_KEY_F23            = 114,
	SIBOX_KEY_F24            = 115,
	SIBOX_KEY_EXECUTE        = 116,
	SIBOX_KEY_HELP           = 117, /**< AL Integrated Help Center */
	SIBOX_KEY_MENU           = 118, /**< Menu (show menu) */
	SIBOX_KEY_SELECT         = 119,
	SIBOX_KEY_STOP           = 120, /**< AC Stop */
	SIBOX_KEY_AGAIN          = 121, /**< AC Redo/Repeat */
	SIBOX_KEY_UNDO           = 122, /**< AC Undo */
	SIBOX_KEY_CUT            = 123, /**< AC Cut */
	SIBOX_KEY_COPY           = 124, /**< AC Copy */
	SIBOX_KEY_PASTE          = 125, /**< AC Paste */
	SIBOX_KEY_FIND           = 126, /**< AC Find */
	SIBOX_KEY_MUTE           = 127,
	SIBOX_KEY_VOLUMEUP       = 128,
	SIBOX_KEY_VOLUMEDOWN     = 129,
	/* not sure whether there's a reason to enable these */
	/*     SIBOX_KEY_LOCKINGCAPSLOCK = 130,  */
	/*     SIBOX_KEY_LOCKINGNUMLOCK = 131, */
	/*     SIBOX_KEY_LOCKINGSCROLLLOCK = 132, */
	SIBOX_KEY_KP_COMMA       = 133,
	SIBOX_KEY_KP_EQUALSAS400 = 134,

	SIBOX_KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
	SIBOX_KEY_INTERNATIONAL2 = 136,
	SIBOX_KEY_INTERNATIONAL3 = 137, /**< Yen */
	SIBOX_KEY_INTERNATIONAL4 = 138,
	SIBOX_KEY_INTERNATIONAL5 = 139,
	SIBOX_KEY_INTERNATIONAL6 = 140,
	SIBOX_KEY_INTERNATIONAL7 = 141,
	SIBOX_KEY_INTERNATIONAL8 = 142,
	SIBOX_KEY_INTERNATIONAL9 = 143,
	SIBOX_KEY_LANG1          = 144, /**< Hangul/English toggle */
	SIBOX_KEY_LANG2          = 145, /**< Hanja conversion */
	SIBOX_KEY_LANG3          = 146, /**< Katakana */
	SIBOX_KEY_LANG4          = 147, /**< Hiragana */
	SIBOX_KEY_LANG5          = 148, /**< Zenkaku/Hankaku */
	SIBOX_KEY_LANG6          = 149, /**< reserved */
	SIBOX_KEY_LANG7          = 150, /**< reserved */
	SIBOX_KEY_LANG8          = 151, /**< reserved */
	SIBOX_KEY_LANG9          = 152, /**< reserved */

	SIBOX_KEY_ALTERASE   = 153, /**< Erase-Eaze */
	SIBOX_KEY_SYSREQ     = 154,
	SIBOX_KEY_CANCEL     = 155, /**< AC Cancel */
	SIBOX_KEY_CLEAR      = 156,
	SIBOX_KEY_PRIOR      = 157,
	SIBOX_KEY_RETURN2    = 158,
	SIBOX_KEY_SEPARATOR  = 159,
	SIBOX_KEY_OUT        = 160,
	SIBOX_KEY_OPER       = 161,
	SIBOX_KEY_CLEARAGAIN = 162,
	SIBOX_KEY_CRSEL      = 163,
	SIBOX_KEY_EXSEL      = 164,

	SIBOX_KEY_KP_00              = 176,
	SIBOX_KEY_KP_000             = 177,
	SIBOX_KEY_THOUSANDSSEPARATOR = 178,
	SIBOX_KEY_DECIMALSEPARATOR   = 179,
	SIBOX_KEY_CURRENCYUNIT       = 180,
	SIBOX_KEY_CURRENCYSUBUNIT    = 181,
	SIBOX_KEY_KP_LEFTPAREN       = 182,
	SIBOX_KEY_KP_RIGHTPAREN      = 183,
	SIBOX_KEY_KP_LEFTBRACE       = 184,
	SIBOX_KEY_KP_RIGHTBRACE      = 185,
	SIBOX_KEY_KP_TAB             = 186,
	SIBOX_KEY_KP_BACKSPACE       = 187,
	SIBOX_KEY_KP_A               = 188,
	SIBOX_KEY_KP_B               = 189,
	SIBOX_KEY_KP_C               = 190,
	SIBOX_KEY_KP_D               = 191,
	SIBOX_KEY_KP_E               = 192,
	SIBOX_KEY_KP_F               = 193,
	SIBOX_KEY_KP_XOR             = 194,
	SIBOX_KEY_KP_POWER           = 195,
	SIBOX_KEY_KP_PERCENT         = 196,
	SIBOX_KEY_KP_LESS            = 197,
	SIBOX_KEY_KP_GREATER         = 198,
	SIBOX_KEY_KP_AMPERSAND       = 199,
	SIBOX_KEY_KP_DBLAMPERSAND    = 200,
	SIBOX_KEY_KP_VERTICALBAR     = 201,
	SIBOX_KEY_KP_DBLVERTICALBAR  = 202,
	SIBOX_KEY_KP_COLON           = 203,
	SIBOX_KEY_KP_HASH            = 204,
	SIBOX_KEY_KP_SPACE           = 205,
	SIBOX_KEY_KP_AT              = 206,
	SIBOX_KEY_KP_EXCLAM          = 207,
	SIBOX_KEY_KP_MEMSTORE        = 208,
	SIBOX_KEY_KP_MEMRECALL       = 209,
	SIBOX_KEY_KP_MEMCLEAR        = 210,
	SIBOX_KEY_KP_MEMADD          = 211,
	SIBOX_KEY_KP_MEMSUBTRACT     = 212,
	SIBOX_KEY_KP_MEMMULTIPLY     = 213,
	SIBOX_KEY_KP_MEMDIVIDE       = 214,
	SIBOX_KEY_KP_PLUSMINUS       = 215,
	SIBOX_KEY_KP_CLEAR           = 216,
	SIBOX_KEY_KP_CLEARENTRY      = 217,
	SIBOX_KEY_KP_BINARY          = 218,
	SIBOX_KEY_KP_OCTAL           = 219,
	SIBOX_KEY_KP_DECIMAL         = 220,
	SIBOX_KEY_KP_HEXADECIMAL     = 221,

	SIBOX_KEY_LCTRL  = 224,
	SIBOX_KEY_LSHIFT = 225,
	SIBOX_KEY_LALT   = 226, /**< alt, option */
	SIBOX_KEY_LGUI   = 227, /**< windows, command (apple), meta */
	SIBOX_KEY_RCTRL  = 228,
	SIBOX_KEY_RSHIFT = 229,
	SIBOX_KEY_RALT   = 230, /**< alt gr, option */
	SIBOX_KEY_RGUI   = 231, /**< windows, command (apple), meta */

	SIBOX_KEY_MODE = 257, /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special SDL_KMOD_MODE for it I'm adding it here
                                 */

	/* @} */ /* Usage page 0x07 */

	/**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
	/* @{ */

	SIBOX_KEY_SLEEP = 258, /**< Sleep */
	SIBOX_KEY_WAKE  = 259, /**< Wake */

	SIBOX_KEY_CHANNEL_INCREMENT = 260, /**< Channel Increment */
	SIBOX_KEY_CHANNEL_DECREMENT = 261, /**< Channel Decrement */

	SIBOX_KEY_MEDIA_PLAY           = 262, /**< Play */
	SIBOX_KEY_MEDIA_PAUSE          = 263, /**< Pause */
	SIBOX_KEY_MEDIA_RECORD         = 264, /**< Record */
	SIBOX_KEY_MEDIA_FAST_FORWARD   = 265, /**< Fast Forward */
	SIBOX_KEY_MEDIA_REWIND         = 266, /**< Rewind */
	SIBOX_KEY_MEDIA_NEXT_TRACK     = 267, /**< Next Track */
	SIBOX_KEY_MEDIA_PREVIOUS_TRACK = 268, /**< Previous Track */
	SIBOX_KEY_MEDIA_STOP           = 269, /**< Stop */
	SIBOX_KEY_MEDIA_EJECT          = 270, /**< Eject */
	SIBOX_KEY_MEDIA_PLAY_PAUSE     = 271, /**< Play / Pause */
	SIBOX_KEY_MEDIA_SELECT         = 272, /* Media Select */

	SIBOX_KEY_AC_NEW        = 273, /**< AC New */
	SIBOX_KEY_AC_OPEN       = 274, /**< AC Open */
	SIBOX_KEY_AC_CLOSE      = 275, /**< AC Close */
	SIBOX_KEY_AC_EXIT       = 276, /**< AC Exit */
	SIBOX_KEY_AC_SAVE       = 277, /**< AC Save */
	SIBOX_KEY_AC_PRINT      = 278, /**< AC Print */
	SIBOX_KEY_AC_PROPERTIES = 279, /**< AC Properties */

	SIBOX_KEY_AC_SEARCH    = 280, /**< AC Search */
	SIBOX_KEY_AC_HOME      = 281, /**< AC Home */
	SIBOX_KEY_AC_BACK      = 282, /**< AC Back */
	SIBOX_KEY_AC_FORWARD   = 283, /**< AC Forward */
	SIBOX_KEY_AC_STOP      = 284, /**< AC Stop */
	SIBOX_KEY_AC_REFRESH   = 285, /**< AC Refresh */
	SIBOX_KEY_AC_BOOKMARKS = 286, /**< AC Bookmarks */

	/* @} */ /* Usage page 0x0C */


	/**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
	/* @{ */

	SIBOX_KEY_SOFTLEFT  = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
	SIBOX_KEY_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
	SIBOX_KEY_CALL      = 289, /**< Used for accepting phone calls. */
	SIBOX_KEY_ENDCALL   = 290, /**< Used for rejecting phone calls. */

	/* @} */ /* Mobile keys */

	/* Add any other keys here. */

	SIBOX_KEY_RESERVED = 400, /**< 400-500 reserved for dynamic keycodes */

	SIBOX_KEY_COUNT = 512 /**< not a key, just marks the number of scancodes for array bounds */
};

NODISCARD const char* ScancodeToString(Scancode scancode);
