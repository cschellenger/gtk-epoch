#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

static void text_changed(GtkWidget *millis_entry, gpointer data)
{
  const gchar* currText = gtk_editable_get_chars (GTK_EDITABLE(millis_entry), 0, -1);
  char outtime[80];
  time_t seconds;
  struct tm ts;
  if (strlen(currText) > 0)
  {
    if (!g_ascii_isdigit (currText[strlen(currText) - 1]))
    {
      g_print("Ignoring non-digit char\n");
      gtk_entry_set_text(GTK_ENTRY(millis_entry), g_strndup(currText, strlen(currText) - 1));
    }
    currText = gtk_editable_get_chars (GTK_EDITABLE(millis_entry), 0, -1);
    seconds = (time_t)(strtoll(currText, NULL, 0) / 1000);
    ts = *localtime(&seconds);
    strftime(outtime, sizeof(outtime), "%Y-%m-%d %H:%M:%S %Z", &ts);
    gtk_entry_set_text(GTK_ENTRY(data), outtime);
  }
}

static void init_time_to_now(GtkWidget *millis_entry, GtkWidget *date_label)
{
  char intime[20];
  uint64_t millis = time(NULL) * 1000;
  snprintf(intime, 20, "%llu", millis);
  gtk_entry_set_text(GTK_ENTRY(millis_entry), intime);
}

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *main_box;
  GtkWidget *millis_entry;
  GtkWidget *date_label;
  GValue editable_false = G_VALUE_INIT;
  GValue max_chars = G_VALUE_INIT;

  /* Configuration for making field non editable */
  g_value_init(&editable_false, G_TYPE_BOOLEAN);
  g_value_set_boolean(&editable_false, FALSE);

  /* Configuration for setting default field width */
  g_value_init(&max_chars, G_TYPE_INT);
  g_value_set_int(&max_chars, 25);

  /* Main window */
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Epoch Converter");
  gtk_window_set_default_size (GTK_WINDOW (window), 225, 50);
  gtk_window_set_icon_name(GTK_WINDOW (window), "applications-engineering");

  /* Main container */
  main_box = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);
  gtk_container_add (GTK_CONTAINER (window), main_box);

  /* Input for timestamp in millis */
  millis_entry = gtk_entry_new();
  g_object_set_property(G_OBJECT(millis_entry), "width-chars", &max_chars);

  /* Output widget for date. I use a GtkEntry so you can select the text. */
  date_label = gtk_entry_new();
  g_object_set_property(G_OBJECT(date_label), "editable", &editable_false);
  g_object_set_property(G_OBJECT(date_label), "width-chars", &max_chars);

  /* Add the widgets to the container */
  gtk_container_add(GTK_CONTAINER (main_box), millis_entry);
  gtk_container_add(GTK_CONTAINER (main_box), date_label);

  /* Connect the text change signal for the input to a method */
  g_signal_connect (GTK_EDITABLE(millis_entry), "changed", G_CALLBACK (text_changed), date_label);

  /* Start by initializing the input to the current timestamp */
  init_time_to_now(millis_entry, date_label);

  /* Begin display */
  gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("net.darkport.epoch", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
