#pragma once

using Scancode = enum Scancode
{
	PAPI_KEY_UNKNOWN = 0,

	/**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
	/* @{ */

	PAPI_KEY_A = 4,
	PAPI_KEY_B = 5,
	PAPI_KEY_C = 6,
	PAPI_KEY_D = 7,
	PAPI_KEY_E = 8,
	PAPI_KEY_F = 9,
	PAPI_KEY_G = 10,
	PAPI_KEY_H = 11,
	PAPI_KEY_I = 12,
	PAPI_KEY_J = 13,
	PAPI_KEY_K = 14,
	PAPI_KEY_L = 15,
	PAPI_KEY_M = 16,
	PAPI_KEY_N = 17,
	PAPI_KEY_O = 18,
	PAPI_KEY_P = 19,
	PAPI_KEY_Q = 20,
	PAPI_KEY_R = 21,
	PAPI_KEY_S = 22,
	PAPI_KEY_T = 23,
	PAPI_KEY_U = 24,
	PAPI_KEY_V = 25,
	PAPI_KEY_W = 26,
	PAPI_KEY_X = 27,
	PAPI_KEY_Y = 28,
	PAPI_KEY_Z = 29,

	PAPI_KEY_1 = 30,
	PAPI_KEY_2 = 31,
	PAPI_KEY_3 = 32,
	PAPI_KEY_4 = 33,
	PAPI_KEY_5 = 34,
	PAPI_KEY_6 = 35,
	PAPI_KEY_7 = 36,
	PAPI_KEY_8 = 37,
	PAPI_KEY_9 = 38,
	PAPI_KEY_0 = 39,

	PAPI_KEY_RETURN    = 40,
	PAPI_KEY_ESCAPE    = 41,
	PAPI_KEY_BACKSPACE = 42,
	PAPI_KEY_TAB       = 43,
	PAPI_KEY_SPACE     = 44,

	PAPI_KEY_MINUS        = 45,
	PAPI_KEY_EQUALS       = 46,
	PAPI_KEY_LEFTBRACKET  = 47,
	PAPI_KEY_RIGHTBRACKET = 48,
	PAPI_KEY_BACKSLASH    = 49, /**< Located at the lower left of the return
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
	PAPI_KEY_NONUSHASH    = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate PAPI_KEY_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
	PAPI_KEY_SEMICOLON    = 51,
	PAPI_KEY_APOSTROPHE   = 52,
	PAPI_KEY_GRAVE        = 53, /**< Located in the top left corner (on both ANSI
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
	PAPI_KEY_COMMA        = 54,
	PAPI_KEY_PERIOD       = 55,
	PAPI_KEY_SLASH        = 56,

	PAPI_KEY_CAPSLOCK = 57,

	PAPI_KEY_F1  = 58,
	PAPI_KEY_F2  = 59,
	PAPI_KEY_F3  = 60,
	PAPI_KEY_F4  = 61,
	PAPI_KEY_F5  = 62,
	PAPI_KEY_F6  = 63,
	PAPI_KEY_F7  = 64,
	PAPI_KEY_F8  = 65,
	PAPI_KEY_F9  = 66,
	PAPI_KEY_F10 = 67,
	PAPI_KEY_F11 = 68,
	PAPI_KEY_F12 = 69,

	PAPI_KEY_PRINTSCREEN = 70,
	PAPI_KEY_SCROLLLOCK  = 71,
	PAPI_KEY_PAUSE       = 72,
	PAPI_KEY_INSERT      = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
	PAPI_KEY_HOME        = 74,
	PAPI_KEY_PAGEUP      = 75,
	PAPI_KEY_DELETE      = 76,
	PAPI_KEY_END         = 77,
	PAPI_KEY_PAGEDOWN    = 78,
	PAPI_KEY_RIGHT       = 79,
	PAPI_KEY_LEFT        = 80,
	PAPI_KEY_DOWN        = 81,
	PAPI_KEY_UP          = 82,

	PAPI_KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
	PAPI_KEY_KP_DIVIDE    = 84,
	PAPI_KEY_KP_MULTIPLY  = 85,
	PAPI_KEY_KP_MINUS     = 86,
	PAPI_KEY_KP_PLUS      = 87,
	PAPI_KEY_KP_ENTER     = 88,
	PAPI_KEY_KP_1         = 89,
	PAPI_KEY_KP_2         = 90,
	PAPI_KEY_KP_3         = 91,
	PAPI_KEY_KP_4         = 92,
	PAPI_KEY_KP_5         = 93,
	PAPI_KEY_KP_6         = 94,
	PAPI_KEY_KP_7         = 95,
	PAPI_KEY_KP_8         = 96,
	PAPI_KEY_KP_9         = 97,
	PAPI_KEY_KP_0         = 98,
	PAPI_KEY_KP_PERIOD    = 99,

	PAPI_KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
	PAPI_KEY_APPLICATION    = 101, /**< windows contextual menu, compose */
	PAPI_KEY_POWER          = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
	PAPI_KEY_KP_EQUALS      = 103,
	PAPI_KEY_F13            = 104,
	PAPI_KEY_F14            = 105,
	PAPI_KEY_F15            = 106,
	PAPI_KEY_F16            = 107,
	PAPI_KEY_F17            = 108,
	PAPI_KEY_F18            = 109,
	PAPI_KEY_F19            = 110,
	PAPI_KEY_F20            = 111,
	PAPI_KEY_F21            = 112,
	PAPI_KEY_F22            = 113,
	PAPI_KEY_F23            = 114,
	PAPI_KEY_F24            = 115,
	PAPI_KEY_EXECUTE        = 116,
	PAPI_KEY_HELP           = 117, /**< AL Integrated Help Center */
	PAPI_KEY_MENU           = 118, /**< Menu (show menu) */
	PAPI_KEY_SELECT         = 119,
	PAPI_KEY_STOP           = 120, /**< AC Stop */
	PAPI_KEY_AGAIN          = 121, /**< AC Redo/Repeat */
	PAPI_KEY_UNDO           = 122, /**< AC Undo */
	PAPI_KEY_CUT            = 123, /**< AC Cut */
	PAPI_KEY_COPY           = 124, /**< AC Copy */
	PAPI_KEY_PASTE          = 125, /**< AC Paste */
	PAPI_KEY_FIND           = 126, /**< AC Find */
	PAPI_KEY_MUTE           = 127,
	PAPI_KEY_VOLUMEUP       = 128,
	PAPI_KEY_VOLUMEDOWN     = 129,
	/* not sure whether there's a reason to enable these */
	/*     PAPI_KEY_LOCKINGCAPSLOCK = 130,  */
	/*     PAPI_KEY_LOCKINGNUMLOCK = 131, */
	/*     PAPI_KEY_LOCKINGSCROLLLOCK = 132, */
	PAPI_KEY_KP_COMMA       = 133,
	PAPI_KEY_KP_EQUALSAS400 = 134,

	PAPI_KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
	PAPI_KEY_INTERNATIONAL2 = 136,
	PAPI_KEY_INTERNATIONAL3 = 137, /**< Yen */
	PAPI_KEY_INTERNATIONAL4 = 138,
	PAPI_KEY_INTERNATIONAL5 = 139,
	PAPI_KEY_INTERNATIONAL6 = 140,
	PAPI_KEY_INTERNATIONAL7 = 141,
	PAPI_KEY_INTERNATIONAL8 = 142,
	PAPI_KEY_INTERNATIONAL9 = 143,
	PAPI_KEY_LANG1          = 144, /**< Hangul/English toggle */
	PAPI_KEY_LANG2          = 145, /**< Hanja conversion */
	PAPI_KEY_LANG3          = 146, /**< Katakana */
	PAPI_KEY_LANG4          = 147, /**< Hiragana */
	PAPI_KEY_LANG5          = 148, /**< Zenkaku/Hankaku */
	PAPI_KEY_LANG6          = 149, /**< reserved */
	PAPI_KEY_LANG7          = 150, /**< reserved */
	PAPI_KEY_LANG8          = 151, /**< reserved */
	PAPI_KEY_LANG9          = 152, /**< reserved */

	PAPI_KEY_ALTERASE   = 153, /**< Erase-Eaze */
	PAPI_KEY_SYSREQ     = 154,
	PAPI_KEY_CANCEL     = 155, /**< AC Cancel */
	PAPI_KEY_CLEAR      = 156,
	PAPI_KEY_PRIOR      = 157,
	PAPI_KEY_RETURN2    = 158,
	PAPI_KEY_SEPARATOR  = 159,
	PAPI_KEY_OUT        = 160,
	PAPI_KEY_OPER       = 161,
	PAPI_KEY_CLEARAGAIN = 162,
	PAPI_KEY_CRSEL      = 163,
	PAPI_KEY_EXSEL      = 164,

	PAPI_KEY_KP_00              = 176,
	PAPI_KEY_KP_000             = 177,
	PAPI_KEY_THOUSANDSSEPARATOR = 178,
	PAPI_KEY_DECIMALSEPARATOR   = 179,
	PAPI_KEY_CURRENCYUNIT       = 180,
	PAPI_KEY_CURRENCYSUBUNIT    = 181,
	PAPI_KEY_KP_LEFTPAREN       = 182,
	PAPI_KEY_KP_RIGHTPAREN      = 183,
	PAPI_KEY_KP_LEFTBRACE       = 184,
	PAPI_KEY_KP_RIGHTBRACE      = 185,
	PAPI_KEY_KP_TAB             = 186,
	PAPI_KEY_KP_BACKSPACE       = 187,
	PAPI_KEY_KP_A               = 188,
	PAPI_KEY_KP_B               = 189,
	PAPI_KEY_KP_C               = 190,
	PAPI_KEY_KP_D               = 191,
	PAPI_KEY_KP_E               = 192,
	PAPI_KEY_KP_F               = 193,
	PAPI_KEY_KP_XOR             = 194,
	PAPI_KEY_KP_POWER           = 195,
	PAPI_KEY_KP_PERCENT         = 196,
	PAPI_KEY_KP_LESS            = 197,
	PAPI_KEY_KP_GREATER         = 198,
	PAPI_KEY_KP_AMPERSAND       = 199,
	PAPI_KEY_KP_DBLAMPERSAND    = 200,
	PAPI_KEY_KP_VERTICALBAR     = 201,
	PAPI_KEY_KP_DBLVERTICALBAR  = 202,
	PAPI_KEY_KP_COLON           = 203,
	PAPI_KEY_KP_HASH            = 204,
	PAPI_KEY_KP_SPACE           = 205,
	PAPI_KEY_KP_AT              = 206,
	PAPI_KEY_KP_EXCLAM          = 207,
	PAPI_KEY_KP_MEMSTORE        = 208,
	PAPI_KEY_KP_MEMRECALL       = 209,
	PAPI_KEY_KP_MEMCLEAR        = 210,
	PAPI_KEY_KP_MEMADD          = 211,
	PAPI_KEY_KP_MEMSUBTRACT     = 212,
	PAPI_KEY_KP_MEMMULTIPLY     = 213,
	PAPI_KEY_KP_MEMDIVIDE       = 214,
	PAPI_KEY_KP_PLUSMINUS       = 215,
	PAPI_KEY_KP_CLEAR           = 216,
	PAPI_KEY_KP_CLEARENTRY      = 217,
	PAPI_KEY_KP_BINARY          = 218,
	PAPI_KEY_KP_OCTAL           = 219,
	PAPI_KEY_KP_DECIMAL         = 220,
	PAPI_KEY_KP_HEXADECIMAL     = 221,

	PAPI_KEY_LCTRL  = 224,
	PAPI_KEY_LSHIFT = 225,
	PAPI_KEY_LALT   = 226, /**< alt, option */
	PAPI_KEY_LGUI   = 227, /**< windows, command (apple), meta */
	PAPI_KEY_RCTRL  = 228,
	PAPI_KEY_RSHIFT = 229,
	PAPI_KEY_RALT   = 230, /**< alt gr, option */
	PAPI_KEY_RGUI   = 231, /**< windows, command (apple), meta */

	PAPI_KEY_MODE = 257, /**< I'm not sure if this is really not covered
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

	PAPI_KEY_SLEEP = 258, /**< Sleep */
	PAPI_KEY_WAKE  = 259, /**< Wake */

	PAPI_KEY_CHANNEL_INCREMENT = 260, /**< Channel Increment */
	PAPI_KEY_CHANNEL_DECREMENT = 261, /**< Channel Decrement */

	PAPI_KEY_MEDIA_PLAY           = 262, /**< Play */
	PAPI_KEY_MEDIA_PAUSE          = 263, /**< Pause */
	PAPI_KEY_MEDIA_RECORD         = 264, /**< Record */
	PAPI_KEY_MEDIA_FAST_FORWARD   = 265, /**< Fast Forward */
	PAPI_KEY_MEDIA_REWIND         = 266, /**< Rewind */
	PAPI_KEY_MEDIA_NEXT_TRACK     = 267, /**< Next Track */
	PAPI_KEY_MEDIA_PREVIOUS_TRACK = 268, /**< Previous Track */
	PAPI_KEY_MEDIA_STOP           = 269, /**< Stop */
	PAPI_KEY_MEDIA_EJECT          = 270, /**< Eject */
	PAPI_KEY_MEDIA_PLAY_PAUSE     = 271, /**< Play / Pause */
	PAPI_KEY_MEDIA_SELECT         = 272, /* Media Select */

	PAPI_KEY_AC_NEW        = 273, /**< AC New */
	PAPI_KEY_AC_OPEN       = 274, /**< AC Open */
	PAPI_KEY_AC_CLOSE      = 275, /**< AC Close */
	PAPI_KEY_AC_EXIT       = 276, /**< AC Exit */
	PAPI_KEY_AC_SAVE       = 277, /**< AC Save */
	PAPI_KEY_AC_PRINT      = 278, /**< AC Print */
	PAPI_KEY_AC_PROPERTIES = 279, /**< AC Properties */

	PAPI_KEY_AC_SEARCH    = 280, /**< AC Search */
	PAPI_KEY_AC_HOME      = 281, /**< AC Home */
	PAPI_KEY_AC_BACK      = 282, /**< AC Back */
	PAPI_KEY_AC_FORWARD   = 283, /**< AC Forward */
	PAPI_KEY_AC_STOP      = 284, /**< AC Stop */
	PAPI_KEY_AC_REFRESH   = 285, /**< AC Refresh */
	PAPI_KEY_AC_BOOKMARKS = 286, /**< AC Bookmarks */

	/* @} */ /* Usage page 0x0C */


	/**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
	/* @{ */

	PAPI_KEY_SOFTLEFT  = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
	PAPI_KEY_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
	PAPI_KEY_CALL      = 289, /**< Used for accepting phone calls. */
	PAPI_KEY_ENDCALL   = 290, /**< Used for rejecting phone calls. */

	/* @} */ /* Mobile keys */

	/* Add any other keys here. */

	PAPI_KEY_RESERVED = 400, /**< 400-500 reserved for dynamic keycodes */

	PAPI_KEY_COUNT = 512 /**< not a key, just marks the number of scancodes for array bounds */
};

NODISCARD const char* ScancodeToString(Scancode scancode);
