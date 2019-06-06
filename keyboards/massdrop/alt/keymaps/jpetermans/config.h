#pragma once 

//overrides
#undef TAPPING_TOGGLE
#define TAPPING_TOGGLE 3
#define TAPPING_TERM 200
#define LEADER_TIMEOUT 300
#define LEADER_PER_KEY_TIMING

//RBG Matrix
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define DISABLE_RGB_MATRIX_DIGITAL_RAIN
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_TYPING_HEATMAP

//RGB Light settings related to Knight Rider and sets number of leds involved
#undef RGBLED_NUM
#define  RGBLED_NUM 7
