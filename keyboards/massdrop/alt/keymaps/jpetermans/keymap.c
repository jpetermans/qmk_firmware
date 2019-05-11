#include QMK_KEYBOARD_H

//define led display settings, values for led_mode_global
//using bit flags
#define LED_ALL 0x00 //should match rgb_matrix_types.h
#define LED_MODS 0x01 //should match rgb_matrix_types.h
#define LED_UNDERGLOW 0x02 //should match rgb_matrix_types.h
#define LED_KEYS 0x04 //should match rgb_matrix_types.h
#define LED_UNDERGLOW_SINGLE 0x08
#define LED_KEYS_SINGLE 0x10
#define LED_KEYS_ACTIVE 0x20


enum alt_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
    KC_LALL,
    KC_LMOD,
    KC_LGLW,
    KC_LKYS,
    KC_LGSL,
    KC_LKSL,
    KC_LKAK,
    LK_____,
    DYNAMIC_MACRO_RANGE,    //Dynamic macros
};

//must be after DYNAMIC_MACRO_RANGE is set
#include "dynamic_macro.h"
#define KC_DRS1 DYN_REC_START1
#define KC_DRS2 DYN_REC_START2
#define KC_DMP1 DYN_MACRO_PLAY1
#define KC_DMP2 DYN_MACRO_PLAY2
#define KC_DRS  DYN_REC_STOP
#define TG_NKRO MAGIC_TOGGLE_NKRO //Toggle 6KRO / NKRO mode


enum tap_keys {
    TD_CAD,
    TD_LOCK,
    TD_UNLK,
    TD_F4,
    TD_DKL,
    TD_DKR,
};

enum alt_layers {
    _BASE,
    _NUMPAD,
    _FNAV,
    _LOCK,
    _UNLOCK ,
    _LED,
    _SYS,
};



keymap_config_t keymap_config;
uint16_t led_mode_global = 0x00; //start off

// leader key
LEADER_EXTERNS();
bool leader_success;
bool leader_unlock;

uint8_t arr_rgb [3]; //r, g, b
int arr_ledindex [2]; //led_gsk, led_ksk

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,  \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_LEAD, \
      TT(_FNAV), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  TG(_NUMPAD), \
        KC_LSPO, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,          KC_UP,   TG(_LED),  \
        KC_LCTL, KC_LGUI, TD(TD_DKL),                    LT(_FNAV, KC_SPC),                    TD(TD_DKR), MO(_LED),KC_LEFT, KC_DOWN, MT(MOD_LCTL, KC_RGHT)  \
    ),
    [_NUMPAD] = LAYOUT(
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_NLCK,XXXXXXX,KC_PSLS,KC_PAST, KC_PMNS, _______, _______, XXXXXXX, \
        _______, XXXXXXX, KC_MS_U, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_P7,  KC_P8,  KC_P9,  KC_PPLS, _______, _______, _______, XXXXXXX, \
      MO(_FNAV), KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX, XXXXXXX, XXXXXXX,  KC_P4,  KC_P5,  KC_P6,  _______, _______,          _______, TG(_NUMPAD),\
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_COMM,  KC_P1,  KC_P2,  KC_P3,  XXXXXXX, _______,          _______, XXXXXXX, \
        _______, _______, _______,                              KC_P0,                          KC_PDOT, XXXXXXX, _______, _______, _______ \
    ),
    [_FNAV] = LAYOUT(
        KC_GRV,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7, KC_F8,    KC_F9,  KC_F10,  KC_F11,  KC_F12,  _______, TG(_LOCK), \
        KC_CAPS, KC_DMP2, _______, _______, _______, _______, _______, KC_PGUP, KC_UP,  KC_PGDN, KC_PSCR, _______, _______,   KC_DEL, _______, \
        _______, KC_DMP1, KC_BTN2, KC_BTN1, _______, _______, KC_HOME, KC_LEFT, KC_DOWN,KC_RGHT,  KC_INS, _______,           _______, _______, \
        _______,  KC_APP, _______, KC_CALC, _______, _______, KC_END,  _______, _______, _______, _______, _______,          KC_VOLU, OSL(_SYS),\
        _______, _______, _______,                            _______,                            _______, KC_NLCK, KC_MUTE, KC_VOLD, KC_MPLY \
    ),
    [_LOCK] = LAYOUT(
        LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, \
        LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, \
        LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____,          LK_____, LK_____, \
        LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____, LK_____,          MO(_UNLOCK), LK_____, \
        LK_____, LK_____, LK_____,                            LK_____,                            LK_____, MO(_LED), LK_____, LK_____, LK_____  \
    ),
    [_UNLOCK] = LAYOUT(
        KC_LEAD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
        XXXXXXX, XXXXXXX, KC_LCTL, KC_RCTL, KC_MS_U, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, \
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, \
        XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
    ),
    [_LED] = LAYOUT(
        XXXXXXX, KC_LALL, KC_LGLW, KC_LKYS, KC_LGSL, KC_LKSL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TD(TD_UNLK), \
        XXXXXXX, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
        XXXXXXX, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, \
        XXXXXXX, RGB_M_B, RGB_TOG, RGB_M_SW,RGB_M_K, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, \
        XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
    ),
    [_SYS] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_DRS1, \
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_DRS2, \
       TG(_SYS), XXXXXXX ,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, KC_DRS, \
        XXXXXXX, XXXXXXX ,XXXXXXX, XXXXXXX, XXXXXXX, MD_BOOT, TG_NKRO, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, \
        XXXXXXX, XXXXXXX, XXXXXXX,                            KC_LEAD,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
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
      [TD_F4] = ACTION_TAP_DANCE_DOUBLE(KC_RALT, LALT(KC_F4)),
      [TD_CAD]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ctlaltdel_down, ctlaltdel_up),
      [TD_DKR] = ACTION_TAP_DANCE_DOUBLE(KC_RALT, LCTL(LGUI(KC_RGHT))),
      [TD_DKL] = ACTION_TAP_DANCE_DOUBLE(KC_LALT, LCTL(LGUI(KC_LEFT)))
};



// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
      LEADER_DICTIONARY() {
          leader_success = leading = false;
          leader_unlock = false;

          SEQ_TWO_KEYS(KC_W, KC_Q) {
              SEND_STRING(SS_LALT(SS_TAP(X_F4)));
              leader_success = true;
          }

          SEQ_THREE_KEYS(KC_RCTL, KC_LCTL, KC_MS_U) {
              layer_off(_LOCK);
              leader_unlock = true;
          }
          leader_end();
      }
};


void leader_end(void){
    if (leader_success){
        // nothing
    } else if (leader_unlock){
        //TODO: positive led blink, lighting does not work, but wait does occur
        /* rgb_matrix_set_color(70, 0xFF, 0x00, 0x00); */
        /* rgb_matrix_set_color(75, 0xFF, 0x00, 0x00); */
        /* rgb_matrix_set_color(80, 0xFF, 0x00, 0x00); */
        /* wait_ms(500); */
        /* rgb_matrix_set_color(70, 0x00, 0x00, 0x00); */
        /* rgb_matrix_set_color(75, 0x00, 0x00, 0x00); */
        /* rgb_matrix_set_color(80, 0x00, 0x00, 0x00); */
        /* wait_ms(450); */
        /* rgb_matrix_set_color(70, 0xFF, 0x00, 0x00); */
        /* rgb_matrix_set_color(75, 0xFF, 0x00, 0x00); */
        /* rgb_matrix_set_color(80, 0xFF, 0x00, 0x00); */
        /* wait_ms(900); */
        /* rgb_matrix_set_color(70, 0x00, 0x00, 0x00); */
        /* rgb_matrix_set_color(75, 0x00, 0x00, 0x00); */
        /* rgb_matrix_set_color(80, 0x00, 0x00, 0x00); */
    }
}


#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;


    if (!process_record_dynamic_macro(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case KC_LALL:
            if (record->event.pressed) {
                led_mode_global = LED_ALL;
                rgb_matrix_indicators_user();
            }
            return false;
        case KC_LGLW:
            if (record->event.pressed) {
                led_mode_global = LED_UNDERGLOW;
                rgb_matrix_indicators_user();
            }
            return false;
        case KC_LKYS:
            if (record->event.pressed) {
                led_mode_global = LED_KEYS;
                rgb_matrix_indicators_user();
            }
            return false;
        case KC_LGSL:
            if (record->event.pressed) {
                led_mode_global = LED_UNDERGLOW_SINGLE;
                rgb_matrix_indicators_user();
            }
            return false;
        case KC_LKSL:
            if (record->event.pressed) {
                led_mode_global = LED_KEYS_SINGLE;
                rgb_matrix_indicators_user();
            }
            return false;
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


//TODO: turn off leds on usb suspend
void suspend_power_down_user(void) {
    rgb_matrix_set_suspend_state(true);
}
void suspend_wakeup_init_user(void) {
    rgb_matrix_set_suspend_state(false);
}

void matrix_init_keymap(void) {
}


//these probably go away once PR(#5811) merged
extern rgb_config_t rgb_matrix_config;
extern led_config_t g_led_config;

// from drashna's ergodox keymap.c, sets full set of leds rather than individual
// also from keyboards/dztech/dz40rgb/keymaps/default/keymap.c for new rgb structure from PR(#5783)
// added new parameter to filter effect by LED_FLAG_
void rgb_matrix_layer_helper (uint8_t red, uint8_t green, uint8_t blue, int led_index[2], uint8_t led_flag) {
    switch (led_flag) {
        case LED_ALL:
        case LED_UNDERGLOW:
        case LED_KEYS:
            for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
                if (HAS_FLAGS(g_led_config.flags[i], led_flag)) {
                    rgb_matrix_set_color( i, red, green, blue );
                }
            }
            break;
        case LED_UNDERGLOW_SINGLE:
            rgb_matrix_set_color(led_index[0], red, green, blue);
            break;
        case LED_KEYS_SINGLE:
            rgb_matrix_set_color(led_index[1], red, green, blue);
            break;
    }
}

//function to set keyboard led settings to all, keys, active keys, underglow, etc. should match rgb_matrix_types.h values
//use array to set active key leds and current settings (e.g. NGKRO)

//TODO: adjust hsv.val of layer indicators
//TODO: if RGB_TOG is off then this does not update (flush only?)
//TODO: how often does this run?
void rgb_matrix_indicators_user(void) {
    //read global setting for all, keys, underglow, underglow indicator or key indicator and set led_mode_global
    //TODO: text if leds toggled on? and don't run if off
    switch (biton32(layer_state)) {
        case _BASE:
            arr_ledindex [0] = 69; // led_gsk
            arr_ledindex [1] = 1;// led_ksk
            rgb_matrix_layer_helper(RGB_WHITE, arr_ledindex, led_mode_global);
            break;

        case _NUMPAD:
            arr_ledindex [0] = 69; // led_gsk
            arr_ledindex [1] = 3; // led_ksk
            rgb_matrix_layer_helper(RGB_GREEN, arr_ledindex, led_mode_global);
            break;

        case _FNAV:
            arr_ledindex [0] = 70; // led_gsk
            arr_ledindex [1] = 2; // led_ksk
            rgb_matrix_layer_helper(RGB_BLUE, arr_ledindex, led_mode_global);
            break;

        case _LOCK:
        case _UNLOCK:
            arr_ledindex [0] = 69; // led_gsk
            arr_ledindex [1] = 14;// led_ksk
            rgb_matrix_layer_helper(RGB_RED, arr_ledindex, led_mode_global);
            break;
        /* case _UNLOCK: */

        case _LED:
            arr_ledindex [0] = 69; // led_gsk
            arr_ledindex [1] = 4; // led_ksk
            rgb_matrix_layer_helper(RGB_GOLD, arr_ledindex, led_mode_global);
            break;

        case _SYS:
            arr_ledindex [0] = 69; // led_gsk
            arr_ledindex [1] = 5; // led_ksk
            rgb_matrix_layer_helper(RGB_CORAL, arr_ledindex, led_mode_global);
    }
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    //TODO: setting heatmap sets permanent underglow until mode is changed.
    //      Underglow continues to not update with rgb_matrix_indicators_user
    /* rgb_matrix_config.mode = RGB_MATRIX_TYPING_HEATMAP; */
    rgblight_mode(RGB_MATRIX_TYPING_HEATMAP);
    /* arr_ledindex[0] = 69; */
    /* rgb_matrix_layer_helper(RGB_WHITE, arr_ledindex, LED_UNDERGLOW_SINGLE); */
};

