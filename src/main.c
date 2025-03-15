/*
 *  Leafpad - GTK+ based simple text editor
 *  Copyright (C) 2004-2005 Tarot Osuji
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define GLOBAL_VARIABLE_DEFINE
#include "leafpad.h"
#include <glib/gstdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	gint width;
	gint height;
	gchar *fontname;
	gboolean wordwrap;
	gboolean linenumbers;
	gboolean autoindent;
} Conf;

static void load_config_file(Conf *conf)
{
	FILE *fp;
	gchar *path;
	gchar buf[BUFSIZ];
	gchar **num;
	
	path = g_build_filename(g_get_user_config_dir(),
	    PACKAGE, PACKAGE "rc", NULL);
	fp = fopen(path, "r");
	g_free(path);
	if (!fp)
		return;

	if (fgets(buf, sizeof(buf), fp)) {
		num = g_strsplit(buf, "." , 3);
		if ((atoi(num[1]) >= 8) && (atoi(num[2]) >= 0)) {
			fgets(buf, sizeof(buf), fp);
			conf->width = atoi(buf);
			fgets(buf, sizeof(buf), fp);
			conf->height = atoi(buf);
			fgets(buf, sizeof(buf), fp);
			g_free(conf->fontname);
			conf->fontname = g_strdup(buf);
			fgets(buf, sizeof(buf), fp);
			conf->wordwrap = atoi(buf);
			fgets(buf, sizeof(buf), fp);
			conf->linenumbers = atoi(buf);
			fgets(buf, sizeof(buf), fp);
			conf->autoindent = atoi(buf);
		}
		g_strfreev(num);
	}
	fclose(fp);
}

void save_config_file(void)
{
	FILE *fp;
	gchar *path;
	GtkItemFactory *ifactory;
	gint width, height;
	gchar *fontname;
	gboolean wordwrap, linenumbers, autoindent;
	
	gtk_window_get_size(GTK_WINDOW(pub->mw->window), &width, &height);
	fontname = get_font_name_from_widget(pub->mw->view);
	ifactory = gtk_item_factory_from_widget(pub->mw->menubar);
	wordwrap = gtk_check_menu_item_get_active(
		GTK_CHECK_MENU_ITEM(gtk_item_factory_get_item(ifactory,
			"/Options/Word Wrap")));
	linenumbers = gtk_check_menu_item_get_active(
		GTK_CHECK_MENU_ITEM(gtk_item_factory_get_item(ifactory,
			"/Options/Line Numbers")));
	autoindent = gtk_check_menu_item_get_active(
		GTK_CHECK_MENU_ITEM(gtk_item_factory_get_item(ifactory,
			"/Options/Auto Indent")));
	
	path = g_build_filename(g_get_user_config_dir(), PACKAGE, NULL);
	if (!g_file_test(path, G_FILE_TEST_IS_DIR)) {
		g_mkdir_with_parents(path, 0700);
	}
	g_free(path);
	path = g_build_filename(g_get_user_config_dir(),
	    PACKAGE, PACKAGE "rc", NULL);
	fp = fopen(path, "w");
	if (!fp) {
		g_print("%s: can't save config file - %s\n", PACKAGE, path);
		return;
	}
	g_free(path);
	
	fprintf(fp, "%s\n", PACKAGE_VERSION);
	fprintf(fp, "%d\n", width);
	fprintf(fp, "%d\n", height);
	fprintf(fp, "%s\n", fontname);
	fprintf(fp, "%d\n", wordwrap);
	fprintf(fp, "%d\n", linenumbers);
	fprintf(fp, "%d\n", autoindent);
	fclose(fp);
	
	g_free(fontname);
}

gint jump_linenum = 0;

static void parse_args(gint argc, gchar **argv, FileInfo *fi)
{
	EncArray *encarray;
	gint i;
	GError *error = NULL;
	
	GOptionContext *context;
	gchar *opt_codeset = NULL;
	gint opt_tab_width = 0;
	gboolean opt_jump = 0;
	gboolean opt_version = FALSE;
	GOptionEntry entries[] = 
	{
		{ "codeset", 0, 0, G_OPTION_ARG_STRING, &opt_codeset, "Set codeset to open file", "CODESET" },
		{ "tab-width", 0, 0, G_OPTION_ARG_INT, &opt_tab_width, "Set tab width", "WIDTH" },
		{ "jump", 0, 0, G_OPTION_ARG_INT, &opt_jump, "Jump to specified line", "LINENUM" },
		{ "version", 0, 0, G_OPTION_ARG_NONE, &opt_version, "Show version number", NULL },
		{ NULL }
	};
	
	context = g_option_context_new("[filename]");
	g_option_context_add_main_entries(context, entries, PACKAGE);
	g_option_context_add_group(context, gtk_get_option_group(TRUE));
	g_option_context_set_ignore_unknown_options(context, FALSE);
	g_option_context_parse(context, &argc, &argv, &error);
	g_option_context_free(context);
	
	if (error) {
		g_print("%s: %s\n", PACKAGE, error->message);
		g_error_free(error);
		exit(-1);
	}
	if (opt_version) {
		g_print("%s\n", PACKAGE_STRING);
		exit(0);
	}
	if (opt_codeset) {
		g_convert("TEST", -1, "UTF-8", opt_codeset, NULL, NULL, &error);
		if (error) {
			g_error_free(error);
			error = NULL;
		} else {
			g_free(fi->charset);
			fi->charset = g_strdup(opt_codeset);
		}
	}
	if (opt_tab_width)
		indent_set_default_tab_width(opt_tab_width);
	if (opt_jump)
		jump_linenum = opt_jump;
	
	if (fi->charset 
		&& (g_strcasecmp(fi->charset, get_default_charset()) != 0)
		&& (g_strcasecmp(fi->charset, "UTF-8") != 0)) {
		encarray = get_encoding_items(get_encoding_code());
		for (i = 0; i < ENCODING_MAX_ITEM_NUM; i++)
			if (encarray->item[i])
				if (g_strcasecmp(fi->charset, encarray->item[i]) == 0)
					break;
		if (i == ENCODING_MAX_ITEM_NUM)
			fi->charset_flag = TRUE;
	}
	
	if (argc >= 2)
		fi->filename = parse_file_uri(argv[1]);
}

gint main(gint argc, gchar **argv)
{
	Conf *conf;
	GtkItemFactory *ifactory;
	gchar *stdin_data = NULL;
	
	bindtextdomain(PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(PACKAGE, "UTF-8");
	textdomain(PACKAGE);
	
	pub = g_malloc(sizeof(PublicData));
	pub->fi = g_malloc(sizeof(FileInfo));
	pub->fi->filename     = NULL;
	pub->fi->charset      = NULL;
	pub->fi->charset_flag = FALSE;
	pub->fi->lineend      = LF;
	
	parse_args(argc, argv, pub->fi);
	
	gtk_init(&argc, &argv);
	g_set_application_name(PACKAGE_NAME);
	
	pub->mw = create_main_window();
	
	conf = g_malloc(sizeof(Conf));
	conf->width       = 600;
	conf->height      = 400;
	conf->fontname    = g_strdup("Monospace 12");
	conf->wordwrap    = FALSE;
	conf->linenumbers = FALSE;
	conf->autoindent  = FALSE;
	
	load_config_file(conf);
	
	gtk_window_set_default_size(
		GTK_WINDOW(pub->mw->window), conf->width, conf->height);
	set_text_font_by_name(pub->mw->view, conf->fontname);
	
	ifactory = gtk_item_factory_from_widget(pub->mw->menubar);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(
		gtk_item_factory_get_widget(ifactory, "/Options/Word Wrap")),
		conf->wordwrap);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(
		gtk_item_factory_get_widget(ifactory, "/Options/Line Numbers")),
		conf->linenumbers);
	indent_refresh_tab_width(pub->mw->view);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(
		gtk_item_factory_get_widget(ifactory, "/Options/Auto Indent")),
		conf->autoindent);
	
	gtk_widget_show_all(pub->mw->window);
	g_free(conf->fontname);
	g_free(conf);
	
#ifdef ENABLE_EMACS
	check_emacs_key_theme(GTK_WINDOW(pub->mw->window), ifactory);
#endif
	
	hlight_init(pub->mw->buffer);
	undo_init(pub->mw->view,
		gtk_item_factory_get_widget(ifactory, "/Edit/Undo"),
		gtk_item_factory_get_widget(ifactory, "/Edit/Redo"));
//	hlight_init(pub->mw->buffer);
	
	if (pub->fi->filename)
		file_open_real(pub->mw->view, pub->fi);
#ifdef G_OS_UNIX
	else
		stdin_data = gedit_utils_get_stdin();
#endif
	if (stdin_data) {
		gchar *str;
		GtkTextIter iter;
		
		str = g_convert(stdin_data, -1, "UTF-8",
			get_default_charset(), NULL, NULL, NULL);
		g_free(stdin_data);
	
//		gtk_text_buffer_set_text(buffer, "", 0);
		gtk_text_buffer_get_start_iter(pub->mw->buffer, &iter);
		gtk_text_buffer_insert(pub->mw->buffer, &iter, str, strlen(str));
		gtk_text_buffer_get_start_iter(pub->mw->buffer, &iter);
		gtk_text_buffer_place_cursor(pub->mw->buffer, &iter);
		gtk_text_buffer_set_modified(pub->mw->buffer, FALSE);
		gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(pub->mw->view), &iter, 0, FALSE, 0, 0);
		g_free(str);
	}
	
	if (jump_linenum) {
		GtkTextIter iter;
		
		gtk_text_buffer_get_iter_at_line(pub->mw->buffer, &iter, jump_linenum - 1);
		gtk_text_buffer_place_cursor(pub->mw->buffer, &iter);
//		gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(textview), &iter, 0.1, FALSE, 0.5, 0.5);
		scroll_to_cursor(pub->mw->buffer, 0.25);
	}
	
	set_main_window_title();
//	hlight_apply_all(pub->mw->buffer);
	
	gtk_main();
	
	return 0;
}
