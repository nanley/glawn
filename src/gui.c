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

static pack *data;

void create_gui (pack *ui_data)
{
	data = ui_data;

	GtkBuilder * builder 	= gtk_builder_new_from_file("./gui/window.glade");
	GtkWidget * window 	= GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	data->nameEntry		= GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
	data->pwdEntry		= GTK_WIDGET(gtk_builder_get_object(builder, "entry2"));
	data->locCBox 		= GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton2"));
	data->issCBox 		= GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton1"));
	data->radio_this 	= GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton1"));
	data->radio_other = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
	data->macEntry 		= GTK_WIDGET(gtk_builder_get_object(builder, "entry3"));
	data->spinner 		= GTK_WIDGET(gtk_builder_get_object(builder, "spinner1"));
	data->status 		= GTK_WIDGET(gtk_builder_get_object(builder, "label4"));
	data->logout 		= GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
	data->login 		= GTK_WIDGET(gtk_builder_get_object(builder, "button2"));

	/* Connect signal handlers */
	gtk_builder_connect_signals(builder, data);

	/* Display window */
	gtk_widget_show_all (window);
}

pack *get_gui() {return data;}

void update_cmd (char *custom)
{
	if (curl_return == CURLE_OK) {
		g_printf ("libcurl output: :)\n");
		g_printf ("%s: %s\n\n", custom, buffer);
	} else {
		g_printf ("libcurl output: %s\n", curl_easy_strerror (curl_return));
		g_printf ("%s: No reply received\n\n", custom);
	}
}

void update_gui (enum _GUI_states gs)
{
	switch (gs) {
		case SPIN:
			gtk_widget_set_sensitive (data->spinner, TRUE);
			gtk_spinner_start (GTK_SPINNER(data->spinner));
			break;
		case SPINSTOP:
			gtk_spinner_stop (GTK_SPINNER(data->spinner));
			gtk_widget_set_sensitive (data->spinner, FALSE);
			break;
		case CHECK_STAT_START:
			update_gui (SPIN);
			gtk_label_set_markup ( GTK_LABEL(data->status), "<small>Checking your status..</small>");
			break;
		case CHECK_STAT_END:
			if (g_str_has_prefix (buffer, "logged_in") || g_str_has_prefix (buffer, "logging")) {

				/* Select to login other device, disable logging in this device */
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON  (data->radio_other), TRUE);
				gtk_widget_set_sensitive (data->radio_this, FALSE);

				/* Allow logging out */
				gtk_widget_set_sensitive (data->logout, TRUE);

				/* Don't allow changing location */
				gtk_widget_set_sensitive (data->locCBox, FALSE);
				gtk_label_set_markup ( GTK_LABEL(data->status), "<small>Logged in</small>");
			} else  {
				gtk_label_set_markup ( GTK_LABEL(data->status), "<small>Not logged in</small>");
				gtk_widget_set_sensitive (data->login, TRUE);
			}
			update_gui(SPINSTOP);
			break;
		case LOG_IN_END:
			if (g_str_has_prefix (buffer, "Already") || g_str_has_prefix (buffer, "Logging")) {
				gtk_label_set_markup (GTK_LABEL(data->status),
				"<span font_weight='bold' font_size='small' fgcolor='#3A6A3A'>Login successful</span>");
				gtk_entry_set_text (GTK_ENTRY(data->pwdEntry), "");

				/* Allow logging out */
				gtk_widget_set_sensitive (data->logout, TRUE);
				gtk_widget_set_sensitive (data->locCBox, FALSE);

			} else {
				gtk_label_set_markup (GTK_LABEL(data->status),
				"<span font_weight='bold' font_size='small' fgcolor='#632525'>Login failed</span>");
			}
			update_gui(SPINSTOP);
			break;
		case LOG_OUT_END:
			if (g_strrstr (buffer, ":: LAWN Logout") || g_strrstr (buffer, ":: LAWN :: Login")) {
				gtk_label_set_markup (GTK_LABEL(data->status),
				"<span font_weight='bold' font_size='small'>Logout successful</span>");
				gtk_widget_set_sensitive (data->logout, FALSE);
				gtk_widget_set_sensitive (data->login, TRUE);
				gtk_widget_set_sensitive (data->locCBox, TRUE);
			} else {
				gtk_label_set_markup (GTK_LABEL(data->status),
				"<span font_weight='bold' font_size='small'>Logout failed</span>");
			}
			update_gui(SPINSTOP);
			break;
		case LOAD_INI_START:
			update_gui (SPIN);
			int current = get_url_index();
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(data->locCBox), current);
			break;
	}
}
