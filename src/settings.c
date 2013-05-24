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


GKeyFile *settings;
static gchar **locs;
extern pack data;

void load_settings ()
{
	settings = g_key_file_new();
	g_key_file_set_list_separator(settings, ';');
	if (!g_key_file_load_from_file(settings, SETTINGS_FILE, G_KEY_FILE_NONE, NULL)) {
		fprintf(stderr, "Can't find settings file: %s\n", SETTINGS_FILE);
		exit(EXIT_FAILURE);
	}

	/* Load location from settings file */
	locs = g_key_file_get_string_list(settings, "config", "sites", NULL, NULL);

	update_gui (LOAD_INI_START);
}

void save_settings ()
{
	/* Write last location to settings file */
	gchar *settings_data = g_key_file_to_data(settings, NULL, NULL);
	g_file_set_contents (SETTINGS_FILE, settings_data, -1, NULL);

	/* Free up pointers */
	g_key_file_free(settings);
	g_strfreev(locs);
}

void set_url ()
{
	int index = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(data.locCBox));
	g_key_file_set_integer(settings, "config", "current", index);
}


gchar *get_url ()
{
	int index = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(data.locCBox));
	return locs[index];
}

