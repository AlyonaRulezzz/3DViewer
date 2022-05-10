#ifndef SRC_INTERFACE_H_
#define SRC_INTERFACE_H_

#include <gtk/gtk.h>
#include <locale.h>
#include <math.h>

#include "./parser/s21_affine_transformations.h"
#include "./parser/s21_parsing_file.h"

#define WIDTH 900
#define HEIGHT 600

#define SWIDTH 480
#define SHEIGHT 210

#define AWIDTH 380
#define AHEIGHT 200

#define DWIDTH 450
#define DHEIGHT 325
#define MAX_GIF_FILE 50

#define SPAGES 2

typedef enum values_projection_type { Parallel, Central } proj_type;
typedef enum values_edge_type { Solid, Dashed } ed_type;
typedef enum values_vertex_type { None, Circle, Square } ver_type;
typedef enum colors_background { Light, Dark, Purple } back_col;
typedef enum colors_edges_or_vertex { Black, White, Green, Red } drow_col;
typedef enum value_picture { Null, Png, Bmp, Jpeg, Gif, Stop } pic_val;
typedef struct settings_t {
    proj_type projection_type;
    ver_type vertex_type;
    ed_type edge_type;

    back_col background_color;
    drow_col edges_color;
    drow_col vertex_color;

    double verter_size;
    double width_edge;
} Settings;

typedef struct interface_t {
    // main window
    GtkApplication *app;

    GtkWidget *main_window;
    GtkWidget *drawing_area;

    GtkWidget *main_box;
    GtkWidget *left_box;
    GtkWidget *right_box;

    GtkWidget *box_for_drawing_area;

    // information of object
    GtkWidget *file_name_label;
    GtkWidget *vertex_count_label;
    GtkWidget *edges_count_label;

    GtkWidget *box_for_grid;
    GtkWidget *entries_position[3];
    GtkWidget *entries_rotation[3];
    GtkWidget *entries_scale[3];

    GFile *file;
    char *text;

    // settings window
    GtkWidget *settings_window;
    GtkWidget *settings_box;

    // about program window
    GtkWidget *about_program_window;

    // general settings
    GtkWidget *projection_type_cbox;
    GtkWidget *edge_display_type_cbox;
    GtkWidget *save_settings_switch;
    GtkWidget *background_color_cbox;

    // object settings
    GtkWidget *edge_width_entry;
    GtkWidget *vertex_size_entry;
    GtkWidget *vertex_display_method_cbox;
    GtkWidget *edge_color_cbox;
    GtkWidget *vertex_color_cbox;
    char *vertex_display_method;

    st_data *data;
    Settings *settings;
    pic_val picture_value;

    int count_file_in_gif;

    double vector_position[3];
    double vector_rotation[3];
    double vector_scale[3];
} Interface;

const char *pagesName[SPAGES] = {"General", "Object"};
const char *statesName[3] = {"Position", "Euler", "Scale"};
const char *xyzName[3] = {"x", "y", "z"};

int RunApp(int argc, char **argv);
static void create_main_window(GtkApplication *app, Interface *interface);
static void create_header_buttons(Interface *interface);
static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, Interface *interface);
static void load_file(GtkWidget *button, Interface *interface);
static void set_text_labels_of_inf_obj(Interface *interface);
static void on_response(GtkNativeDialog *native, int response, Interface *interface);
static void create_buttons_in_menu(GtkWidget *menu_button, Interface *interface);
static void create_about_program_window(GtkWidget *button, Interface *interface);
static void create_main_elements(Interface *interface);
static void add_boxes_to_general_box(Interface *interface);
static void create_field_for_record_gif(GtkWidget *frame, Interface *interface);
static void create_field_for_obj_inf(GtkWidget *frame, Interface *interface);
static void create_states(GtkWidget *box, Interface *interface);
static void create_drawing_area(Interface *interface);
static void create_box_for_drawing_area(Interface *interface);
static void create_settings_window(Interface *interface);
static void create_sidebar(Interface *interface);
static void set_settings_field(GtkWidget *box, const char *name, Interface *interface);
static void create_object_settings(GtkWidget *box, Interface *interface);
static void create_general_settings(GtkWidget *box, Interface *interface);
static void destroy_settings_window(GtkWidget *settings_window, Interface *interface);
static void destroy_about_program_window(GtkWidget *about_program_window, Interface *interface);
static void create_text_view(GtkWidget *hbox);
static guint setup_scroll(GtkTextView *textview);
static void save_settings(GtkWidget *button, Interface *interface);
static gboolean scroll_to_end(GtkTextView *textview);
static void initialize_settings(Interface *interface);

static void save_settings_file(Settings *interface);
static void read_settings_file(Settings *interface);
static void vertex_drawing(cairo_t *cr, Interface *interface);
static void edge_drawing(cairo_t *cr, Interface *interface);
static void save_picture_png(GtkWidget *button, Interface *interface);
static void save_picture_jpeg(GtkWidget *button, Interface *interface);
static void save_picture_bmp(GtkWidget *button, Interface *interface);
static void create_gif_picture(GtkWidget *button, Interface *interface);
static void create_gif(GtkWidget *button, Interface *interface);

#endif  // SRC_INTERFACE_H_
