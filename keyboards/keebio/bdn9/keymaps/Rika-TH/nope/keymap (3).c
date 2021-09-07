#include QMK_KEYBOARD_H

bool is_alt_tab_active = false;    // ADD this near the begining of keymap.c
uint16_t alt_tab_timer = 0;        // we will be using them soon.

enum custom_keycodes {             // Make sure have the awesome keycode ready
  ALT_TAB = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {               // This will do most of the grunt work with the keycodes.
    case ALT_TAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
  }
  return true;
}

void matrix_scan_user(void) {     // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}


enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

// ZERO: BASE, ONE: DISCORD, TWO: MOUSE, THREE: STG
enum layer_names {
  _ZERO,
  _ONE, 
  _TWO,
  _THREE
};


bool fn_held;
void dance_layers(qk_tap_dance_state_t *state, void *user_data)
{
  if (state->pressed)
  {
    layer_on(_ONE);
    fn_held = true;
  }
  switch (state->count)
  {
  case 1: //off all layers, just base on
    if (!state->pressed)
    {
      layer_off(_TWO);
      layer_off(_ONE);
      layer_off(_THREE);
      fn_held = false;
    }
    break;
  case 2: //emote? layer
    layer_on(_ONE);
    layer_off(_TWO);
    layer_off(_THREE);
    break;
  case 3: //mouse layer
    layer_on(_TWO);
    layer_off(_ONE);
    layer_off(_THREE);
    break;
  case 4: //STG layer
    layer_on(_THREE);
    layer_off(_ONE);
    layer_off(_TWO);
    break;
  }
}
void dance_layers_finish(qk_tap_dance_state_t *state, void *user_data)
{
  if (fn_held)
  {
    layer_off(_ONE);
    fn_held = false;

  }
}

enum {
LY_SHF = 0,
};

qk_tap_dance_action_t tap_dance_actions[] = {
    [LY_SHF] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_layers, dance_layers_finish),
};

// Custom Macros (hardcoded)
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case KC_B:
        if (record->event.pressed) {
            SEND_STRING(":hahaa:\n");
        } return false;

    case KC_J:
        if (record->event.pressed) {
            SEND_STRING(":LeSanae:\n");
        } return false;

    case KC_D:
        if (record->event.pressed) {
           SEND_STRING(":eanaseL:\n");
        } return false;

    case KC_E:
        if (record->event.pressed) {
           SEND_STRING(":MOGGERS:\n");
        } return false;

    case KC_F:
        if (record->event.pressed) {
           SEND_STRING(":HowAboutNo1::HowAboutNo2\n");
           tap_code16(S(KC_ENT));
           SEND_STRING(":HowAboutNo3::HowAboutNo4:\n");
        } return false;

    case KC_G:
        if (record->event.pressed) {
           SEND_STRING(":dipshit:\n");
        } return false;

    case KC_H:
        if (record->event.pressed) {
           SEND_STRING(":+1:\n");
        } return false;

    case KC_I:
        if (record->event.pressed) {
           SEND_STRING(":ok_hand:\n");
        } return false;
    }
    return true;
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_ZERO] = LAYOUT(
    KC_MPLY, TD(LY_SHF), KC_MUTE,
    MO(2),   KC_UP,   KC_F13,
    KC_LEFT, KC_DOWN, KC_RGHT
),

[_ONE] = LAYOUT(
    KC_G, KC_TRNS, KC_F,
    KC_D, KC_B, KC_J,
    KC_H, KC_E, KC_I
),

[_TWO] = LAYOUT(
    RESET,   KC_TRNS, KC_STOP,
     KC_BTN1, KC_ACL2, KC_BTN2,
     KC_WH_U, KC_BTN3, KC_WH_D
),

[_THREE] = LAYOUT(
    KC_ESC , KC_TRNS, KC_R,
    KC_C, KC_LCTL, KC_V,
    KC_LSFT, KC_Z, KC_X
)

};

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
    switch(biton32(layer_state)) {
      case _THREE:
        clockwise ? tap_code16(KC_RIGHT) : tap_code16(KC_LEFT);
        break;
      case _TWO:
        clockwise ? tap_code16(KC_MS_R) : tap_code16(KC_MS_L);
        break;
      case _ONE:
        clockwise ? tap_code16(A(KC_UP)) : tap_code16(A(KC_DOWN));
        break;
      default:
        clockwise ? tap_code16(C(KC_TAB)) : tap_code16(C(S(KC_TAB)));
        break;
      }
    }
    else if (index == _MIDDLE) {
        if (clockwise) {
            tap_code(KC_DOWN);
        } else {
            tap_code(KC_UP);
        }
    }
    else if (index == _RIGHT) {
    switch(biton32(layer_state)) {
      case _THREE:
        clockwise ? tap_code16(KC_DOWN) : tap_code16(KC_UP);
        break;
      case _TWO:
        clockwise ? tap_code16(KC_MS_D) : tap_code16(KC_MS_U);
        break;
      case _ONE:
        clockwise ? tap_code16(C(A(KC_DOWN))) : tap_code16(C(A(KC_UP)));
        break;
      default:
        clockwise ? tap_code16(A(KC_TAB)) : tap_code16(A(S(KC_TAB)));
        break;
      }
    }
}