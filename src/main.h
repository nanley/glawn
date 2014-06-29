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
#include "callbacks.h"
#include "settings.h"
#include "gui.h"

typedef struct gpointer {
	GtkWidget *nameEntry;
	GtkWidget *pwdEntry;
	GtkWidget *issCBox;
	GtkWidget *locCBox;
	GtkWidget *macEntry;
	GtkWidget *status;
	GtkWidget *spinner;
	GtkWidget *login;
	GtkWidget *logout;
	GtkWidget *radio_this;
	GtkToggleButton *radio_other;
} pack;

extern pack data;
