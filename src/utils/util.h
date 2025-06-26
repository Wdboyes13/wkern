/*
WKern - A Bare Metal OS / Kernel I am making (For Fun)
Copyright (C) 2025  Wdboyes13

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <types/nums.h>

int kstrcmp(const char *a, const char *b);
void kshcmp();
char *kstrchr(const char *str, int c);
int ktolower(int c);
int kisdigit(int c);
int kisspace(int c);
int kisxdigit(int c);
size_t kstrlen(const char *str);
int split(char *input, char **argv, int max_args);

void ZZZ(int ms);

int katoi(const char *str);
int katoi_auto(const char *str);
void kitoa(unsigned int num, char *buf);

void draw_smile();

u32 kmstrlen(const char *str);