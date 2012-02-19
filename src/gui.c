#include "callbacks.h"

extern pack data;
extern gchar *buffer;
extern int curl_return;
extern char location[34];

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
			gtk_widget_set_sensitive (data.spinner, TRUE);
			gtk_spinner_start ((GtkSpinner *) data.spinner);
			break;
		case SPINSTOP:
			gtk_spinner_stop ((GtkSpinner*)data.spinner);
			gtk_widget_set_sensitive (data.spinner, FALSE);
			break;
		case CHECK_STAT_START:
			update_gui (SPIN);
			gtk_label_set_markup ( (GtkLabel *)data.status, "<small>Checking your status..</small>");
			break;
		case CHECK_STAT_END: 
			if (g_str_has_prefix (buffer, "logged_in") || g_str_has_prefix (buffer, "logging")) {

				/* Select to login other device, disable logging in this device */
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON  (data.radio_other), TRUE);
				gtk_widget_set_sensitive (data.radio_this, FALSE);
				
				/* Allow logging out */
				gtk_widget_set_sensitive (data.logout, TRUE);
				gtk_label_set_markup ( (GtkLabel *)data.status, "<small>Logged in</small>");
			} else  {
				gtk_label_set_markup ( (GtkLabel *)data.status, "<small>Not logged in</small>");
				gtk_widget_set_sensitive (data.login, TRUE);			
			}
			update_gui(SPINSTOP);
			break;
		case LOG_IN_END:	
			if (g_str_has_prefix (buffer, "Already") || g_str_has_prefix (buffer, "Logging")) {
				gtk_label_set_markup ((GtkLabel *) data.status,
				"<span font_weight='bold' font_size='small' fgcolor='#3A6A3A'>Login successful</span>");
				gtk_entry_set_text ((GtkEntry *) data.pwdEntry, "");
				gtk_widget_set_sensitive (data.logout, TRUE);
				gtk_toggle_button_set_active ((GtkToggleButton *) data.pwdCBox, FALSE);
			} else {
				gtk_label_set_markup ((GtkLabel *) data.status,
				"<span font_weight='bold' font_size='small' fgcolor='#632525'>Login failed</span>");
			}
			update_gui(SPINSTOP);
			break;
		case LOG_OUT_END:	
			if (g_strrstr (buffer, ":: LAWN Logout") || g_strrstr (buffer, ":: LAWN :: Login")) {
				gtk_label_set_markup ((GtkLabel *) data.status,
				"<span font_weight='bold' font_size='small'>Logout successful</span>");
				gtk_widget_set_sensitive (data.logout, FALSE);
				gtk_widget_set_sensitive (data.login, TRUE);
			} else {
				gtk_label_set_markup ((GtkLabel *) data.status,
				"<span font_weight='bold' font_size='small'>Logout failed</span>");
			}
			update_gui(SPINSTOP);
			break;
		case LOAD_INI_START:
			update_gui (SPIN);
			if (!g_strcmp0 (location, "https://mower.georgiatech-metz.fr/"))
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(data.locCBox), TRUE);
			break;
	}
}
