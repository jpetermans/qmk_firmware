#include QMK_KEYBOARD_H

enum alt_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
};


enum alt_layers {
    _BASE,
    _NUMPAD,
    _FNAV,
    _LED,
    _LOCK
};


#define TG_NKRO MAGIC_TOGGLE_NKRO //Toggle 6KRO / NKRO mode

keymap_config_t keymap_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,  \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_HOME, \
      TT(_FNAV), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  TG(_NUMPAD), \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   MO(_LED),  \
        KC_LCTL, KC_LGUI, KC_LALT,                          LT(_FNAV, KC_SPC),                    KC_RALT, MO(_LED),KC_LEFT, KC_DOWN, KC_RGHT  \
    ),
    [_NUMPAD] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______,  KC_P7,  KC_P8,  KC_P9, KC_PSLS, _______, _______, _______, _______,\
        _______, _______, _______, _______, _______, _______, _______,  KC_P4,  KC_P5,  KC_P6, KC_PAST, _______, _______, _______, _______,\
      MO(_FNAV), _______, _______, _______, _______, _______, _______,  KC_P1,  KC_P2,  KC_P3, KC_PMNS, _______,          _______, TG(_NUMPAD),\
        _______, _______, _______, _______, _______, _______, _______,  KC_P0,KC_COMM,KC_PDOT, KC_PPLS, _______,          _______, _______,\
        _______, _______, _______,                            KC_PENT,                         _______, _______, _______, _______, _______ \
    ),
    [_FNAV] = LAYOUT(
        KC_GRV,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7, KC_F8,    KC_F9,  KC_F10,  KC_F11,  KC_F12,  _______, _______, \
        KC_CAPS, _______, _______, _______, _______, _______, _______, KC_PGUP, KC_UP,  KC_PGDN, KC_PSCR, _______, _______,   KC_DEL, _______,  \
        _______, _______, KC_BTN2, KC_BTN1, _______, _______, KC_HOME, KC_LEFT, KC_DOWN,KC_RGHT,  KC_INS, _______,           _______, _______,\
        _______,  KC_APP, _______, KC_CALC, _______, _______, KC_END,  _______, _______, _______, _______, _______,          KC_VOLU, _______,\
        _______, _______, _______,                            _______,                       TD(TD_CAD), KC_NLCK, KC_MUTE, KC_VOLD, KC_MPLY \
    ),
    [_LED] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, TO(_LOCK), \
        _______, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, _______, _______, _______, _______, _______, _______, _______, _______, _______,  \
        _______, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, RGB_M_B, RGB_TOG, RGB_M_SW,RGB_M_K, MD_BOOT, TG_NKRO, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______  \
    ),
    [_LOCK] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TD(TD_UNLK), \
        XXXXXXX, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
        XXXXXXX, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, \
        XXXXXXX, RGB_M_B, RGB_TOG, RGB_M_SW,RGB_M_K, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, \
        XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
    ),
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______  \
    ),
    */
};

//Tap Dance Definitions
/* void unlock_kb (qk_tap_dance_state_t *state, void *user_data) { */
/*     if (state->count = 3) { */
/*         register_code(KC_LCTL) */
/*     else { */
/*         register_code(KC_NO) */
/*     } */

/* } */

void ctlaltdel_down (qk_tap_dance_state_t *state, void *user_data) {
    if (state->count >= 3) {
        register_code(KC_DEL);
        register_code(KC_LALT);
        register_code(KC_LCTL);
    } else {
    }
}
void ctlaltdel_up (qk_tap_dance_state_t *state, void *user_data) {
    if (state->count >= 3) {
        unregister_code(KC_DEL);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
    } else {
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
      [TD_UNLK] = ACTION_TAP_DANCE_DUAL_ROLE(XXXXXXX, _BASE),
      [TD_CAD]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ctlaltdel_down, ctlaltdel_up)
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
};

#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}
