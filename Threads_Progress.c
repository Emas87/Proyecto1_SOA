#include <gtk/gtk.h>
//gcc Threads_Progress.c `pkg-config --cflags --libs gtk+-3.0` 
static double percent = 0.0;
GtkWidget *window, *progress, *grid, *label;

 
static gboolean inc_progress(gpointer data)
{
   percent += 0.05;
   if(percent > 1.0)
       percent = 0.0;
   gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data), percent);
   char c[3];
   sprintf(c, "%d%%", (int)(percent*100));
   gtk_progress_bar_set_text(GTK_PROGRESS_BAR(data), c);
}
void Widgets_bars_setup(int n_tids){
   
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
   
   for(int i = 0;i<n_tids;i++){
/*// Like "ENEMY enemies[100]", but from the heap
ENEMY* enemies = malloc(100 * sizeof(ENEMY));
if (!enemies) { error handling }

// You can index pointers just like arrays.
enemies[0] = CreateEnemy();

// Make the array bigger
ENEMY* more_enemies = realloc(enemies, 200 * sizeof(ENEMY));
if (!more_enemies) { error handling }
enemies = more_enemies;

// Clean up when you're done.
free(enemies);*/
   }
   progress = gtk_progress_bar_new();
   gtk_widget_set_size_request (progress,1000, -1);
   label = gtk_label_new("Pi = ");
   //gtk_label_set_text(GTK_LABEL(label), "World Hello");
   //char c[4];
   //sprintf(c, "%d%%", 0);
   grid = gtk_grid_new();
   gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0.2);
   gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress),TRUE);
   gtk_grid_attach(GTK_GRID(grid),progress,0,0,1,1);
   gtk_grid_attach(GTK_GRID(grid),label,1,0,1,1);   
   gtk_container_add(GTK_CONTAINER(window), grid); 
   gtk_widget_show_all(window);
   gtk_main();

}
void update_progress(int Porcentaje,int tid_activo){

}
void update_result(int Resultado,int tid_activo){

}
int main(int argc, char* argv[])
{
   int Porcentaje = 26;
   double Resultado = 3.1416;
   int tid_activo = 2;
   int tids[4] = {6,1,3,5,NULL};
   int n_tids = 4;
   gtk_init(&argc, &argv);
   Progress_bars_setup(n_tids);
  
   return 0;
}
