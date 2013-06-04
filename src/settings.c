/*
    This file is part of Glawn.

    Copyright (C) 2010-2013  Nanley Chery <nanleychery@gmail.com>

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

#include "callbacks.h"
#include "settings.h"

#define SETTINGS_FILE "./config/settings.ini"

static GKeyFile *settings;
extern pack data;

void load_settings ()
{
	settings = g_key_file_new();
	if (!g_key_file_load_from_file(settings, SETTINGS_FILE, G_KEY_FILE_KEEP_COMMENTS, NULL)) {
		fprintf(stderr, "Can't find settings file: %s\n", SETTINGS_FILE);
		exit(EXIT_FAILURE);
	}
}

void save_settings ()
{
	/* Write last location to settings file */
	gchar *settings_data = g_key_file_to_data(settings, NULL, NULL);
	g_file_set_contents (SETTINGS_FILE, settings_data, -1, NULL);

	/* Free up pointers */
	g_key_file_free(settings);
}

void set_url ()
{
	int index = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(data.locCBox));
	g_key_file_set_integer(settings, "config", "current", index);
}


gchar *get_url ()
{
	static gchar *locs[] = {
		"https://auth.lawn.gatech.edu/",
		"https://mower.georgiatech-metz.fr/"
	};
	int index = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(data.locCBox));
	return locs[index];
}

int get_url_index ()
{
	return g_key_file_get_integer(settings, "config", "current", NULL);
}
