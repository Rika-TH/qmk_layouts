#include QMK_KEYBOARD_H

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

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
      fn_held = false;
      rgb_matrix_enable();
      rgb_matrix_sethsv(180, 255, 204);
    }
    break;
  case 2: //emote? layer
    layer_on(_ONE);
    layer_off(_TWO);
    rgb_matrix_set_color(4, 110, 194, 85);
    rgb_matrix_set_color(5, 56, 127, 12);
    rgb_matrix_set_color(6, 110, 194, 85);
    break;
  case 3: //macro layer
    layer_on(_TWO);
    layer_off(_ONE);
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

enum keyboard_keycodes {
CT_LY,
CKK_HA,
CKK_LE,
CKK_EL,
CKK_MO,
CKK_NO,
CKK_DS,
CKK_UP,
CKK_OK};

qk_tap_dance_action_t tap_dance_actions[] = {
    [CT_LY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_layers, dance_layers_finish)
};

// Custom Macros (hardcoded)
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case CKK_HA:
        if (record->event.pressed) {
            SEND_STRING(":hahaa:");
        } return false;

    case CKK_LE:
        if (record->event.pressed) {
            SEND_STRING(":LeSanae:");
        } return false;

    case CKK_EL:
        if (record->event.pressed) {
           SEND_STRING(":eanaseL:");
        } return false;

    case CKK_MO:
        if (record->event.pressed) {
           SEND_STRING(":MOGGERS:");
        } return false;

    case CKK_NO:
        if (record->event.pressed) {
           SEND_STRING(":HowAboutNo1::HowAboutNo2");
           register_code16(S(KC_ENT));
           SEND_STRING(":HowAboutNo3::HowAboutNo4");
        } return false;

    case CKK_DS:
        if (record->event.pressed) {
           SEND_STRING(":dipshit:");
        } return false;

    case CKK_UP:
        if (record->event.pressed) {
           SEND_STRING(":+1:");
        } return false;

    case CKK_OK:
        if (record->event.pressed) {
           SEND_STRING(":ok_hand:");
        } return false;
    }
    return true;
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_ZERO] = LAYOUT(
    KC_MPLY, CT_LY, KC_MUTE,
    MO(1),   KC_UP,   RGB_MOD,
    KC_LEFT, KC_DOWN, KC_RGHT
),

[_ONE] = LAYOUT(
    RESET,   KC_TRNS, KC_STOP,
     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS,  KC_TRNS, KC_TRNS
),

[_TWO] = LAYOUT(
    CKK_DS, KC_TRNS, CKK_NO,
    CKK_EL, CKK_HA, CKK_LE,
    CKK_UP, CKK_MO, CKK_OK
),

[_THREE] = LAYOUT(
    _______, KC_TRNS,_______,
    _______, _______, _______,
    _______, _______, _______
)

};

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
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
        } else {
            tap_code(KC_PGUP);
        }
    }
}