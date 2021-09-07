#include QMK_KEYBOARD_H

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

enum layer_names {
  _ZERO,
  _DISCORD,
  _MOUSE,
  _STG
};


bool fn_held;
void dance_layers(qk_tap_dance_state_t *state, void *user_data)
{
  if (state->pressed)
  {
    layer_on(_DISCORD);
    fn_held = true;
  }
  switch (state->count)
  {
  case 1: //off all layers, just base on
    if (!state->pressed)
    {
      layer_off(_MOUSE);
      layer_off(_DISCORD);
      layer_off(_STG);
      fn_held = false;
    }
    break;
  case 2: //emote? layer
    layer_on(_DISCORD);
    layer_off(_MOUSE);
    layer_off(_STG);
    break;
  case 3: //mouse layer
    layer_on(_MOUSE);
    layer_off(_DISCORD);
    layer_off(_STG);
    break;
  case 4: //STG layer
    layer_on(_STG);
    layer_off(_DISCORD);
    layer_off(_MOUSE);
    break;
  }
}
void dance_layers_finish(qk_tap_dance_state_t *state, void *user_data)
{
  if (fn_held)
  {
    layer_off(_DISCORD);
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
    MO(1),   KC_UP,   RGB_MOD,
    KC_LEFT, KC_DOWN, KC_RGHT
),

[_DISCORD] = LAYOUT(
    KC_G, KC_TRNS, KC_F,
    KC_D, KC_B, KC_J,
    KC_H, KC_E, KC_I
),

[_MOUSE] = LAYOUT(
    RESET,   KC_TRNS, KC_STOP,
     KC_TRNS, RESET, KC_TRNS,
     KC_TRNS,  KC_TRNS, KC_TRNS
),

[_STG] = LAYOUT(
    KC_ESC , KC_TRNS, KC_R,
    KC_C, KC_LCTL, KC_V,
    KC_LSFT, KC_Z, KC_X
)

};

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else { ctrl tab L1alt up down L2 mouse l+r L3hori L4
            tap_code(KC_VOLD);
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
        if (clockwise) {
            tap_code(KC_PGDN);
        } else { alt tab L1 ctrl alt up down L2 mouse u+d L3 vert L4
            tap_code(KC_PGUP);
        }
    }
}