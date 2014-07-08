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

int main (int argc, char *argv[])
{
	/* Initialize gtk functions */
	gtk_init (&argc, &argv);

	/* Create main window */
	pack data;
	create_gui(&data);


	/* Ascii art */
	g_printf ("\n===========================\n"
		  "     Welcome to Glawn!\n"
		  "===========================\n\n");

	/*Load_settings */
	load_settings();
	update_gui (LOAD_INI_START);
	
	// send curl request for status
	update_gui (CHECK_STAT_START);
	g_thread_new("Check status", (GThreadFunc)check_status, NULL);
	g_timeout_add_seconds_full(G_PRIORITY_LOW, 1, init_check_status, NULL, NULL);
	
   	/* Start main loop */
	gtk_main ();

	return 0;
}
