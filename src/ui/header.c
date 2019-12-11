/*
 * Copyright (C) 2017-2019 Martin Larralde <martin.larralde@ens-paris-saclay.fr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


static void torrent_page_set_title(TorrentPage* page, char* title) {
  g_debug("Setting page title: %s", title);
  gtk_label_set_text(GTK_LABEL(page->title), title);
}


static void torrent_page_set_leechers_value(TorrentPage* page, int leechers) {
  if (leechers == -1) {
    gtk_label_set_markup(GTK_LABEL(page->leechers), "<span foreground='red'><b>N/A</b></span>");
  } else {
    char *markup = g_strdup_printf ("<b><span foreground='red'>%i</span></b>", leechers);
    gtk_label_set_markup (GTK_LABEL (page->leechers), markup);
    g_free (markup);
  }
};


static void torrent_page_set_seeders_value(TorrentPage* page, int seeders) {
  if (seeders == -1) {
    gtk_label_set_markup(GTK_LABEL(page->seeders), "<span foreground='green'><b>N/A</b></span>");
  } else {
    char *markup = g_strdup_printf ("<b><span foreground='green'>%i</span></b>", seeders);
    gtk_label_set_markup (GTK_LABEL (page->seeders), markup);
    g_free (markup);
  }
};


static void torrent_page_update_torrent_status(TorrentPage* page) {
  gtk_widget_set_sensitive(page->refresh, FALSE);

  ThunarxFileInfo* info = torrent_page_get_file(page);
  TorrentInfo* torrent_info = torrent_info_from_thunarx_file_info(info);

  TorrentStatus* status = torrent_status_from_thunarx_file_info(THUNARX_FILE_INFO(info));
  torrent_page_set_seeders_value(TORRENT_PAGE(page), status->seeders);
  torrent_page_set_leechers_value(TORRENT_PAGE(page), status->leechers);
  torrent_status_delete(status);

  gtk_widget_set_sensitive(page->refresh, TRUE);
}


static void torrent_page_on_refresh_clicked(GtkButton *button, gpointer callback_data) {
  torrent_page_update_torrent_status((TorrentPage*) callback_data);
}


static GtkWidget* torrent_page_new_header(TorrentPage* page) {

  GtkWidget* header_hbox    = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  GtkWidget* stats_grid     = gtk_grid_new();
  GtkWidget* header_title   = gtk_label_new("Torrent name");
  GtkWidget* header_image   = gtk_image_new_from_icon_name("application-torrent", GTK_ICON_SIZE_DIALOG);
  GtkWidget* seeders_label  = gtk_label_new("<b>Seeders: </b>");
  GtkWidget* leechers_label = gtk_label_new("<b>Leechers: </b>");
  GtkWidget* seeders_value  = gtk_label_new("<span foreground='green'><b>N/A</b></span>");
  GtkWidget* leechers_value = gtk_label_new("<span foreground='red'><b>N/A</b></span>");
  GtkWidget* refresh_button = gtk_button_new_from_icon_name("view-refresh", GTK_ICON_SIZE_BUTTON);

  gtk_button_set_label (GTK_BUTTON(refresh_button), "Refresh");
  g_signal_connect(G_OBJECT(refresh_button), "clicked", G_CALLBACK(torrent_page_on_refresh_clicked), page);

  gtk_label_set_use_markup(GTK_LABEL(seeders_label),  TRUE);
  gtk_label_set_use_markup(GTK_LABEL(leechers_label), TRUE);
  gtk_label_set_use_markup(GTK_LABEL(seeders_value),  TRUE);
  gtk_label_set_use_markup(GTK_LABEL(leechers_value), TRUE);

  gtk_widget_set_halign(stats_grid, 0.0f);
  gtk_widget_set_valign(stats_grid, 0.5f);

  gtk_grid_attach(GTK_GRID(stats_grid), header_image, 0, 0, 1, 2);
  gtk_grid_attach_next_to(GTK_GRID(stats_grid), seeders_label, header_image, GTK_POS_RIGHT, 1, 1);
  gtk_grid_attach_next_to(GTK_GRID(stats_grid), seeders_value, seeders_label, GTK_POS_RIGHT, 1, 1);
  gtk_grid_attach_next_to(GTK_GRID(stats_grid), leechers_label, seeders_label, GTK_POS_BOTTOM, 1, 1);
  gtk_grid_attach_next_to(GTK_GRID(stats_grid), leechers_value, leechers_label, GTK_POS_RIGHT, 1, 1);

  gtk_box_pack_start(GTK_BOX(header_hbox), stats_grid,      TRUE,  TRUE, 0);
  gtk_box_pack_end(GTK_BOX(header_hbox), refresh_button,    FALSE, TRUE, 0);

  page->title    = header_title;
  page->refresh  = refresh_button;
  page->seeders  = seeders_value;
  page->leechers = leechers_value;

  return header_hbox;
}
