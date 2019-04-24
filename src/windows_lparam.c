/* Copyright 2019 Dietrich Epp <depp@zdome.net>
   This file is licensed under the terms of the MIT license. See LICENSE.txt
   for details. */
#include "keytable.h"

unsigned keycode_windows_from_lparam(unsigned lparam) {
    return ((lparam >> 16) & 0x7f) | ((lparam & (1 << 24)) != 0 ? 0x80 : 0);
}
