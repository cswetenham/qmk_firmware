/* Copyright 2020 Iris Swetenham
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

#pragma once

#include "quantum.h"
// readability
#define ___ KC_NO

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
  k01, k02, k03, k04, k05,    k35, k34, k33, k32, k31, \
  k11, k12, k13, k14, k15,    k45, k44, k43, k42, k41, \
  k21, k22, k23, k24, k25,    k55, k54, k53, k52, k51, \
            k06, k16, k26,    k56, k46, k36            \
  ) \
  { \
    { k01, k02, k03, k04, k05, k06 }, \
    { k11, k12, k13, k14, k15, k16 }, \
    { k21, k22, k23, k24, k25, k26 }, \
    { k31, k32, k33, k34, k35, k36 }, \
    { k41, k42, k43, k44, k45, k46 }, \
    { k51, k52, k53, k54, k55, k56 }, \
  }
