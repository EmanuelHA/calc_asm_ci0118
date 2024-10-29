#include <gtk/gtk.h>
//#include "calculadora.asm"
#define W_SIZE 64
extern int _process_data(int fst_op, int snd_op, int operator);
extern void _mem_reset();

int op_code = 0;
int input   = 0;
int data    = 0;
char buffer[33];

static void activate          (GtkApplication* application, gpointer user_data);
static void on_digit_pressed            (GtkWidget* button, gpointer user_data);
static void on_auxiliar_pressed         (GtkWidget* button, gpointer user_data);
static void on_operator_pressed         (GtkWidget* button, gpointer user_data);
static void on_calculate_pressed        (GtkWidget* button, gpointer user_data);
static void display_data                ();

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    app =  gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

 static void activate(GtkApplication* application, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* button;
    GtkWidget* label;
    GtkWidget *frame;

    window = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(window), "CALCU MEISTER");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 350);

    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    label = gtk_label_new(NULL);
    display_data(GTK_LABEL(label), input);

    frame = gtk_frame_new(NULL);
    gtk_widget_set_size_request(frame, W_SIZE, W_SIZE);
    gtk_frame_set_child(GTK_FRAME(frame), label);
    gtk_grid_attach(GTK_GRID(grid), frame, 0, 0, 10, 1);

    button = gtk_button_new_with_label("7");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 2, 4, 1, 1);

    button = gtk_button_new_with_label("8");
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 4, 1, 1);

    button = gtk_button_new_with_label("9");
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 6, 4, 1, 1);

    button = gtk_button_new_with_label("4");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 2, 6, 1, 1);

    button = gtk_button_new_with_label("5");
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 6, 1, 1);

    button = gtk_button_new_with_label("6");
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 6, 6, 1, 1);

    button = gtk_button_new_with_label("1");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 2, 8, 1, 1);

    button = gtk_button_new_with_label("2");
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 8, 1, 1);

    button = gtk_button_new_with_label("3");
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 6, 8, 1, 1);

    button = gtk_button_new_with_label("0");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_digit_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 2, 10, 4, 1);

    button = gtk_button_new_with_label("AC");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_auxiliar_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 2, 2, 1, 1);

    button = gtk_button_new_with_label("CE");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_auxiliar_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 2, 1, 1);

    button = gtk_button_new_with_label("+/-");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_auxiliar_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 6, 2, 1, 1);

    button = gtk_button_new_with_label("/");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_operator_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 8, 2, 1, 1);

    button = gtk_button_new_with_label("*");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_operator_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 8, 4, 1, 1);

    button = gtk_button_new_with_label("-");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_operator_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 8, 6, 1, 1);

    button = gtk_button_new_with_label("+");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_operator_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 8, 8, 1, 3);

    button = gtk_button_new_with_label("=");
    gtk_widget_set_size_request(button, W_SIZE, W_SIZE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_calculate_pressed), label);
    gtk_grid_attach(GTK_GRID(grid), button, 6, 10, 1, 1);

    gtk_window_present(GTK_WINDOW(window));

}

static void on_digit_pressed(GtkWidget* button, gpointer user_data) {
    const gchar* digit = gtk_button_get_label(GTK_BUTTON(button));
    if (((input <= 0) && ((input * 10) <= 0))
    || ((input > 0) && ((input * 10) > 0))) {
        input *= 10;
        input += atoi(digit);
        display_data(GTK_LABEL(user_data), input); 
    }
}

static void on_auxiliar_pressed(GtkWidget* button, gpointer user_data) {
    const gchar* digit = gtk_button_get_label(GTK_BUTTON(button));
    switch (*digit) {
        case 'A':
            _mem_reset();
            input   = 0;
            data    = 0;
            op_code = 0;
        break;
        case 'C':
            input = 0;
        break;
        case '+':
            input = (input)? input * -1 : data * -1;
        break;
    }
    display_data(GTK_LABEL(user_data), input); 
}

static void on_operator_pressed(GtkWidget* button, gpointer user_data) {
    const gchar* digit = gtk_button_get_label(GTK_BUTTON(button));
    switch (*digit) {
        case '+':
        op_code = 1;
        break;
        case '-':
        op_code = 2;
        break;
        case '*':
        op_code = 3;
        break;
        case '/':
        op_code = 4;
        break;
    }
    data = (input)? input : data;
    input = 0;
    display_data(GTK_LABEL(user_data), data); 
}


static void on_calculate_pressed(GtkWidget* button, gpointer user_data) {
    data = _process_data(data, input, op_code);
    input = 0;
    display_data(GTK_LABEL(user_data), data);
}

static void display_data(GtkLabel* label, int data) {
    snprintf(buffer, sizeof(buffer), "<span font='20'>%d</span>", data);
    gtk_label_set_markup(GTK_LABEL(label), buffer);
}