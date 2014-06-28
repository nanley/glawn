/*
    This file is part of Glawn.

    Copyright (C) 2010-2014  Nanley Chery <nanleychery@gmail.com>

    Glawn is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Glawn is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
enum _GUI_states {SPIN, SPINSTOP, LOAD_INI_START, CHECK_STAT_START, CHECK_STAT_END, LOG_IN_END, LOG_OUT_END };

/* Function Prototypes */
void update_cmd (char *);
void update_gui (enum _GUI_states gs);
