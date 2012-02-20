/*
    This file is part of Glawn.

    Copyright (C) 2010-2012  Nanley Chery <nanleychery@gmail.com>

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

//#define DEBUG


#include <gtk/gtk.h>
#include <glib/gprintf.h> // only call toplevel
#include <glib/gstdio.h>
#include <curl/curl.h>


typedef struct gpointer {
	GtkWidget *nameEntry;
	GtkWidget *pwdEntry;
	GtkWidget *issCBox;
	GtkWidget *pwdCBox;
	GtkWidget *locCBox;
	GtkWidget *macEntry;
	GtkWidget *status;
	GtkWidget *spinner;
	GtkWidget *login;
	GtkWidget *logout;
	GtkWidget *radio_this;
	GtkWidget *radio_other;
} pack;

enum _GUI_states {SPIN, SPINSTOP, LOAD_INI_START, CHECK_STAT_START, CHECK_STAT_END, LOG_IN_END, LOG_OUT_END };

/* Callback function prototypes */
void mac_switch (GtkWidget *widget, int boolean);
void show_pass (GtkToggleButton *pass_cbox, GtkEntry *pass_text);
void quit_glawn ();
void set_url ();
void login ();
void logout (GtkWidget *widget, GtkWidget *button);
size_t curl_callback (void *buffer, size_t size, size_t nmemb, void *userp);

/* Other functions */
void load_settings ();
int check_status ();
void update_cmd (char *);
void update_gui (enum _GUI_states gs);
int init_check_status();
void init_mutex();
