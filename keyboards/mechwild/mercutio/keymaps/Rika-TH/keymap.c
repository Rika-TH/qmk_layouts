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


#include QMK_KEYBOARD_H

#ifdef OLED_DRIVER_ENABLE // Probably shouldn't miss this lol? (could always put it in the external file I guess?)
#include "OLEDPATTERNS.C"
#endif

enum custom_layers {
    _CMAK,
    _QWRT,
    _NUMS,
    _FUNC,
};

enum custom_keycodes {
	MC_HBNO = SAFE_RANGE,	// Tiled discord emote "How about no"
	CKC_ESC,				// Custom gesc tab behaviour
};

// Custom Macros (hardcoded)
uint8_t custom_tabKC = 0; // Recording of which of the tab behaviours is active to reset when key is released

uint8_t shiftisheld = 0;
uint8_t altisheld = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {    
    case MC_HBNO:
        if (record->event.pressed) {
           SEND_STRING(":HowAboutNo1::HowAboutNo2\n");
           tap_code16(S(KC_ENT));
           SEND_STRING(":HowAboutNo3::HowAboutNo4:\n");
        } return false;
		
	// Upcoming is a fix attempt for the locking, probably VERY bad, may be removed
	case KC_LSFT:
		if (record->event.pressed) {shiftisheld = 1; register_code(KC_LSFT); }
		else {shiftisheld = 0; unregister_code(KC_LSFT); } return false;
	case KC_LALT:
		if (record->event.pressed) {altisheld = 1; register_code(KC_LALT); }
		else {altisheld = 0; unregister_code(KC_LALT); } return false;
		
	case CKC_ESC:
        if (record->event.pressed) {
			if (keyboard_report->mods & MOD_BIT(KC_LALT) && keyboard_report->mods & MOD_BIT(KC_LSFT)) {
				custom_tabKC = 1;
				unregister_code(KC_LSFT);
				unregister_code(KC_LALT);
				register_code(KC_GRV);
			}
			else if (keyboard_report->mods & MOD_BIT(KC_LALT)) {
				custom_tabKC = 2;
				unregister_code(KC_LALT);
				register_code(KC_ESC);
			}
			else {
				register_code(KC_TAB);
			}
        }
		else { // When the custom key is released (DESPERATELY NEEDS A CHECK FOR IF THE KEYS BEING REGISTERED ARE ACTUALLY HELD OTHERWISE THEY WILL LOCK) IT FUCKING WORKS
			if (custom_tabKC == 1) {
				unregister_code(KC_GRV);
				if (shiftisheld == 1) {register_code(KC_LSFT);}
				if (altisheld == 1) {register_code(KC_LALT);}
				custom_tabKC = 0;
			}
			else if (custom_tabKC == 2) {
				unregister_code(KC_ESC);
				if (altisheld == 1) {register_code(KC_LALT);}
				custom_tabKC = 0;
			}
			else {
			unregister_code(KC_TAB);
			}
		} return false; //I have no clue why I use return instead of break here but whatever
    }
    return true;
};

// ifdef can't really go here unless I add one for each of the keys used or something... Maybe alternate one? (undefined keys will be in map)
// same idea could be used on the keylock key
#define CC_DEGR UC(0xB0)	// Degree sign 			(0x00B0)
#define CC_PWR2 UC(0xB2)	// Superscript two 		(0x00B2)
#define CC_PWR3 UC(0xB3)	// Superscript three 		(0x00B3)
#define CC_SQRR UC(0x221A)	// Square root sign		(0x221A)
#define CC_CUBR UC(0x221B)	// Cube root sign			(0x221B)
#define CC_AMOG UC(0xD9E)	// Amogus (Sinhala script) 	(0x0D9E)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_CMAK] = LAYOUT_all(
                                                                                                                KC_MUTE,
    CKC_ESC,          KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
    KC_BSPC,          KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_ENT,
    KC_LSFT, KC_SLSH, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,           KC_RSFT,
    KC_LCTL, KC_LGUI, KC_LALT,          KC_SPC,  MO(_FUNC),        MO(_NUMS),        KC_RALT, KC_RGUI,          KC_RCTL ),

  [_QWRT] = LAYOUT_all(
                                                                                                                KC_MUTE,
    CKC_ESC,          KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_CAPS,          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,
    KC_LSFT, KC_SLSH, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,           KC_UP,
    KC_LCTL, KC_LGUI, KC_LALT,          KC_SPC,  MO(_FUNC),        MO(_NUMS),        KC_LEFT, KC_DOWN,          KC_RIGHT ),

  [_NUMS] = LAYOUT_all(
                                                                                                                KC_TRNS,
    KC_ESC,           KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
    KC_TRNS,          KC_TRNS, KC_GRV,  KC_NUBS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LBRC, KC_RBRC, KC_QUOT, KC_NUHS, KC_QUOT,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MINS, KC_EQL,  KC_SLSH,          KC_UP,
    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,          KC_TRNS,          KC_LEFT, KC_DOWN,          KC_RIGHT ),

  [_FUNC] = LAYOUT_all(
                                                                                                                KC_TRNS,
    KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  CC_AMOG,
    KC_TRNS,          KC_PSCR, CC_PWR2, CC_PWR3, KC_F11,  CC_DEGR, KC_TRNS, KC_F12,  KC_TRNS, KC_TRNS, KC_SLCK, RESET,
    KC_TRNS, KC_TRNS, KC_TRNS, CC_SQRR, CC_CUBR, KC_TAB,  KC_TRNS, MC_HBNO, KC_TRNS, KC_TRNS, KC_NLCK,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS, KC_LOCK,          KC_TRNS )
};

#ifdef ENCODER_ENABLE       // Encoder Functionality
    uint8_t selected_layer = 0;
    bool encoder_update_user(uint8_t index, bool clockwise) {
        #ifdef OLED_DRIVER_ENABLE
            oled_clear();
            oled_render();
        #endif
        switch (index) {
            case 0:         // This is the only encoder right now, keeping for consistency
                if ( clockwise ) {
                    if ( selected_layer  < 1 && keyboard_report->mods & MOD_BIT(KC_LSFT) ) { // If you are holding L shift, encoder changes layers. Edited to only work for layer 0 and 1
                        selected_layer ++;
                        layer_move(selected_layer);
                    } else if (keyboard_report->mods & MOD_BIT(KC_LALT) ) {
						if (shiftisheld == 1) {unregister_code(KC_LSFT);} // Here to make it still work correctly even if shift were held as why not! (added an if here aswell juuuust incase shouldn't decrease processing time so should be fine)
						tap_code16(A(KC_TAB));
						if (shiftisheld == 1) {register_code(KC_LSFT);}
					} else if (keyboard_report->mods & MOD_MASK_CTRL) {
						if (shiftisheld == 1) {unregister_code(KC_LSFT);}
						tap_code16(C(KC_TAB));
						if (shiftisheld == 1) {register_code(KC_LSFT);}
					} else {
						if ( selected_layer == 1 ) {
							tap_code(KC_VOLU); // Otherwise it just changes volume, not anymore! but now once more! And now only on QWERTY!
						}
                        else {
							tap_code(KC_WH_D);
						}
                    }
                } else if ( !clockwise ) {
                    if ( selected_layer  > 0 && keyboard_report->mods & MOD_BIT(KC_LSFT) ){
                        selected_layer --;
                        layer_move(selected_layer);
                    } else if (keyboard_report->mods & MOD_BIT(KC_LALT) ) {
						tap_code16(A(S(KC_TAB)));
					} else if (keyboard_report->mods & MOD_MASK_CTRL) {
						tap_code16(C(S(KC_TAB)));
					} else {
						if ( selected_layer == 1 ) {
							tap_code(KC_VOLD);
						}
                        else {
							tap_code(KC_WH_U);
						}
                    }
                }
        }
    return true;
    }
#endif

#ifdef OLED_DRIVER_ENABLE   // OLED Functionality
    oled_rotation_t oled_init_user(oled_rotation_t rotation) {
        return OLED_ROTATION_180;       // flips the display 180 degrees if offhand
    }

        bool clear_screen = false;          // used to manage singular screen clears to prevent display glitch
	// OLED patterns moved to external file to make navigating the keymap actually bareable
    void oled_task_user(void) {

        if ( IS_HOST_LED_OFF(USB_LED_NUM_LOCK) && IS_HOST_LED_OFF(USB_LED_CAPS_LOCK) && IS_HOST_LED_OFF(USB_LED_SCROLL_LOCK) && selected_layer == 0 && get_highest_layer(layer_state) == 0 ) {
            render_name();
            clear_screen = true;
        } else {
            if (clear_screen == true) {
                oled_clear();
                oled_render();
                clear_screen = false;
            }
			oled_set_cursor(0,0);
			switch (get_highest_layer(layer_state)) { //add number layer on here for oled, arrow in selected and potentially manual check all layer instead of here
                    case 0:
                        render_colemak();
                        break;
                    case 1:
                        render_qwerty();
                        break;
                    case 2:
                        render_numbersym();
                        break;
                    case 3:
                        render_functionrow();
                        break;
                    default:
                        break; // Imagine having a text output or similar for when something goes wrong (think this may be the cause of my confusion about a sudden 1KB compile size decrease lol)
                }
			/*switch (selected_layer) {
				case 0:
				render_ly1();
				//point arrow at layer 0 etc
				break;
				case 1:
				render_ly2();
				break;
				case 2:
				render_ly3(); // When I finally add all this these bits may well be effectively mute for my use as think I'm gonna stop layer lock for 2 and 3 as only have 0 and 1 as base layers
				break;
				case 3:
				render_ly4();
				break;
				default:
				break;
			}*/
			// if (IS_LAYER_ON(0) == true) {light layer 1 number}
			
			led_t led_state = host_keyboard_led_state();
			if (led_state.caps_lock == true) {
				render_caps();
			}
			if (led_state.num_lock == true) {
				render_num();
			}
			if (led_state.scroll_lock == true) {
				render_scroll();
			}
        }
    }
#endif