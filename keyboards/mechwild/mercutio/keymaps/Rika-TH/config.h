/* Copyright 2021 Kyle McCreery 
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

#pragma once 

/* Define custom font */ // Don't need to use this with me removing all OLED text, could change things, but eh, breaking it anyway
// #define OLED_FONT_H "keyboards/mechwild/mercutio/lib/mercutiofont.c"
/* Changes some settings to hopefully lessen potential OLED burn-in */
#define OLED_TIMEOUT 2800
#define OLED_BRIGHTNESS 95
// Changes font sizes to make putting caps lock etc much easier, does break the font though, but personal usecase is personal
#define OLED_FONT_WIDTH 4 	// default 6
//#define OLED_FONT_HEIGHT 2	// default 8
/* Sets the unicode input mode for square and cubed */
#define UNICODE_SELECTED_MODES UC_WINC
// Valid options (that work well), UC_LNX UC_WINC UC_MAC keys can be made for switching between these WINC requires windows composer from github
#define USB_POLLING_INTERVAL_MS 5
// #define DEBOUNCE 5 // Just commented to potentially add debounce change due to some annoyances, potentially edit base config for each compile or use different command?
// If mechanical locking support could be disabled then that could save a little over 200 bytes
#define NO_ACTION_ONESHOT // Disables oneshot mods, saves about 500bytes
// Mousekey configs
#define MK_3_SPEED
#define MK_MOMENTARY_ACCEL
#define MOUSEKEY_MOVE_DELTA 20

#define MK_C_OFFSET_2 120
#define MK_C_INTERVAL_2 4
#define MK_W_OFFSET_UNMOD 2
#define MK_W_INTERVAL_UNMOD 120

#define MK_C_OFFSET_1 8
#define MK_W_OFFSET_1 1

// shit holding shift with win still triggers the layer switching, guess add layering to end of the if chain?
// maybe have ctrl be another option to slow down? having two speeds is just nice one for yeeting to get where you need to then move a lil