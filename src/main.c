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

pack data;

int main (int argc, char *argv[])
{
	/* GUI widgets */
	GtkWidget *window;
	GtkWidget *align;
	GtkWidget *expander;
	GtkWidget *label;
	GtkWidget *radio1, *radio2;
	GtkWidget *check;
	GtkWidget *button;
	GtkWidget *entry;
	GtkTooltips *tooltip;

	/* Packing boxes */
	GtkWidget *vbMain;			/* main vertical box */
	GtkWidget *hbox1, *vbox1, *vbox2;	/* username/password */
	GtkWidget *hbox2, *vbox3, *hbMac;	/* login options */
	GtkWidget *hbox3;			/* cancel/login buttons */
	GtkWidget *hbox9;			/* show pwd & gtl */

	/* Initialize multihreading and gtk functions */
	g_thread_init (NULL);
	gdk_threads_init ();
	gdk_threads_enter ();
	gtk_init (&argc, &argv);

	/* Create main window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(window), "Glawn");
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	g_signal_connect (window, "destroy", G_CALLBACK(quit_glawn), NULL);
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file ("./icons/glawn-icon.png", NULL);
  	gtk_window_set_icon (GTK_WINDOW(window), pixbuf);

	/* Create pack boxes */
	vbMain= gtk_vbox_new (FALSE, 3);
	vbox1 = gtk_vbox_new (FALSE, 0);
	vbox2 = gtk_vbox_new (FALSE, 3);
	vbox3 = gtk_vbox_new (FALSE, 0);
	hbox1 = gtk_hbox_new (FALSE, 0);
	hbox2 = gtk_hbox_new (FALSE, 0);
	hbox3 = gtk_hbox_new (FALSE, 0);
	hbMac = gtk_hbox_new (FALSE, 0);
	hbox9 = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), vbMain);

	/* Username entry label */
	label = gtk_label_new ("GT Account : ");
	gtk_misc_set_alignment ((GtkMisc *)label, 0, 0);
	gtk_box_pack_start (GTK_BOX(vbox1), label, TRUE, FALSE, 0);

	/* Password entry label */
	label = gtk_label_new ("Password : ");
	gtk_misc_set_alignment ((GtkMisc *)label, 0, 0);
	gtk_box_pack_start (GTK_BOX(vbox1), label, TRUE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox1), vbox1, FALSE, FALSE, 0);

	/* Blank text for alignment */
	label = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX(vbox1),label,FALSE, FALSE, 2);

	/* Username entry field */
	data.nameEntry = entry = gtk_entry_new_with_max_length (50);
	gtk_entry_set_width_chars (GTK_ENTRY(entry), 25);
	g_signal_connect (entry, "activate", G_CALLBACK(login), NULL);
	gtk_box_pack_start (GTK_BOX(vbox2), entry, FALSE, FALSE, 0);

	/* Password entry field */
	data.pwdEntry = entry = gtk_entry_new_with_max_length(50);
	gtk_entry_set_visibility (GTK_ENTRY(entry), FALSE);
	g_signal_connect (entry, "activate", G_CALLBACK(login), NULL);
	gtk_box_pack_start (GTK_BOX(vbox2), entry, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox1), vbox2, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX(vbMain), hbox1, FALSE, FALSE, 0);

	/* Show password checkbox */
	data.pwdCBox = check = gtk_check_button_new_with_mnemonic ("_Show password");
	g_signal_connect (check, "toggled", G_CALLBACK(show_pass), GTK_ENTRY(entry));
	gtk_box_pack_start (GTK_BOX(hbox9), check, FALSE, FALSE, 0);

	/* Location checkbox */
	data.locCBox = check = gtk_check_button_new_with_mnemonic ("_GTL");
	tooltip = gtk_tooltips_new ();
	gtk_tooltips_set_tip (tooltip, check, "Enable Georgia Tech-Lorraine support", NULL);
	gtk_box_pack_start (GTK_BOX(hbox9), check, FALSE, FALSE, 9);
	gtk_box_pack_start (GTK_BOX(vbox2), hbox9, FALSE, FALSE, 0);

	/* Options expander */
	expander = gtk_expander_new_with_mnemonic ("_Options");
	gtk_box_pack_start (GTK_BOX(vbMain), expander, FALSE, FALSE, 0);
	gtk_container_add (GTK_CONTAINER(expander), hbox2);

	/* Left indent alignment for expander */
	align = gtk_alignment_new (0, 0, 1 , 1);
	gtk_widget_set_size_request (align, 17, 0);
	gtk_box_pack_start (GTK_BOX(hbox2), align, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox2), vbox3, FALSE, FALSE, 0);

	/* ISS checkbox */
	data.issCBox = check = gtk_check_button_new_with_mnemonic
				("_Enable Inbound Service Security");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(check), TRUE);
	gtk_box_pack_start (GTK_BOX(vbox3), check, FALSE, FALSE, 0);
	tooltip = gtk_tooltips_new ();
	gtk_tooltips_set_tip (tooltip, check, "When turned on,"
	" devices outside of the LAWN network are blocked from "
	"connecting to services running on your device.", NULL);

	/* Login this device radio */
	data.radio_this = radio1 = gtk_radio_button_new_with_mnemonic_from_widget
					(NULL, "Lo_gin this device");
	g_signal_connect (radio1, "toggled", G_CALLBACK(mac_switch), (gpointer) FALSE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(radio1), TRUE);
	gtk_box_pack_start (GTK_BOX(vbox3), radio1, FALSE, FALSE, 0);

	/* Login other device radio */
	data.radio_other = radio2 = gtk_radio_button_new_with_mnemonic_from_widget
		(GTK_RADIO_BUTTON(radio1), "Login a _different device");
	g_signal_connect (radio2, "toggled", G_CALLBACK(mac_switch), (gpointer) TRUE);
	gtk_box_pack_start (GTK_BOX(vbox3), radio2, FALSE, FALSE, 0);
	gtk_tooltips_set_tip (tooltip, radio2, "You can authenticate other"
	" devices currently on LAWN. The login will last as long as the"
	" device maintains a valid DHCP lease. You accept responsibility"
	" for all usage associated with this device.", NULL);

	/* MAC address label and entry */
	label = gtk_label_new ("        MAC Address : "); // more elegance
	data.macEntry = entry = gtk_entry_new_with_max_length (12);
	gtk_entry_set_width_chars (GTK_ENTRY(entry), 18);
	gtk_widget_set_sensitive (entry, FALSE);
	gtk_box_pack_start (GTK_BOX(hbMac), label, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbMac), entry, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX(vbox3), hbMac, FALSE, FALSE, 0);

	/* Login button */
	data.login = button = gtk_button_new_with_mnemonic (" _Login ");
	gtk_widget_set_sensitive (button, FALSE);
	g_signal_connect (button, "clicked", G_CALLBACK(login), NULL);
	gtk_box_pack_end (GTK_BOX(hbox3), button, FALSE, FALSE, 5);

	/* Logout button */
	data.logout = button = gtk_button_new_with_mnemonic ("Logou_t");
        gtk_widget_set_sensitive (button, FALSE);
	g_signal_connect (button, "clicked", G_CALLBACK(logout), NULL);
	gtk_box_pack_end (GTK_BOX(hbox3), button, FALSE, FALSE, 0);

	/* Spinner */
	data.spinner = gtk_spinner_new ();
	gtk_box_pack_start (GTK_BOX(hbox3), data.spinner, FALSE, FALSE, 2);

	/* Statusbar */
	data.status = label = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX(hbox3), label, FALSE, FALSE, 2);
	gtk_box_pack_start (GTK_BOX(vbMain), hbox3, FALSE, FALSE, 0);

	/* Display window */
	gtk_widget_show_all (window);

	/* Ascii art */
	g_printf ("\n===========================\n"
		  "     Welcome to Glawn!\n"
		  "===========================\n\n");

	/*Load_settings */
	init_mutex();
	load_settings();
	update_gui (LOAD_INI_START);
	
	// send curl request for status
	update_gui (CHECK_STAT_START);
	g_thread_create((GThreadFunc)check_status, NULL, FALSE, NULL);
	g_timeout_add_seconds_full(G_PRIORITY_LOW, 1, init_check_status, NULL, NULL);
	
   	/* Start main loop */
	gtk_main ();
	gdk_threads_leave ();

	return 0;
}
