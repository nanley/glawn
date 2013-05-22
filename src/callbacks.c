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
#define SETTINGS_FILE "./config/settings.ini"
extern pack data;
GKeyFile *settings;

static CURL *curl;
int curl_return;

gchar *buffer;
gchar **locs;

static GMutex * GTK_Mutex = NULL;


void show_pass (GtkToggleButton *pass_cbox, GtkEntry *pass_text)
{
	gtk_entry_set_visibility(pass_text, gtk_toggle_button_get_active (pass_cbox));
}


void mac_switch (GtkWidget *widget, int Boolean)
{
	gtk_widget_set_sensitive (data.macEntry, Boolean);
}


void quit_glawn ()
{
	/* Write last location to settings file */
	gchar *settings_data = g_key_file_to_data(settings, NULL, NULL);
	g_file_set_contents (SETTINGS_FILE, settings_data, -1, NULL);
	g_key_file_free(settings);
	g_strfreev(locs);

	/* Free pointers and exit */
	curl_easy_cleanup (curl);
	g_free (buffer);
	g_mutex_unlock(GTK_Mutex);
	g_mutex_free(GTK_Mutex);
	gtk_main_quit ();
}


size_t curl_callback (void *buffer, size_t size, size_t nmemb, void *userp)
{
	return g_strlcpy ((char *)userp, (char *)buffer, 160);
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



void load_settings ()
{
	/* Load location from settings file */
	settings = g_key_file_new();
	if (!g_key_file_load_from_file(settings, SETTINGS_FILE, G_KEY_FILE_NONE, NULL)) {
		fprintf(stderr, "Can't find settings file: %s\n", SETTINGS_FILE);
		exit(EXIT_FAILURE);
	}
	g_key_file_set_list_separator(settings, ';');
	locs = g_key_file_get_string_list(settings, "config", "sites", NULL, NULL);
	update_gui (LOAD_INI_START);
}


void init_mutex() {
	g_assert (GTK_Mutex == NULL);
	GTK_Mutex = g_mutex_new();
}

int init_check_status() {
	if (g_mutex_trylock(GTK_Mutex)) {
		update_gui (CHECK_STAT_END);
		return FALSE;
	}
	return TRUE;
}

int check_status ()
{
	g_mutex_lock(GTK_Mutex);
	buffer = g_realloc(buffer, 160);

	// initialize curl library 
	curl = curl_easy_init ();
	curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, curl_callback);
	curl_easy_setopt (curl, CURLOPT_WRITEDATA, buffer);
	curl_easy_setopt (curl, CURLOPT_CAINFO, "./config/cacert.pem");
	curl_easy_setopt (curl, CURLOPT_TIMEOUT, 5);

	g_printf ("Connecting to : %s\n", get_url());

	// setup and send status request
	g_sprintf (buffer, "%slogin_status.php", get_url());
	curl_easy_setopt (curl, CURLOPT_URL, buffer);
	curl_easy_setopt (curl, CURLOPT_HTTPGET, 1L);
	curl_return = curl_easy_perform (curl);
	
	// update status in GUI and CMD 
	update_cmd("Initial status");
	g_mutex_unlock(GTK_Mutex);
	return FALSE;
}




void login ()
{
	// create POST string from GUI widgets
	char *packet = g_strdup_printf ("username=%s&password=%s%s%s%s&output=text",
		gtk_entry_get_text (GTK_ENTRY(data.nameEntry)),
		gtk_entry_get_text (GTK_ENTRY(data.pwdEntry)), 
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(data.issCBox)) ? "" : "&iss=false",
		gtk_widget_is_sensitive (data.macEntry) ? "&mac=" : "",
		gtk_entry_get_text (GTK_ENTRY(data.macEntry)));

	// update label
	gtk_label_set_markup ( GTK_LABEL(data.status), "<small>Logging you in..</small>");
	update_gui (SPIN);
	
#ifndef DEBUG
	g_printf ("HTTP POST strg: [Enable debug mode to display sensitive information.]\n");
#else
	g_printf ("HTTP POST strg: %s\n", packet);
#endif

	// setup and send HTTP POST
	g_sprintf (buffer, "%sindex.php", get_url());
	curl_easy_setopt (curl, CURLOPT_URL, buffer);
	curl_easy_setopt (curl, CURLOPT_POSTFIELDS, packet);
	curl_return = curl_easy_perform (curl);

	// update status in GUI and CMD 
	update_cmd("Inbound packet");
	update_gui (LOG_IN_END);
	
	// free allocated string
	g_free (packet);
}




void logout (GtkWidget *widget, GtkWidget *button)
{
	// update label
	gtk_label_set_markup ( GTK_LABEL(data.status), "<small>Logging you out..</small>");
	update_gui (SPIN);

	// setup and send logout request
	g_sprintf (buffer, "%slogout.php?output=text", get_url());
	curl_easy_setopt (curl, CURLOPT_URL, buffer);
	curl_easy_setopt (curl, CURLOPT_HTTPGET, 1L);
	curl_return = curl_easy_perform (curl);

	// update status in GUI and CMD
	update_cmd("Logout result");
	update_gui (LOG_OUT_END);
}
