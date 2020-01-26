#include QMK_KEYBOARD_H
#include "led_controller.h"

//Define Layer Names
#define _BASE 0
#define _NUMPAD 1
#define _FNAV 2
#define _MEDIA 3
#define _TILDE 4

//IS31 chip has 8 available led pages, using 0 for all leds and 7 for single toggles
#define MAX_PAGES 6

enum tap_keys {
    TD_CAD,
    TD_LDR,
    TD_F4,
    TD_DKL,
    TD_DKR,
};

enum ic60_led_modes {
    MODE_ALL,
    MODE_GAME,
    MODE_SINGLE,
    MODE_PAGE,
    MODE_FLASH,
};

enum ic60_keycodes {
    BACKLT = SAFE_RANGE,
    BRIGHT,
    DIM,
    ALL,
    GAME,
    LED_SK,
    LED_PG,
    LED_FLSH,
};

uint32_t led_layer_state = 0; //used to match led layers to the current keymap layer
uint8_t led_mode_global = MODE_SINGLE;
uint8_t backlight_status_global = 1; //init on/off state of backlight

// leader key
LEADER_EXTERNS();
bool leader_success;
bool leader_unlock;

/* ==================================
 *             KEYMAPS
 * ==================================*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_60_ansi_split_bs_rshift(
        KC_GESC,   KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_MINS, KC_EQL, TD(TD_LDR),KC_NO,
        KC_TAB,    KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_LBRC, KC_RBRC,KC_BSPC,
        TT(_FNAV), KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT, KC_ENT,
        KC_LSFT,   KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,KC_RSFT, KC_NO,
        KC_LCTL,   KC_LGUI,TD(TD_DKL),            LT(_FNAV, KC_SPC),      TD(TD_DKR),TG(_NUMPAD),MO(_MEDIA), KC_RCTL
    ),

    /* numpad */
    [_NUMPAD] = LAYOUT_60_ansi_split_bs_rshift(
        _______,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,KC_NLCK, KC_PSLS,KC_PAST,KC_PMNS,_______,_______,_______,KC_NO,
        _______,XXXXXXX,KC_MS_U,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,   KC_P7, KC_P8, KC_P9,  KC_PPLS, _______,_______,_______,
        MO(_FNAV),KC_MS_L,KC_MS_D,KC_MS_R,XXXXXXX,XXXXXXX,XXXXXXX, KC_P4, KC_P5, KC_P6,  _______, _______,_______,
        _______,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,   KC_P1, KC_P2, KC_P3,  _______, _______,KC_NO,
        _______,_______,_______,                  KC_P0,                         KC_DOT,_______,_______,_______
    ),

    /* F-, arrow, and media keys */
    [_FNAV] = LAYOUT_60_ansi_split_bs_rshift(
        KC_GRV, KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12, _______,KC_NO,
        KC_CAPS,DM_PLY2,_______,_______,_______,_______,_______,KC_PGUP,KC_UP,KC_PGDN,KC_PSCR,_______,_______,KC_DEL,
        _______,DM_PLY1,KC_BTN2,KC_BTN1,_______,_______,KC_HOME,KC_LEFT,KC_DOWN,KC_RGHT,KC_INS,_______,_______,
        _______,KC_APP, _______,KC_CALC,_______,_______,KC_END,_______,_______,_______,_______,_______,KC_NO,
        _______,_______,_______,               _______,           TD(TD_CAD),KC_NLCK,_______,_______
    ),

    /* media */
    [_MEDIA] = LAYOUT_60_ansi_split_bs_rshift(
        _______, LED_SK,LED_PG,LED_FLSH,_______,_______,_______, _______, _______, _______,KC_MUTE, KC_VOLD, KC_VOLU,_______,KC_NO,
        _______,DM_REC2,_______,_______,_______,_______,_______, _______, _______, _______,_______, _______,_______,_______,
        _______,DM_REC1,_______,_______,_______,   GAME,_______, _______, _______, _______,_______, _______,_______,
        _______,DM_RSTP,     ALL, BRIGHT,    DIM, BACKLT,_______, _______, KC_MPRV, KC_MNXT,KC_MSTP, _______,KC_NO,
        _______,_______,_______,                KC_MPLY,             _______,_______, _______,_______
    ),
    /* template */
    /* [4] = LAYOUT_60_ansi_split_bs_rshift(*/
    /*     _______,_______,_______,_______,_______,_______,_______, _______, _______, _______,_______, _______,_______,_______,KC_NO, */
    /*     _______,_______,_______,_______,_______,_______,_______, _______, _______, _______,_______, _______,_______,_______,*/
    /*     _______,_______,_______,_______,_______,_______,_______, _______, _______, _______,_______, _______,_______,*/
    /*     _______,_______,_______,_______,_______,_______,_______, _______, _______, _______,_______, _______,KC_NO,*/
    /*     _______,_______,_______,               _______,             _______,_______, _______,_______*/
    /* ), */
};

/* ==================================
 *          LED MAPPING
 * ==================================*/

/*
    Infinity60 LED MAP
    11 12 13 14 15 16 17 18 21 22 23 24 25  26 27*
     28 31 32 33 34 35 36 37 38 41 42 43 44 45
     46 47 48 51 52 53 54 55 56 57 58 61    62
      63 64 65 66 67 68 71 72 73 74 75      76 77*
    78  81  82       83         84  85  86  87
    *Unused in Alphabet Layout
*/

//======== full page arrays =========
//any change in array size needs to be mirrored in matrix_init_user
uint8_t led_numpad[18] = {
  18,   22,23,24,25,
  37,38,41,42,
  55,56,57,58,
  72,73,74,75,
     83
};
//LED Page 2 - _Nav
uint8_t led_nav[13] = {
     31,                  38,
  47,48,              55,56,57,
     64,65,66
};
//LED Page 3 - _Media
uint8_t led_media[19] = {
  12,13,14,           23,24,25,
     31,
     47,48,
     64,65,66,67,68,    73,74,75,
           83,          86
};
//LED Page 4 - _Game "WASD"
uint8_t led_game[5] = {
  11,
        32,
     47,48,51
};

//======== qmk functions =========
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    msg_t msg;

    switch (keycode) {
        case ALL:
          if(record->event.pressed) {
            led_mode_global = led_mode_global == MODE_ALL ? MODE_SINGLE : MODE_ALL;
            msg=TOGGLE_ALL;
            chMBPost(&led_mailbox, msg, TIME_IMMEDIATE);
          }
          return false;

        case BACKLT:
          if(record->event.pressed) {
            backlight_status_global ^= 1;
            msg=(backlight_status_global << 8) | TOGGLE_BACKLIGHT;
            chMBPost(&led_mailbox, msg, TIME_IMMEDIATE);
          }
          return false;

        case GAME:
          if(record->event.pressed) {
            led_mode_global = led_mode_global == MODE_GAME ? MODE_SINGLE : MODE_GAME;

            msg=(4 << 8) | DISPLAY_PAGE;
            chMBPost(&led_mailbox, msg, TIME_IMMEDIATE);
          }
          return false;

        case BRIGHT:
          if(record->event.pressed) {
            msg=(1 << 8) | STEP_BRIGHTNESS;
            chMBPost(&led_mailbox, msg, TIME_IMMEDIATE);
          }
          return false;

        case DIM:
          if(record->event.pressed) {
            msg=(0 << 8) | STEP_BRIGHTNESS;
            chMBPost(&led_mailbox, msg, TIME_IMMEDIATE);
          }
          return false;

        //set led_mode for matrix_scan to toggle leds
        case LED_SK:
          led_mode_global = MODE_SINGLE;
          return false;
        case LED_PG:
          led_mode_global = MODE_PAGE;
          return false;
        case LED_FLSH:
          led_mode_global = MODE_FLASH;
          return false;

        default:
            return true;
    }
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    led_controller_init();

    // Write predefined led pages.
    write_led_page(_NUMPAD, led_numpad, 16);
    chThdSleepMilliseconds(10);

    write_led_page(_FNAV, led_nav, 12);
    chThdSleepMilliseconds(10);

    write_led_page(_MEDIA, led_media, 15);
    chThdSleepMilliseconds(10);

    write_led_page(4, led_game, 5);
    chThdSleepMilliseconds(1000);
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
      [TD_LDR] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, KC_LEAD),
      [TD_F4] = ACTION_TAP_DANCE_DOUBLE(KC_RALT, LALT(KC_F4)),
      [TD_CAD]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ctlaltdel_down, ctlaltdel_up),
      [TD_DKR] = ACTION_TAP_DANCE_DOUBLE(KC_RALT, LCTL(LGUI(KC_RGHT))),
      [TD_DKL] = ACTION_TAP_DANCE_DOUBLE(KC_LALT, LCTL(LGUI(KC_LEFT)))
};

// Loops constantly in the background.
void matrix_scan_user(void) {

  LEADER_DICTIONARY() {
      leader_success = leading = false;
      leader_unlock = false;

      SEQ_TWO_KEYS(KC_W, KC_Q) {
          SEND_STRING(SS_LALT(SS_TAP(X_F4)));
          leader_success = true;
      }

      SEQ_TWO_KEYS(KC_G, KC_M) {
          SEND_STRING("@gmail.com");
          leader_unlock = true;
      }

      SEQ_THREE_KEYS(KC_A, KC_E, KC_W) {
          SEND_STRING("@");
          leader_unlock = true;
      }

      leader_end();
  }
}

void led_matrix_indicators_user(void) {
  uint8_t page;
  uint8_t led_pin_byte;
  msg_t msg;

  if (backlight_status_global == 0) {//backlight is off, skip the rest
    return;
  }

  if (led_layer_state != layer_state && led_mode_global != MODE_GAME && led_mode_global != MODE_ALL) {
    //check mode
    //Turn on layer indicator or page depending on mode
    switch(led_mode_global) {
      case MODE_FLASH: //flash preset page leds then single indicator
        page = biton32(layer_state) > MAX_PAGES ? 7 : biton32(layer_state);
        msg=(page << 8) | DISPLAY_PAGE;
        chMBPost(&led_mailbox, msg, TIME_IMMEDIATE);
        chThdSleepMilliseconds(500);
        //flow to display single layer leds

      case MODE_SINGLE: //light layer indicators for all active layers
        led_pin_byte = layer_state & 0xFF;
        msg=(7 << 8) | DISPLAY_PAGE;
        chMBPost(&led_mailbox, msg, TIME_IMMEDIATE);
        msg=(1 << 16) | (led_pin_byte << 8) | SET_FULL_ROW;
        chMBPost(&led_mailbox, msg, TIME_IMMEDIATE);
        break;

      case MODE_PAGE: //display pre-defined led page
        page = biton32(layer_state) > MAX_PAGES ? 7 : biton32(layer_state);
        msg=(page << 8) | DISPLAY_PAGE;
        chMBPost(&led_mailbox, msg, TIME_IMMEDIATE);
        break;
      }
  led_layer_state = layer_state;
  }
}
