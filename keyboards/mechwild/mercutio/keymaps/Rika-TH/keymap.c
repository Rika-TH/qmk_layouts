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



// win ralt encoder changes unicode output method displayed in currently dead space on oled (maybe text output aswell?)

#include QMK_KEYBOARD_H

#ifdef OLED_ENABLE
#include "OLEDPATTERNS.C"
#endif

enum custom_layers {
    _CMAK,
    _QWRT,
    _NUMS,
    _FUNC,
};

enum custom_keycodes {
    MC_HBNO = SAFE_RANGE,  // Tiled discord emote "How about no"
    CKC_ESC,               // Custom gesc tab behaviour
    CKC_ENC,
    CKC_CLKB,
};

// Custom Macros (hardcoded)
uint8_t custom_tabKC = 0; // Recording of which of the tab behaviours is active to reset when key is released

bool lshiftisheld = false;
bool laltisheld = false;
bool rsftqwert = false; // Checks up arrow key presence for purpose of shift encoder with one hand
bool raltqwert = false; // Checks left arrow key presence for purpose of alt encoder with one hand
bool rwinqwert = false; // Checks down arrow key presence for purpose of win encoder with one hand
bool rctlqwert = false; // Checks right arrow key presence for purpose of ctrl encoder with one hand

    bool process_record_user(uint16_t keycode, keyrecord_t *record) {
        switch (keycode) {    
                
            // Upcoming is a fix attempt for the locking, probably VERY bad, may be  r      emoved
    
    // on hard layer switch force reset of shift key
    // have timer maybe reset others or simply reset arrows if switching to colemak as there are no arrows and reset command keys when switching to QWERTY
    
    case KC_LSFT: // Start of the record key presence block
        if (record->event.pressed) {lshiftisheld = true; register_code(KC_LSFT); }
        else {lshiftisheld = false; unregister_code(KC_LSFT); } return false;
    case KC_LALT:
        if (record->event.pressed) {laltisheld = true; register_code(KC_LALT); }
        else {laltisheld = false; unregister_code(KC_LALT); } return false;
    case KC_UP:
        if (record->event.pressed) {rsftqwert = true; register_code(KC_UP); }
        else {rsftqwert = false; unregister_code(KC_UP); } return false;
    case KC_LEFT:
        if (record->event.pressed) {raltqwert = true; register_code(KC_LEFT); }
        else {raltqwert = false; unregister_code(KC_LEFT); } return false;
    case KC_DOWN:
        if (record->event.pressed) {rwinqwert = true; register_code(KC_DOWN); }
        else {rwinqwert = false; unregister_code(KC_DOWN); } return false;
    case KC_RIGHT:
        if (record->event.pressed) {rctlqwert = true; register_code(KC_RIGHT); }
        else {rctlqwert = false; unregister_code(KC_RIGHT); } return false;
        
        
    case MC_HBNO:
        if (record->event.pressed) {
           SEND_STRING(":HowAboutNo1::HowAboutNo2:");
           tap_code16(S(KC_ENT));
           SEND_STRING(":HowAboutNo3::HowAboutNo4\n");
        } return false;
        
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
                if (lshiftisheld == true) {register_code(KC_LSFT);}
                if (laltisheld == true) {register_code(KC_LALT);}
                custom_tabKC = 0;
            }
            else if (custom_tabKC == 2) {
                unregister_code(KC_ESC);
                if (laltisheld == true) {register_code(KC_LALT);}
                custom_tabKC = 0;
            }
            else {
            unregister_code(KC_TAB);
            }
        } return false; //I have no clue why I use return instead of break here but whatever
        
    case CKC_ENC:
        if (record->event.pressed) {
            if (keyboard_report->mods & MOD_MASK_GUI || rctlqwert == true){
                if (keyboard_report->mods & MOD_MASK_SHIFT){tap_code(KC_MS_BTN2);}
                else {tap_code(KC_MS_BTN1);}
            }
            tap_code(KC_MUTE);
        } return false;
        
    case CKC_CLKB:
        if (record->event.pressed) {clear_keyboard();}
        else {} return false;
        
    }
    return true;
};

// ifdef can't really go here unless I add one for each of the keys used or something... Maybe alternate one? (undefined keys will be in map)
// same idea could be used on the keylock key
#define CC_DEGR UC(0xB0)    // Degree sign             (0x00B0)
#define CC_PWR2 UC(0xB2)    // Superscript two         (0x00B2)
#define CC_PWR3 UC(0xB3)    // Superscript three         (0x00B3)
#define CC_SQRR UC(0x221A)    // Square root sign        (0x221A)
#define CC_CUBR UC(0x221B)    // Cube root sign            (0x221B)
#define CC_AMOG UC(0xD9E)    // Amogus (Sinhala script)     (0x0D9E)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_CMAK] = LAYOUT_all(
                                                                                                                CKC_ENC,
    CKC_ESC,          KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
    KC_BSPC,          KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_ENT,
    KC_LSFT, KC_SLSH, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,           KC_RSFT,
    KC_LCTL, KC_LGUI, KC_LALT,          KC_SPC,  MO(_FUNC),        MO(_NUMS),        KC_RALT, KC_RGUI,          KC_RCTL ),

  [_QWRT] = LAYOUT_all(
                                                                                                                CKC_ENC,
    CKC_ESC,          KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_CAPS,          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,
    KC_LSFT, KC_SLSH, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,           KC_UP,
    KC_LCTL, KC_LGUI, KC_LALT,          KC_SPC,  MO(_FUNC),        MO(_NUMS),        KC_LEFT, KC_DOWN,          KC_RIGHT ),

  [_NUMS] = LAYOUT_all(
                                                                                                                KC_TRNS,
    KC_ESC,           KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
    KC_TRNS,          KC_TRNS, KC_TRNS, KC_GRV,  KC_NUBS, KC_TRNS, KC_TRNS, KC_LBRC, KC_RBRC, KC_TRNS, KC_NUHS, KC_QUOT,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MINS, KC_EQL,  KC_SLSH,          KC_UP,
    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,          KC_TRNS,          KC_LEFT, KC_DOWN,          KC_RIGHT ),

  [_FUNC] = LAYOUT_all(
                                                                                                                KC_TRNS,
    KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  CC_AMOG,
    KC_TRNS,          KC_PSCR, CC_PWR2, CC_PWR3, KC_F11,  CC_DEGR, KC_TRNS, KC_F12,  CKC_CLKB,KC_TRNS, KC_SLCK, RESET,
    KC_TRNS, KC_TRNS, KC_TRNS, CC_SQRR, CC_CUBR, KC_TAB,  KC_TRNS, MC_HBNO, KC_TRNS, KC_TRNS, KC_NLCK,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS, KC_LOCK,          KC_TRNS )
};

#ifdef ENCODER_ENABLE       // Encoder Functionality
    uint8_t selected_layer = 0;
    bool encoder_update_user(uint8_t index, bool clockwise) {
        switch (index) {
            case 0:         // This is the only encoder right now, keeping for consistency
                if ( clockwise ) { // wait... FUCK IT, I will unreg both shifts etc, the really weird off chance error is fine, who the hell. why the hell do I spend another 3 minutes thinking about this to make ANY fix, this shouldn't need to be a thought in the processor
                    if ( selected_layer  < 1 && (keyboard_report->mods & MOD_MASK_SHIFT || rsftqwert == true) ){ // If you are holding L shift, encoder changes layers. Edited to only work for layer 0 and 1
                        selected_layer ++;
                        layer_move(selected_layer);
                        
                    } else if ((keyboard_report->mods & MOD_MASK_GUI || rwinqwert == true) && !(keyboard_report->mods & MOD_MASK_ALT)) { // Win key adds mouse movement with shift shifting direction // mouse down
                        if (keyboard_report->mods & MOD_MASK_SHIFT) {
                            register_code(KC_ACL1);
                            tap_code_delay(KC_MS_D, 120);
                            unregister_code(KC_ACL1);
                        }
                        else {
                            register_code(KC_ACL2);
                            tap_code_delay(KC_MS_D, 120);
                            unregister_code(KC_ACL2);
                        }
                        
                        } else if ((keyboard_report->mods & MOD_MASK_GUI || rctlqwert == true) && keyboard_report->mods & MOD_MASK_ALT) { // mouse right
                        if (keyboard_report->mods & MOD_MASK_SHIFT) {
                            register_code(KC_ACL1);
                            tap_code(KC_MS_R);
                            unregister_code(KC_ACL1);
                        }
                        else {
                            register_code(KC_ACL2);
                            tap_code(KC_MS_R);
                            unregister_code(KC_ACL2);
                        }
                        
                    } else if (keyboard_report->mods & MOD_MASK_ALT || raltqwert == true) {
                        if (lshiftisheld == true) {unregister_code(KC_LSFT);} // Here to make it still work correctly even if shift were held as why not! (added an if here aswell juuuust incase shouldn't decrease processing time so should be fine)
                        tap_code16(A(KC_TAB));
                        if (lshiftisheld == true) {register_code(KC_LSFT);}
                        
                    } else if (keyboard_report->mods & MOD_MASK_CTRL || rctlqwert == true) {
                        if (lshiftisheld == true) {unregister_code(KC_LSFT);}
                        tap_code16(C(KC_TAB));
                        if (lshiftisheld == true) {register_code(KC_LSFT);}
                        
                    } else {
                        if ( selected_layer == 1 ) {
                            tap_code(KC_VOLU); // Otherwise it just changes volume, not anymore! but now once more! And now only on QWERTY!
                        }
                        else {
                            tap_code(KC_WH_D);
                        }
                    }
                } else if ( !clockwise ) {
                    if ( selected_layer  > 0 && (keyboard_report->mods & MOD_MASK_SHIFT || rsftqwert == true) ){
                        selected_layer --;
                        layer_move(selected_layer);
                        
                    } else if ((keyboard_report->mods & MOD_MASK_GUI || rwinqwert == true) && !(keyboard_report->mods & MOD_MASK_ALT)) { // Win key adds mouse movement with shift shifting direction // mouse up
                        if (keyboard_report->mods & MOD_MASK_SHIFT) {
                            register_code(KC_ACL1);
                            tap_code(KC_MS_U);
                            unregister_code(KC_ACL1);
                        }
                        else {
                            register_code(KC_ACL2);
                            tap_code(KC_MS_U);
                            unregister_code(KC_ACL2);
                        }
                        
                    } else if ((keyboard_report->mods & MOD_MASK_GUI || rctlqwert == true) && keyboard_report->mods & MOD_MASK_ALT) { // mouse left
                        if (keyboard_report->mods & MOD_MASK_SHIFT) {
                            register_code(KC_ACL1);
                            tap_code(KC_MS_L);
                            unregister_code(KC_ACL1);
                        }
                        else {
                            register_code(KC_ACL2);
                            tap_code(KC_MS_L);
                            unregister_code(KC_ACL2);
                        }
                        
                    } else if (keyboard_report->mods & MOD_MASK_ALT || raltqwert == true) {
                        tap_code16(A(S(KC_TAB)));
                        
                    } else if (keyboard_report->mods & MOD_MASK_CTRL || rctlqwert == true) {
                        tap_code16(C(S(KC_TAB)));
                        
                    } else {
                        if ( selected_layer == 1 ) { // could make these have ifdefs maybe so the vol and mwheel could go off
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

#ifdef OLED_ENABLE   // OLED Functionality
    oled_rotation_t oled_init_user(oled_rotation_t rotation) {
        return OLED_ROTATION_180;       // flips the display 180 degrees if offhand
    }

        bool clear_screen = false;          // used to manage singular screen clears to prevent display glitch
    // OLED patterns moved to external file to make navigating the keymap actually bareable
    void oled_task_user(void) { // hmm, saw someone mention orders of when things render and think this is the first one? might be usable like a layer system, have most things do on the bottom and then on the top so they don't block eachother
       
       if ( IS_HOST_LED_OFF(USB_LED_NUM_LOCK) && IS_HOST_LED_OFF(USB_LED_CAPS_LOCK) && IS_HOST_LED_OFF(USB_LED_SCROLL_LOCK) && selected_layer == 0 && get_highest_layer(layer_state) == 0 ) {
            render_name();
            clear_screen = true;
        } else {
            if (clear_screen == true) {
                oled_clear();
                oled_render();
                clear_screen = false;
            }
            oled_set_cursor(0,0); //hmm, so maybe it's things after here stopping this from surpasing them, so guess I NEED some part of the clear screen
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
                    default: // if more than 4 layers are present then more cases will want to be added naturally
                        render_what();
                        break; // Imagine having a text output or similar for when something goes wrong (think this may be the cause of my confusion about a sudden 1KB compile size decrease lol)
                }
            switch (selected_layer) {
                case 0:
                render_ly1arr();
                //point arrow at layer 0 etc
                break;
                case 1: // STILL need to fix this, it's even more annoying now I've actually added it as there's the line from the logo...
                render_lyno3arr2();
                render_ly2arr();
                break;
                case 2: // Unused in current layout as layer 3 and 4 are inaccessible via lock, only as temp layers
                render_ly3arr();
                break;
                case 3:
                render_ly4arr();
                break;
                default:
                render_what();
                break;
            }
            /*
            if (IS_LAYER_ON(_CMAK)){// hmm actually it could be an idea to instead of use named var for all of these use the static number so that way the code can be copy pasted easier
                    if (IS_LAYER_ON(_QWRT)){
                        if (IS_LAYER_ON(_NUMS)){
                            if (IS_LAYER_ON(_FUNC)){
                                render_ly2num1();
                                render_ly3num2();
                                render_ly4num3();
                            }
                            else {
                                render_ly2num1();
                                render_ly3num2();
                                render_lyno4num3();
                            }
                        }
                        else if (IS_LAYER_ON(_FUNC)){
                            render_ly2num1();
                            render_lyno3num2();
                            render_ly4num();
                        }
                        else {
                            render_ly2num1();
                            render_lyno3num2();
                        }
                    }
                    else {
                        if (IS_LAYER_ON(_NUMS)){
                            if (IS_LAYER_ON(_FUNC)){
                                render_ly1num();
                                render_ly3num();
                                render_ly4num3();
                            }
                            else {
                                render_ly1num();
                                render_ly3num();
                                render_lyno4num3();
                            }
                        }
                        else if (IS_LAYER_ON(_FUNC)){
                            render_ly1num();
                            render_ly4num();
                        }
                        else {
                            render_ly1num();
                        }
                    }
                }
            else if (IS_LAYER_ON(_QWRT)){
                    if (IS_LAYER_ON(_NUMS)){
                        if (IS_LAYER_ON(_FUNC)){
                            render_ly2num();
                            render_ly3num2();
                            render_ly4num3();
                        }
                        else {
                            render_ly2num();
                            render_ly3num2();
                            render_lyno4num3();
                        }
                    }
                    else if (IS_LAYER_ON(_FUNC)){
                        render_ly2num();
                        render_lyno3num2();
                        render_ly4num();
                    }
                    else {
                        render_ly2num();
                        render_lyno3num2();
                    }
                }
            */
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