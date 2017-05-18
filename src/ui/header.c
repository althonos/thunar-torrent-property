
static GtkWidget* torrent_page_new_header(TorrentPage* page) {

  GtkWidget* header_hbox = gtk_hbox_new(FALSE, 0);
  GtkWidget* header_icon = gtk_image_new_from_icon_name("application-torrent", GTK_ICON_SIZE_DIALOG);
  GtkWidget* header_title = gtk_label_new("Torrent name");
  GtkWidget* stats_vbox = gtk_vbox_new(TRUE, 0);
  GtkWidget* seeders_hbox = gtk_hbox_new(TRUE, 0);
  GtkWidget* leechers_hbox = gtk_hbox_new(TRUE, 0);
  GtkWidget* seeders_label = gtk_label_new("<b>Seeders:</b>");
  GtkWidget* leechers_label = gtk_label_new("<b>Leechers:</b>");
  GtkWidget* seeders_value = gtk_label_new("<span foreground=\"green\"><b>N/A</b></span>");
  GtkWidget* leechers_value = gtk_label_new("<span foreground=\"red\"><b>N/A</b></span>");

  gtk_label_set_use_markup (GTK_LABEL(seeders_label), TRUE);
  gtk_label_set_use_markup (GTK_LABEL(leechers_label), TRUE);
  gtk_label_set_use_markup (GTK_LABEL(seeders_value), TRUE);
  gtk_label_set_use_markup (GTK_LABEL(leechers_value), TRUE);

  gtk_misc_set_alignment(GTK_MISC(seeders_label), 1.0f, 0.5f);
  gtk_misc_set_alignment(GTK_MISC(leechers_label), 1.0f, 0.5f);




  gtk_box_pack_start(GTK_BOX(header_hbox), header_icon, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(header_hbox), header_title, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(header_hbox), stats_vbox, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(stats_vbox), seeders_hbox, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(stats_vbox), leechers_hbox, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(seeders_hbox), seeders_label, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(seeders_hbox), seeders_value, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(leechers_hbox), leechers_label, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(leechers_hbox), leechers_value, FALSE, TRUE, 0);


  return header_hbox;
}
