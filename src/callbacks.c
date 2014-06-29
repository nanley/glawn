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


#include "main.h"


static CURL *curl;
int curl_return;

gchar *buffer;

static GMutex GTK_Mutex;


void show_pass (GtkEntry *entry,
		GtkEntryIconPosition icon_pos,
		GdkEventButton *event,
		gpointer user_data)
{
	int visibility = 0;
	if (event->type == GDK_BUTTON_PRESS)
		visibility = 1;
	gtk_entry_set_visibility(entry, visibility);
}


void mac_switch (GtkToggleButton *togglebutton,
		pack *user_data)
{
	int enabled = gtk_toggle_button_get_active(user_data->radio_other);
	gtk_widget_set_sensitive (data.macEntry, enabled);
}


void quit_glawn ()
{
	save_settings();
	/* Free pointers and exit */
	curl_easy_cleanup (curl);
	g_free (buffer);
	g_mutex_unlock(&GTK_Mutex);
	gtk_main_quit ();
}


size_t curl_callback (void *buffer, size_t size, size_t nmemb, void *userp)
{
	return g_strlcpy ((char *)userp, (char *)buffer, 160);
}





int init_check_status() {
	if (g_mutex_trylock(&GTK_Mutex)) {
		update_gui (CHECK_STAT_END);
		return FALSE;
	}
	return TRUE;
}

int check_status ()
{
	g_mutex_lock(&GTK_Mutex);
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
	g_mutex_unlock(&GTK_Mutex);
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
