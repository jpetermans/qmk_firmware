/*
Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "infinity60.h"

void matrix_init_kb(void) {
	// put your keyboard start-up code here
	// runs once when the firmware starts up

	matrix_init_user();
}

void matrix_scan_kb(void) {
	// put your looping keyboard code here
	// runs every cycle (a lot)

	matrix_scan_user();
}

#ifdef LED_MATRIX_ENABLE
    #include "is31fl3731-simple.h"

const is31_led g_is31_leds[LED_DRIVER_LED_COUNT] = {
/* Refer to IS31 manual for these locations
 *  driver
 *  |   LED address
 *  |   | */

    {0, C1_1},  // k00 KC_GESC
    {0, C1_2},  // k10 KC_1
    {0, C1_3},  // k20 KC_2
    {0, C1_4},  // k30 KC_3
    {0, C1_5},  // k40 KC_4
    {0, C1_6},  // k50 KC_5
    {0, C1_7},  // k60 KC_6
    {0, C1_8},  // k70 KC_7
    {0, C2_1},  // k80 KC_8
    {0, C2_2},  // k01 KC_9
    {0, C2_3},  // k11 KC_0
    {0, C2_4},  // k21 KC_MINS
    {0, C2_5},  // k31 KC_EQL
    {0, C2_6},  // k41 KC_BKSP
    {0, C2_7},  // k51 KC_**unused in alphabet layout
    {0, C2_8},  // k61 KC_TAB
    {0, C3_1},  // k71 KC_Q
    {0, C3_2},  // k81 KC_W
    {0, C3_3},  // k02 KC_E
    {0, C3_4},  // k12 KC_R
    {0, C3_5},  // k22 KC_T
    {0, C3_6},  // k32 KC_Y
    {0, C3_7},  // k42 KC_U
    {0, C3_8},  // k52 KC_I
    {0, C4_1},  // k62 KC_O
    {0, C4_2},  // k72 KC_P
    {0, C4_3},  // k82 KC_LBRC
    {0, C4_4},  // k03 KC_RBRC
    {0, C4_5},  // k13 KC_BSLS
    {0, C4_6},  // k23 KC_CAPS
    {0, C4_7},  // k33 KC_A
    {0, C4_8},  // k43 KC_S
    {0, C5_1},  // k53 KC_D
    {0, C5_2},  // k63 KC_F
    {0, C5_3},  // k73 KC_G
    {0, C5_4},  // k83 KC_H
    {0, C5_5},  // k04 KC_J
    {0, C5_6},  // k14 KC_K
    {0, C5_7},  // k24 KC_L
    {0, C5_8},  // k34 KC_SCLN
    {0, C6_1},  // k44 KC_QUOT
    {0, C6_2},  // k54 KC_ENT
    {0, C6_3},  // k64 KC_LSFT
    {0, C6_4},  // k74 KC_Z
    {0, C6_5},  // k84 KC_X
    {0, C6_6},  // k05 KC_C
    {0, C6_7},  // k15 KC_V
    {0, C6_8},  // k25 KC_B
    {0, C7_1},  // k35 KC_N
    {0, C7_2},  // k45 KC_M
    {0, C7_3},  // k55 KC_COMM
    {0, C7_4},  // k65 KC_DOT
    {0, C7_5},  // k75 KC_SLSH
    {0, C7_6},  // k85 KC_RSFT
    {0, C7_7},  // k06 KC_**unused in alphabet layout
    {0, C7_8},  // k16 KC_LCTL
    {0, C8_1},  // k26 KC_LGUI
    {0, C8_2},  // k36 KC_LALT
    {0, C8_3},  // k46 KC_SPC
    {0, C8_4},  // k56 KC_RALT
    {0, C8_5},  // k66 KC_GUI
    {0, C8_6},  // k76 KC_FN
    {0, C8_7},  // k86 KC_RCTL
};

#endif

