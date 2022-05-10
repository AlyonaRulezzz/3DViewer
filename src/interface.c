#include "interface.h"

static void save_settings_file(Settings *settings) {
    FILE *settings_file;
    settings_file = fopen("settings.txt", "wb");
    fwrite(settings, sizeof(Settings), 1, settings_file);
    fclose(settings_file);
}

static void read_settings_file(Settings *settings) {
    FILE *settings_file;
    settings_file = fopen("settings.txt", "rb");
    if (settings_file != NULL) {
        fread(settings, sizeof(Settings), 1, settings_file);
        fclose(settings_file);
    }
}
static void vertex_drawing(cairo_t *cr, Interface *interface) {
    if (interface->settings->vertex_color == White) {
        cairo_set_source_rgb(cr, 0.94901961, 0.94901961, 0.94901961);
    } else if (interface->settings->vertex_color == Green) {
        cairo_set_source_rgb(cr, 0.17647059, 0.7254902, 0.17647059);
    } else if (interface->settings->vertex_color == Red) {
        cairo_set_source_rgb(cr, 1.0, 0.2, 0.2);
    } else {
        cairo_set_source_rgb(cr, 0.30196078, 0.30196078, 0.30196078);
    }
    double projection_point = 400;
    double x, y;
    for (int i = 0; i < interface->data->count_vert; ++i) {
        if (interface->settings->projection_type == Central) {
            x = projection_point * interface->data->matrix_3d[i].matrix[0][0] /
                (projection_point - interface->data->matrix_3d[i].matrix[2][0]);
            y = projection_point * -interface->data->matrix_3d[i].matrix[1][0] /
                (projection_point - interface->data->matrix_3d[i].matrix[2][0]);
        } else {
            x = interface->data->matrix_3d[i].matrix[0][0] - interface->data->matrix_3d[i].matrix[2][0] / 2;
            y = -interface->data->matrix_3d[i].matrix[1][0] - interface->data->matrix_3d[i].matrix[2][0] / 2;
        }
        if (interface->settings->vertex_type == Circle) {
            cairo_arc(cr, x, y, interface->settings->verter_size, 0, 3 * M_PI);
            cairo_stroke(cr);
        } else if (interface->settings->vertex_type == Square) {
            cairo_rectangle(cr, x - (interface->settings->verter_size / 2),
                            y - (interface->settings->verter_size / 2), interface->settings->verter_size,
                            interface->settings->verter_size);
            cairo_stroke(cr);
        }
    }
}
static void edge_drawing(cairo_t *cr, Interface *interface) {
    if (interface->settings->edges_color == White) {
        cairo_set_source_rgb(cr, 0.94901961, 0.94901961, 0.94901961);
    } else if (interface->settings->edges_color == Green) {
        cairo_set_source_rgb(cr, 0.17647059, 0.7254902, 0.17647059);
    } else if (interface->settings->edges_color == Red) {
        cairo_set_source_rgb(cr, 1.0, 0.2, 0.2);
    } else {
        cairo_set_source_rgb(cr, 0.30196078, 0.30196078, 0.30196078);
    }
    double projection_point = 400;
    double x, y;
    for (int i = 0; i < interface->data->count_facets; ++i) {
        cairo_set_line_width(cr, interface->settings->width_edge);
        if (interface->settings->projection_type == Central) {
            x = projection_point *
                interface->data->matrix_3d[interface->data->polygons[i].vert[0] - 1].matrix[0][0] /
                (projection_point -
                 interface->data->matrix_3d[interface->data->polygons[i].vert[0] - 1].matrix[2][0]);
            y = projection_point *
                -interface->data->matrix_3d[interface->data->polygons[i].vert[0] - 1].matrix[1][0] /
                (projection_point -
                 interface->data->matrix_3d[interface->data->polygons[i].vert[0] - 1].matrix[2][0]);
        } else {
            x = interface->data->matrix_3d[interface->data->polygons[i].vert[0] - 1].matrix[0][0] -
                interface->data->matrix_3d[interface->data->polygons[i].vert[0] - 1].matrix[2][0] / 2;
            y = -interface->data->matrix_3d[interface->data->polygons[i].vert[0] - 1].matrix[1][0] -
                interface->data->matrix_3d[interface->data->polygons[i].vert[0] - 1].matrix[2][0] / 2;
        }
        cairo_move_to(cr, x, y);
        for (int j = 0; j < interface->data->polygons[i].count_number_vert; ++j) {
            if (interface->settings->projection_type == Central) {
                x = projection_point *
                    interface->data->matrix_3d[interface->data->polygons[i].vert[j] - 1].matrix[0][0] /
                    (projection_point -
                     interface->data->matrix_3d[interface->data->polygons[i].vert[j] - 1].matrix[2][0]);
                y = projection_point *
                    -interface->data->matrix_3d[interface->data->polygons[i].vert[j] - 1].matrix[1][0] /
                    (projection_point -
                     interface->data->matrix_3d[interface->data->polygons[i].vert[j] - 1].matrix[2][0]);
            } else {
                x = interface->data->matrix_3d[interface->data->polygons[i].vert[j] - 1].matrix[0][0] -
                    interface->data->matrix_3d[interface->data->polygons[i].vert[j] - 1].matrix[2][0] / 2;
                y = -interface->data->matrix_3d[interface->data->polygons[i].vert[j] - 1].matrix[1][0] -
                    interface->data->matrix_3d[interface->data->polygons[i].vert[j] - 1].matrix[2][0] / 2;
            }
            cairo_line_to(cr, x, y);
        }
        cairo_line_to(cr, x, y);
    }
    cairo_stroke(cr);
}

static void create_gif_picture(GtkWidget *button, Interface *interface);
static void save_picture_png(GtkWidget *button, Interface *interface) {
    interface->picture_value = Png;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                   (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
}
static void save_picture_bmp(GtkWidget *button, Interface *interface) {
    interface->picture_value = Bmp;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                   (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
}
static void save_picture_jpeg(GtkWidget *button, Interface *interface) {
    interface->picture_value = Jpeg;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                   (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
}

static void create_gif_picture(GtkWidget *button, Interface *interface) {
    interface->picture_value = Gif;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                   (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
}
static void create_gif(GtkWidget *button, Interface *interface) {
    interface->picture_value = Stop;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                   (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
}

static gboolean scroll_to_end(GtkTextView *textview) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    GtkTextMark *mark = gtk_text_buffer_get_mark(buffer, "end");
    GtkTextIter iter;

    char *text;
    static int count;
    const char *about_program = "Эта программа создана Евгением Mabobbi и Сергеем Mskyler.";
    gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
    gtk_text_buffer_insert(buffer, &iter, "\n", -1);
    text = g_strdup_printf(about_program);
    count++;

    gtk_text_buffer_insert(buffer, &iter, text, -1);
    g_free(text);

    gtk_text_view_scroll_mark_onscreen(textview, mark);

    if (count > 100) {
        count = 0;
        return G_SOURCE_REMOVE;
    }

    return G_SOURCE_CONTINUE;
}

static guint setup_scroll(GtkTextView *textview) {
    GtkTextBuffer *buffer;
    GtkTextIter iter;

    buffer = gtk_text_view_get_buffer(textview);
    gtk_text_view_set_editable(textview, FALSE);

    // set margin
    gtk_widget_set_margin_start(GTK_WIDGET(textview), 10);
    gtk_widget_set_margin_end(GTK_WIDGET(textview), 10);
    gtk_widget_set_margin_bottom(GTK_WIDGET(textview), 10);
    //

    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_create_mark(buffer, "end", &iter, FALSE);

    return g_timeout_add(100, (GSourceFunc)scroll_to_end, textview);
}

static void remove_timeout(GtkWidget *window, gpointer timeout) {
    g_source_remove(GPOINTER_TO_UINT(timeout));
}

static void create_text_view(GtkWidget *hbox) {
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    GtkWidget *textview = gtk_text_view_new();
    guint timeout;

    gtk_box_append(GTK_BOX(hbox), scrolled_window);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), textview);

    timeout = setup_scroll(GTK_TEXT_VIEW(textview));

    g_signal_connect(textview, "destroy", G_CALLBACK(remove_timeout), GUINT_TO_POINTER(timeout));
}

static void destroy_about_program_window(GtkWidget *about_program_window, Interface *interface) {
    interface->about_program_window == NULL;
}

static void save_settings(GtkWidget *buttton, Interface *interface) {
    char *temp_pro_type =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(interface->projection_type_cbox));
    if (strcmp(temp_pro_type, "Parallel") == 0) {
        interface->settings->projection_type = Parallel;
    } else {
        interface->settings->projection_type = Central;
    }

    char *temp_edge_type =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(interface->edge_display_type_cbox));
    if (strcmp(temp_edge_type, "Solid") == 0) {
        interface->settings->edge_type = Solid;
    } else {
        interface->settings->edge_type = Dashed;
    }

    char *temp_vertex_type =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(interface->vertex_display_method_cbox));
    if (strcmp(temp_vertex_type, "Circle") == 0) {
        interface->settings->vertex_type = Circle;
    } else if (strcmp(temp_vertex_type, "Square") == 0) {
        interface->settings->vertex_type = Square;
    } else {
        interface->settings->vertex_type = None;
    }

    char *temp_background_color =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(interface->background_color_cbox));
    if (strcmp(temp_background_color, "Dark") == 0) {
        interface->settings->background_color = Dark;
    } else if (strcmp(temp_background_color, "Purple") == 0) {
        interface->settings->background_color = Purple;
    } else {
        interface->settings->background_color = Light;
    }

    char *temp_edges_color =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(interface->edge_color_cbox));
    if (strcmp(temp_edges_color, "White") == 0) {
        interface->settings->edges_color = White;
    } else if (strcmp(temp_edges_color, "Green") == 0) {
        interface->settings->edges_color = Green;
    } else if (strcmp(temp_edges_color, "Red") == 0) {
        interface->settings->edges_color = Red;
    } else {
        interface->settings->edges_color = Black;
    }

    char *temp_vertex_color =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(interface->vertex_color_cbox));
    if (strcmp(temp_vertex_color, "White") == 0) {
        interface->settings->vertex_color = White;
    } else if (strcmp(temp_vertex_color, "Green") == 0) {
        interface->settings->vertex_color = Green;
    } else if (strcmp(temp_vertex_color, "Red") == 0) {
        interface->settings->vertex_color = Red;
    } else {
        interface->settings->vertex_color = Black;
    }

    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->vertex_size_entry));
    char *value_size = (char *)gtk_entry_buffer_get_text(buffer);
    sscanf(value_size, "%lf", &interface->settings->verter_size);

    GtkEntryBuffer *edge_size_buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->edge_width_entry));
    char *width_edge_size = (char *)gtk_entry_buffer_get_text(edge_size_buffer);
    sscanf(width_edge_size, "%lf", &interface->settings->width_edge);

    save_settings_file(interface->settings);
    free(temp_background_color);
    free(temp_edge_type);
    free(temp_edges_color);
    free(temp_pro_type);
    free(temp_vertex_color);
    free(temp_vertex_type);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                   (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
}

static void destroy_settings_window(GtkWidget *settings_window, Interface *interface) {
    if (interface->settings_window != NULL) {
        interface->settings_window = NULL;
        create_settings_window(interface);
    }
}

static void create_object_settings(GtkWidget *box, Interface *interface) {
    // edges settings init
    GtkWidget *left_frame = gtk_frame_new("Edges");
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
    GtkWidget *box_for_edge_width = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);

    GtkWidget *edge_display_type_label = gtk_label_new("Type");
    GtkWidget *box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    interface->edge_display_type_cbox = gtk_combo_box_text_new();

    GtkWidget *edge_color_label = gtk_label_new("Color");
    GtkWidget *box4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    interface->edge_color_cbox = gtk_combo_box_text_new();

    GtkWidget *edge_width_label = gtk_label_new("Width");
    interface->edge_width_entry = gtk_entry_new();
    GtkEntryBuffer *edge_size_buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->edge_width_entry));
    char *width_edge_size = (char *)malloc(256 * sizeof(char));
    snprintf(width_edge_size, sizeof(width_edge_size), "%lf", interface->settings->width_edge);
    gtk_entry_buffer_set_text(edge_size_buffer, width_edge_size, -1);

    // vertex settings init
    GtkWidget *right_frame = gtk_frame_new("Vertexes");
    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
    GtkWidget *box_for_vertex_size = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    GtkWidget *box_for_vertex_display_method = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);

    GtkWidget *vertex_display_method_label = gtk_label_new("Display method");
    interface->vertex_display_method_cbox = gtk_combo_box_text_new();

    GtkWidget *vertex_color_label = gtk_label_new("Color");
    GtkWidget *box5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    interface->vertex_color_cbox = gtk_combo_box_text_new();

    GtkWidget *vertex_size_label = gtk_label_new("Size");
    interface->vertex_size_entry = gtk_entry_new();
    GtkEntryBuffer *vertex_size_buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->vertex_size_entry));
    char *vertex_size = (char *)malloc(256 * sizeof(char));
    snprintf(vertex_size, sizeof(vertex_size), "%lf", interface->settings->verter_size);
    gtk_entry_buffer_set_text(vertex_size_buffer, vertex_size, -1);

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->vertex_display_method_cbox), "None");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->vertex_display_method_cbox), "Circle");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->vertex_display_method_cbox), "Square");

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->vertex_color_cbox), "Black");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->vertex_color_cbox), "White");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->vertex_color_cbox), "Green");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->vertex_color_cbox), "Red");

    gtk_combo_box_set_active(GTK_COMBO_BOX(interface->vertex_display_method_cbox),
                             interface->settings->vertex_type);
    gtk_combo_box_set_active(GTK_COMBO_BOX(interface->vertex_color_cbox), interface->settings->vertex_color);

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->edge_display_type_cbox), "Solid");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->edge_display_type_cbox), "Dashed");

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->edge_color_cbox), "Black");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->edge_color_cbox), "White");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->edge_color_cbox), "Green");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->edge_color_cbox), "Red");

    gtk_combo_box_set_active(GTK_COMBO_BOX(interface->edge_display_type_cbox),
                             interface->settings->edge_type);
    gtk_combo_box_set_active(GTK_COMBO_BOX(interface->edge_color_cbox), interface->settings->edges_color);

    // set hexpand
    gtk_widget_set_hexpand(left_frame, TRUE);
    gtk_widget_set_hexpand(right_frame, TRUE);
    // gtk_widget_set_hexpand(interface->vertex_display_method_cbox, TRUE);

    // set margin for left frame
    gtk_widget_set_margin_start(left_frame, 10);
    gtk_widget_set_margin_end(left_frame, 5);
    gtk_widget_set_margin_top(left_frame, 10);
    gtk_widget_set_margin_bottom(left_frame, 10);

    // set margin for right frame
    gtk_widget_set_margin_start(right_frame, 5);
    gtk_widget_set_margin_end(right_frame, 10);
    gtk_widget_set_margin_top(right_frame, 10);
    gtk_widget_set_margin_bottom(right_frame, 10);

    // set margin for edge width label
    gtk_widget_set_margin_start(edge_width_label, 10);
    gtk_widget_set_margin_top(edge_width_label, 10);

    // set margin for vertex size label
    gtk_widget_set_margin_start(vertex_size_label, 10);
    gtk_widget_set_margin_top(vertex_size_label, 10);

    // set margin for gear vertex size
    gtk_widget_set_margin_start(interface->edge_width_entry, 10);
    gtk_widget_set_margin_end(interface->edge_width_entry, 10);
    gtk_widget_set_margin_top(interface->edge_width_entry, 10);

    // set margin for gear edge width
    gtk_widget_set_margin_start(interface->vertex_size_entry, 10);
    gtk_widget_set_margin_end(interface->vertex_size_entry, 10);
    gtk_widget_set_margin_top(interface->vertex_size_entry, 10);

    // set margin for vertex display method label
    gtk_widget_set_margin_start(vertex_display_method_label, 10);

    // set margin for vertex display method cbox
    gtk_widget_set_margin_start(interface->vertex_display_method_cbox, 10);
    gtk_widget_set_margin_end(interface->vertex_display_method_cbox, 10);

    // set margin for edge type cbox label
    gtk_widget_set_margin_start(edge_display_type_label, 10);

    // set margin for edge type cbox
    gtk_widget_set_margin_start(interface->edge_display_type_cbox, 10);
    gtk_widget_set_margin_end(interface->edge_display_type_cbox, 10);

    // set margin edge color cbox
    gtk_widget_set_margin_start(interface->edge_color_cbox, 10);
    gtk_widget_set_margin_top(interface->edge_color_cbox, 10);

    // set margin edge color label
    gtk_widget_set_margin_start(edge_color_label, 10);
    gtk_widget_set_margin_top(edge_color_label, 10);

    // set margin vertex color cbox
    gtk_widget_set_margin_start(interface->vertex_color_cbox, 10);
    gtk_widget_set_margin_top(interface->vertex_color_cbox, 10);

    // set margin vertex color label
    gtk_widget_set_margin_start(vertex_color_label, 10);
    gtk_widget_set_margin_top(vertex_color_label, 10);

    // set margin box5
    gtk_widget_set_margin_bottom(box5, 10);

    // box append
    gtk_box_append(GTK_BOX(box), left_frame);
    gtk_box_append(GTK_BOX(box), right_frame);

    gtk_frame_set_child(GTK_FRAME(left_frame), left_box);
    gtk_frame_set_child(GTK_FRAME(right_frame), right_box);

    gtk_box_append(GTK_BOX(box_for_edge_width), edge_width_label);
    gtk_box_append(GTK_BOX(box_for_edge_width), interface->edge_width_entry);

    gtk_box_append(GTK_BOX(box_for_vertex_size), vertex_size_label);
    gtk_box_append(GTK_BOX(box_for_vertex_size), interface->vertex_size_entry);
    gtk_box_append(GTK_BOX(box_for_vertex_display_method), vertex_display_method_label);
    gtk_box_append(GTK_BOX(box_for_vertex_display_method), interface->vertex_display_method_cbox);

    gtk_box_append(GTK_BOX(box3), edge_display_type_label);
    gtk_box_append(GTK_BOX(box3), interface->edge_display_type_cbox);

    gtk_box_append(GTK_BOX(box4), edge_color_label);
    gtk_box_append(GTK_BOX(box4), interface->edge_color_cbox);

    gtk_box_append(GTK_BOX(box5), vertex_color_label);
    gtk_box_append(GTK_BOX(box5), interface->vertex_color_cbox);

    gtk_box_append(GTK_BOX(left_box), box3);
    gtk_box_append(GTK_BOX(left_box), box_for_edge_width);
    gtk_box_append(GTK_BOX(left_box), box4);

    gtk_box_append(GTK_BOX(right_box), box_for_vertex_display_method);
    gtk_box_append(GTK_BOX(right_box), box_for_vertex_size);
    gtk_box_append(GTK_BOX(right_box), box5);
    // free
    free(width_edge_size);
    free(vertex_size);
}

static void create_general_settings(GtkWidget *box, Interface *interface) {
    GtkWidget *frame = gtk_frame_new(NULL);
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);

    GtkWidget *projection_type_label = gtk_label_new("Projection type");
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    interface->projection_type_cbox = gtk_combo_box_text_new();

    GtkWidget *background_color_label = gtk_label_new("Background color");
    GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    interface->background_color_cbox = gtk_combo_box_text_new();

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->projection_type_cbox), "Parallel");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->projection_type_cbox), "Central");

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->background_color_cbox), "Light");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->background_color_cbox), "Dark");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface->background_color_cbox), "Purple");

    gtk_combo_box_set_active(GTK_COMBO_BOX(interface->projection_type_cbox),
                             interface->settings->projection_type);
    gtk_combo_box_set_active(GTK_COMBO_BOX(interface->background_color_cbox),
                             interface->settings->background_color);

    gtk_widget_set_vexpand(frame, TRUE);
    gtk_frame_set_child(GTK_FRAME(frame), main_box);

    gtk_box_append(GTK_BOX(box1), projection_type_label);
    gtk_box_append(GTK_BOX(box1), interface->projection_type_cbox);
    gtk_box_append(GTK_BOX(box2), background_color_label);
    gtk_box_append(GTK_BOX(box2), interface->background_color_cbox);

    // set margin for frame
    gtk_widget_set_margin_start(frame, 5);
    gtk_widget_set_margin_end(frame, 5);
    gtk_widget_set_margin_top(frame, 5);
    gtk_widget_set_margin_bottom(frame, 5);

    // set margin for "projection type"
    gtk_widget_set_margin_start(projection_type_label, 20);
    gtk_widget_set_margin_top(projection_type_label, 20);
    gtk_widget_set_margin_bottom(projection_type_label, 20);

    gtk_widget_set_margin_start(interface->projection_type_cbox, 20);
    gtk_widget_set_margin_top(interface->projection_type_cbox, 20);
    gtk_widget_set_margin_bottom(interface->projection_type_cbox, 20);

    gtk_widget_set_margin_start(background_color_label, 20);

    gtk_widget_set_margin_start(interface->background_color_cbox, 20);

    // box append
    gtk_box_append(GTK_BOX(box), frame);
    gtk_box_append(GTK_BOX(main_box), box1);
    gtk_box_append(GTK_BOX(main_box), box2);
}

static void set_settings_field(GtkWidget *box, const char *name, Interface *interface) {
    if (box == NULL || name == NULL) {
        return;
    }

    if (name == *(pagesName + 0)) {
        create_general_settings(box, interface);
    } else if (name == *(pagesName + 1)) {
        create_object_settings(box, interface);
    } else if (name == *(pagesName + 2)) {
    } else {
        return;
    }
}

static void create_sidebar(Interface *interface) {
    GtkWidget *sidebar = gtk_stack_sidebar_new();
    GtkWidget *stack = gtk_stack_new();
    const char *name = NULL;

    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);
    gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));

    gtk_widget_set_hexpand(stack, TRUE);

    gtk_box_append(GTK_BOX(interface->settings_box), sidebar);
    gtk_box_append(GTK_BOX(interface->settings_box), stack);

#pragma region ::One of page::

#pragma endregion

    for (int page = 0; page < SPAGES; page++) {
        name = *(pagesName + page);

        GtkWidget *label = gtk_label_new(name);
        GtkWidget *box;

        if (page == 1) {
            box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
        } else {
            box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
        }

        set_settings_field(box, name, interface);

        gtk_stack_add_named(GTK_STACK(stack), box, name);
        g_object_set(gtk_stack_get_page(GTK_STACK(stack), box), "title", name, NULL);
    }

    gtk_window_set_child(GTK_WINDOW(interface->settings_window), interface->settings_box);
}

static void show_settings_window(GtkWidget *button, Interface *interface) {
    if (interface->settings_window != NULL) {
        gtk_widget_show(interface->settings_window);
    }
}

static void create_settings_window(Interface *interface) {
    if (interface->settings_window == NULL) {
        GtkWidget *header = gtk_header_bar_new();
        GtkWidget *save_settings_button = gtk_button_new_from_icon_name("object-select-symbolic");

        interface->settings_window = gtk_application_window_new(interface->app);
        interface->settings_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

        g_signal_connect(GTK_BUTTON(save_settings_button), "clicked", G_CALLBACK(save_settings), interface);
        g_signal_connect(GTK_WINDOW(interface->settings_window), "destroy",
                         G_CALLBACK(destroy_settings_window), interface);

        gtk_window_set_titlebar(GTK_WINDOW(interface->settings_window), header);
        gtk_window_set_title(GTK_WINDOW(interface->settings_window), "Settings");
        gtk_window_set_default_size(GTK_WINDOW(interface->settings_window), SWIDTH, SHEIGHT);
        gtk_window_set_resizable(GTK_WINDOW(interface->settings_window), false);

        gtk_header_bar_pack_start(GTK_HEADER_BAR(header), save_settings_button);
        create_sidebar(interface);
    }
}

static void changes_states_value_pos(GtkWidget *button, Interface *interface) {
    if (interface->file != NULL) {
        for (int i = 0; i < 3; i++) {
            GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->entries_position[i]));
            char *value_pos = (char *)gtk_entry_buffer_get_text(buffer);
            sscanf(value_pos, "%lf", &interface->vector_position[i]);
        }
        moving(interface->data, interface->vector_position[0], interface->vector_position[1],
               interface->vector_position[2]);
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                       (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
    }
}

static void changes_states_value_rot(GtkWidget *button, Interface *interface) {
    if (interface->file != NULL) {
        for (int i = 0; i < 3; i++) {
            GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->entries_rotation[i]));
            char *value_rot = (char *)gtk_entry_buffer_get_text(buffer);

            sscanf(value_rot, "%lf", &interface->vector_rotation[i]);
        }
        rotation(interface->data, interface->vector_rotation[0], interface->vector_rotation[1],
                 interface->vector_rotation[2]);
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                       (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
    }
}

static void changes_states_value_scale(GtkWidget *button, Interface *interface) {
    if (interface->file != NULL) {
        for (int i = 0; i < 3; i++) {
            GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->entries_scale[i]));
            char *value_scale = (char *)gtk_entry_buffer_get_text(buffer);

            sscanf(value_scale, "%lf", &interface->vector_scale[i]);
        }
        resize(interface->data, interface->vector_scale[0], interface->vector_scale[1],
               interface->vector_scale[2]);
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                       (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
    }
}

static void create_states(GtkWidget *box, Interface *interface) {
    GtkWidget *frame_for_states = gtk_frame_new(NULL);
    GtkWidget *grid_for_states = gtk_grid_new();
    interface->box_for_grid = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);

    gtk_widget_set_margin_start(frame_for_states, 40);
    gtk_widget_set_margin_end(frame_for_states, 40);
    gtk_widget_set_margin_bottom(frame_for_states, 20);
    gtk_widget_set_margin_end(interface->box_for_grid, 20);
    gtk_widget_set_margin_start(interface->box_for_grid, 17);

    for (int i = 0; i < 4; i++) {
        if (i > 0) {
            GtkWidget *label_state = gtk_label_new(*(statesName + (i - 1)));
            gtk_grid_attach(GTK_GRID(grid_for_states), label_state, 0, i, 1, 1);

            if (i == 3) {
                gtk_widget_set_margin_bottom(label_state, 5);
            }
        }

        for (int j = 0; j < 4; j++) {
            if (i > 0) {
                if (j > 0) {
                    GtkEntryBuffer *entry_buffer;
                    if (i == 1) {
                        interface->entries_position[j - 1] = gtk_entry_new();
                        entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->entries_position[j - 1]));
                        gtk_grid_attach(GTK_GRID(grid_for_states), interface->entries_position[j - 1], j, i,
                                        1, 1);
                    } else if (i == 2) {
                        interface->entries_rotation[j - 1] = gtk_entry_new();
                        entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->entries_rotation[j - 1]));
                        gtk_grid_attach(GTK_GRID(grid_for_states), interface->entries_rotation[j - 1], j, i,
                                        1, 1);
                    } else {
                        interface->entries_scale[j - 1] = gtk_entry_new();
                        entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(interface->entries_scale[j - 1]));
                        gtk_grid_attach(GTK_GRID(grid_for_states), interface->entries_scale[j - 1], j, i, 1,
                                        1);
                        gtk_widget_set_margin_bottom(interface->entries_scale[j - 1], 5);
                    }
                    gtk_entry_buffer_set_text(entry_buffer, "0", 1);
                }

                if (j == 3) {
                    GtkWidget *button = gtk_button_new_from_icon_name("object-select-symbolic");
                    gtk_grid_attach(GTK_GRID(grid_for_states), button, ++j, i, 1, 1);

                    if (i == 1) {
                        g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(changes_states_value_pos),
                                         interface);
                    } else if (i == 2) {
                        g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(changes_states_value_rot),
                                         interface);
                    } else if (i == 3) {
                        g_signal_connect(GTK_BUTTON(button), "clicked",
                                         G_CALLBACK(changes_states_value_scale), interface);
                        gtk_widget_set_margin_bottom(button, 5);
                    }
                }
            } else {
                GtkWidget *label_state = gtk_label_new(*(xyzName + j));
                gtk_grid_attach(GTK_GRID(grid_for_states), label_state, j + 1, 0, 1, 1);
                gtk_widget_set_margin_top(label_state, 5);
            }
        }
    }
    gtk_grid_set_column_spacing(GTK_GRID(grid_for_states), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid_for_states), 5);

    gtk_box_append(GTK_BOX(interface->box_for_grid), grid_for_states);
    gtk_frame_set_child(GTK_FRAME(frame_for_states), interface->box_for_grid);
    gtk_box_append(GTK_BOX(box), frame_for_states);
}

static void create_box_for_drawing_area(Interface *interface) {
    if (interface->drawing_area == NULL) {
        return;
    }

    interface->box_for_drawing_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
    gtk_box_append(GTK_BOX(interface->box_for_drawing_area), interface->drawing_area);
    create_states(interface->box_for_drawing_area, interface);
}

static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, Interface *interface) {
    setlocale(LC_ALL, "C");
    cairo_surface_t *surface = cairo_get_target(cr);
    if (interface->settings->background_color == Purple) {
        cairo_set_source_rgb(cr, 0.9254902, 0.70196078, 1.0);
    } else if (interface->settings->background_color == Dark) {
        cairo_set_source_rgb(cr, 0.30196078, 0.30196078, 0.30196078);
    } else {
        cairo_set_source_rgb(cr, 0.94901961, 0.94901961, 0.94901961);
    }
    cairo_paint(cr);

    if (interface->file != NULL) {
        cairo_translate(cr, width / 2, height / 2);
        if (interface->settings->edge_type == Dashed) {
            static const double dashed[] = {5.0};
            cairo_set_dash(cr, dashed, 1, 0);
            cairo_stroke(cr);
        }
        if (interface->data != NULL) {
            vertex_drawing(cr, interface);
            edge_drawing(cr, interface);
        }
    }
    if (interface->picture_value == Png) {
        cairo_surface_write_to_png(surface, "./picture/picture.png");
        interface->picture_value = Null;
    } else if (interface->picture_value == Bmp) {
        cairo_surface_write_to_png(surface, "./picture/picture.bmp");
        interface->picture_value = Null;
    } else if (interface->picture_value == Jpeg) {
        cairo_surface_write_to_png(surface, "./picture/picture.jpeg");
        interface->picture_value = Null;
    } else if (interface->picture_value == Gif) {
        if (interface->count_file_in_gif <= 50) {
            char *name_file = (char *)calloc(256, sizeof(char));
            snprintf(name_file, sizeof(char) * 25, "./gif/picture_%d.png", interface->count_file_in_gif);
            cairo_surface_write_to_png(surface, name_file);
            interface->count_file_in_gif++;
            free(name_file);
        } else {
            interface->picture_value = Null;
        }
    } else if (interface->picture_value == Stop) {
        if (interface->count_file_in_gif > 2) {
            system("convert   -delay 10   -loop 0   ./gif/picture_*.png    ./gif/picture.gif");
            for (int i = 0; i < interface->count_file_in_gif; ++i) {
                char *name_file = (char *)calloc(256, sizeof(char));
                snprintf(name_file, sizeof(char) * 25, "./gif/picture_%d.png", i);
                remove(name_file);
                free(name_file);
            }
            interface->picture_value = Null;
        }
        interface->count_file_in_gif = 1;
    } else {
        interface->picture_value = Null;
    }
}

static void create_drawing_area(Interface *interface) {
    interface->drawing_area = gtk_drawing_area_new();

    gtk_widget_set_margin_start(interface->drawing_area, 20);
    gtk_widget_set_margin_end(interface->drawing_area, 20);
    gtk_widget_set_margin_top(interface->drawing_area, 20);
    gtk_widget_set_margin_bottom(interface->drawing_area, 20);

    gtk_widget_set_hexpand(interface->drawing_area, TRUE);
    gtk_widget_set_vexpand(interface->drawing_area, TRUE);

    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(interface->drawing_area), 100);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(interface->drawing_area), 100);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                   (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);

    create_box_for_drawing_area(interface);
}

static void create_field_for_obj_inf(GtkWidget *frame, Interface *interface) {
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
    GtkWidget *file_name_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    GtkWidget *vertex_count_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    GtkWidget *edges_count_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    GtkWidget *v_e_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
    GtkWidget *v_e_frame = gtk_frame_new(NULL);
    GtkWidget *file_load_button = gtk_button_new_from_icon_name("insert-object-symbolic");

    interface->file_name_label = gtk_label_new("");
    interface->vertex_count_label = gtk_label_new("");
    interface->edges_count_label = gtk_label_new("");

    gtk_label_set_xalign(GTK_LABEL(interface->file_name_label), 0.5);
    gtk_label_set_yalign(GTK_LABEL(interface->file_name_label), 0.5);
    gtk_widget_set_hexpand(interface->file_name_label, TRUE);
    gtk_widget_set_hexpand(interface->vertex_count_label, TRUE);
    gtk_widget_set_hexpand(interface->edges_count_label, TRUE);

    // set margin
    gtk_widget_set_margin_start(file_name_box, 20);
    gtk_widget_set_margin_end(file_name_box, 20);
    gtk_widget_set_margin_top(file_name_box, 20);

    gtk_widget_set_margin_start(v_e_frame, 20);
    gtk_widget_set_margin_end(v_e_frame, 20);
    gtk_widget_set_margin_top(v_e_frame, 20);

    gtk_widget_set_margin_start(vertex_count_box, 20);
    gtk_widget_set_margin_top(vertex_count_box, 20);
    gtk_widget_set_margin_end(vertex_count_box, 20);

    gtk_widget_set_margin_start(edges_count_box, 20);
    gtk_widget_set_margin_top(edges_count_box, 10);
    gtk_widget_set_margin_bottom(edges_count_box, 20);
    gtk_widget_set_margin_end(edges_count_box, 20);

    g_signal_connect(GTK_BUTTON(file_load_button), "clicked", G_CALLBACK(load_file), interface);

    // box append
    gtk_box_append(GTK_BOX(file_name_box), interface->file_name_label);
    gtk_box_append(GTK_BOX(file_name_box), file_load_button);
    gtk_box_append(GTK_BOX(vertex_count_box), interface->vertex_count_label);
    gtk_box_append(GTK_BOX(edges_count_box), interface->edges_count_label);
    gtk_box_append(GTK_BOX(v_e_box), vertex_count_box);
    gtk_box_append(GTK_BOX(v_e_box), edges_count_box);

    gtk_box_append(GTK_BOX(main_box), file_name_box);
    gtk_box_append(GTK_BOX(main_box), v_e_frame);

    gtk_frame_set_child(GTK_FRAME(v_e_frame), v_e_box);
    gtk_frame_set_child(GTK_FRAME(frame), main_box);
}

static void create_field_for_record_gif(GtkWidget *frame, Interface *interface) {
    GtkWidget *frame_for_3_buttons = gtk_frame_new(NULL);
    GtkWidget *frame_for_2_buttons = gtk_frame_new("Gif");

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);

    GtkWidget *png_button = gtk_button_new_with_label("png");
    GtkWidget *jpeg_button = gtk_button_new_with_label("jpeg");
    GtkWidget *bmp_button = gtk_button_new_with_label("bmp");
    GtkWidget *play_button = gtk_button_new_from_icon_name("media-playback-start-symbolic");
    GtkWidget *stop_button = gtk_button_new_from_icon_name("media-playback-pause-symbolic");

    g_signal_connect(GTK_BUTTON(play_button), "clicked", G_CALLBACK(create_gif_picture), interface);
    g_signal_connect(GTK_BUTTON(stop_button), "clicked", G_CALLBACK(create_gif), interface);

    g_signal_connect(GTK_BUTTON(png_button), "clicked", G_CALLBACK(save_picture_png), interface);
    g_signal_connect(GTK_BUTTON(jpeg_button), "clicked", G_CALLBACK(save_picture_jpeg), interface);
    g_signal_connect(GTK_BUTTON(bmp_button), "clicked", G_CALLBACK(save_picture_bmp), interface);

    // set margin
    gtk_widget_set_margin_start(frame_for_3_buttons, 10);
    gtk_widget_set_margin_end(frame_for_3_buttons, 10);
    gtk_widget_set_margin_top(frame_for_3_buttons, 10);
    gtk_widget_set_margin_bottom(frame_for_3_buttons, 10);

    gtk_widget_set_margin_start(frame_for_2_buttons, 10);
    gtk_widget_set_margin_end(frame_for_2_buttons, 10);
    gtk_widget_set_margin_top(frame_for_2_buttons, 10);
    gtk_widget_set_margin_bottom(frame_for_2_buttons, 10);

    gtk_widget_set_margin_start(box2, 70);
    gtk_widget_set_margin_top(box2, 10);
    gtk_widget_set_margin_bottom(box2, 10);

    gtk_widget_set_margin_start(box1, 12);
    gtk_widget_set_margin_end(box1, 12);
    gtk_widget_set_margin_top(box1, 12);
    gtk_widget_set_margin_bottom(box1, 12);
    //

    gtk_frame_set_child(GTK_FRAME(frame), main_box);
    gtk_frame_set_child(GTK_FRAME(frame_for_3_buttons), box1);
    gtk_frame_set_child(GTK_FRAME(frame_for_2_buttons), box2);

    gtk_box_append(GTK_BOX(box1), png_button);
    gtk_box_append(GTK_BOX(box1), jpeg_button);
    gtk_box_append(GTK_BOX(box1), bmp_button);

    gtk_box_append(GTK_BOX(box2), play_button);
    gtk_box_append(GTK_BOX(box2), stop_button);

    gtk_box_append(GTK_BOX(main_box), frame_for_3_buttons);
    gtk_box_append(GTK_BOX(main_box), frame_for_2_buttons);
}

static void add_boxes_to_general_box(Interface *interface) {
    GtkWidget *left_frame = gtk_frame_new(NULL);
    GtkWidget *right_top_frame = gtk_frame_new(NULL);
    GtkWidget *right_bottom_frame = gtk_frame_new(NULL);

    gtk_widget_set_hexpand(right_top_frame, TRUE);
    gtk_widget_set_vexpand(right_top_frame, TRUE);
    gtk_widget_set_hexpand(right_bottom_frame, TRUE);
    gtk_widget_set_vexpand(right_bottom_frame, TRUE);

    gtk_widget_set_margin_start(left_frame, 20);
    gtk_widget_set_margin_end(left_frame, 20);
    gtk_widget_set_margin_top(left_frame, 20);
    gtk_widget_set_margin_bottom(left_frame, 20);

    gtk_widget_set_margin_start(right_top_frame, 10);
    gtk_widget_set_margin_end(right_top_frame, 20);
    gtk_widget_set_margin_top(right_top_frame, 20);
    gtk_widget_set_margin_bottom(right_top_frame, 20);

    gtk_widget_set_margin_start(right_bottom_frame, 10);
    gtk_widget_set_margin_end(right_bottom_frame, 20);
    gtk_widget_set_margin_top(right_bottom_frame, 10);
    gtk_widget_set_margin_bottom(right_bottom_frame, 20);

    create_field_for_obj_inf(right_top_frame, interface);
    create_field_for_record_gif(right_bottom_frame, interface);

    gtk_frame_set_child(GTK_FRAME(left_frame), interface->left_box);

    gtk_box_append(GTK_BOX(interface->main_box), left_frame);
    gtk_box_append(GTK_BOX(interface->right_box), right_top_frame);
    gtk_box_append(GTK_BOX(interface->right_box), right_bottom_frame);
    gtk_box_append(GTK_BOX(interface->main_box), interface->right_box);

    gtk_box_append(GTK_BOX(interface->left_box), interface->box_for_drawing_area);
}

static void create_about_program_window(GtkWidget *button, Interface *interface) {
    if (interface->about_program_window == NULL) {
        GtkWidget *header = gtk_header_bar_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);

        interface->about_program_window = gtk_window_new();

        gtk_window_set_titlebar(GTK_WINDOW(interface->about_program_window), header);
        gtk_window_set_title(GTK_WINDOW(interface->about_program_window), "About program");

        g_signal_connect(GTK_WINDOW(interface->about_program_window), "destroy",
                         G_CALLBACK(destroy_about_program_window), interface);
        g_object_add_weak_pointer(G_OBJECT(interface->about_program_window),
                                  (gpointer *)&interface->about_program_window);

        gtk_window_set_default_size(GTK_WINDOW(interface->about_program_window), AWIDTH, AHEIGHT);
        gtk_window_set_resizable(GTK_WINDOW(interface->about_program_window), FALSE);

        gtk_box_set_homogeneous(GTK_BOX(hbox), TRUE);
        gtk_window_set_child(GTK_WINDOW(interface->about_program_window), hbox);

        create_text_view(hbox);

        gtk_widget_show(interface->about_program_window);
    }
}

static void create_main_elements(Interface *interface) { create_drawing_area(interface); }

static void create_buttons_in_menu(GtkWidget *menu_button, Interface *interface) {
    GtkWidget *box_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *about_program_button = gtk_button_new_with_label("About program");
    GtkWidget *popover = gtk_popover_new();

    gtk_widget_set_margin_start(box_menu, 5);
    gtk_widget_set_margin_end(box_menu, 5);

    g_signal_connect(GTK_BUTTON(about_program_button), "clicked", G_CALLBACK(create_about_program_window),
                     interface);

    gtk_box_append(GTK_BOX(box_menu), about_program_button);
    gtk_popover_set_child(GTK_POPOVER(popover), box_menu);

    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), popover);
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_button), "open-menu-symbolic");
}

static void set_text_labels_of_inf_obj(Interface *interface) {
    char edges_count[256] = {0};
    char vertexes_count[256] = {0};

    snprintf(vertexes_count, sizeof(vertexes_count), "Vertexes count: %u", interface->data->count_vert);
    snprintf(edges_count, sizeof(edges_count), "Edges count: %u", interface->data->count_edge);

    gtk_label_set_text(GTK_LABEL(interface->vertex_count_label), vertexes_count);
    gtk_label_set_text(GTK_LABEL(interface->edges_count_label), edges_count);
}

static void set_value_in_entry(GtkWidget *entry, double src_value) {
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    char *value = (char *)malloc(256 * sizeof(char));
    snprintf(value, sizeof(value), "%i", (int)src_value);
    gtk_entry_buffer_set_text(buffer, value, strlen(value));
    free(value);
}

static void on_response(GtkNativeDialog *native, int response, Interface *interface) {
    setlocale(LC_ALL, "C");
    interface->file = NULL;
    interface->text = NULL;
    for (int i = 0; i < 3; i++) {
        interface->vector_position[i] = 0;
        interface->vector_rotation[i] = 0;
        interface->vector_scale[i] = 1;
    }

    if (response == GTK_RESPONSE_ACCEPT) {
        FILE *file;
        char *file_name;
        char *file_suffix;

        interface->file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(native));

        file_name = g_file_get_basename(interface->file);
        file_suffix = file_name + (strlen(file_name) - 3);

        gtk_widget_set_margin_start(interface->box_for_grid, 4);
        gtk_widget_set_margin_end(interface->box_for_grid, 4);

        if (strcmp(file_suffix, "obj") == 0) {  // set filename
            gtk_label_set_text(GTK_LABEL(interface->file_name_label), file_name);
        } else {
            return;
        }

        if (interface->file != NULL) {
            file = fopen(g_file_get_path(interface->file), "r");
        }
        if (file != NULL) {
            if (interface->data != NULL) {
                s21_remove_struct_matrix(interface->data);
                s21_remove_polygons(interface->data);
            }
            interface->data = (st_data *)malloc(sizeof(st_data));
            s21_parsing_file(interface->data, file);

            moving(interface->data, interface->vector_position[0], interface->vector_position[1],
                   interface->vector_position[2]);
            rotation(interface->data, interface->vector_rotation[0], interface->vector_rotation[1],
                     interface->vector_rotation[2]);
            resize(interface->data, interface->vector_scale[0], interface->vector_scale[1],
                   interface->vector_scale[2]);

            for (int i = 0; i < 3; i++) {
                set_value_in_entry(interface->entries_position[i], interface->vector_position[i]);
                set_value_in_entry(interface->entries_rotation[i], interface->vector_rotation[i]);
                set_value_in_entry(interface->entries_scale[i], interface->vector_scale[i]);
            }
            fclose(file);
        }

        set_text_labels_of_inf_obj(interface);
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(interface->drawing_area),
                                       (GtkDrawingAreaDrawFunc)draw_function, interface, NULL);
        g_object_unref(interface->file);
        free(file_name);
    }

    g_object_unref(native);
}

static void load_file(GtkWidget *button, Interface *interface) {
    GtkFileChooserNative *native;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    native = gtk_file_chooser_native_new("Open File", GTK_WINDOW(interface->main_window), action, "_Open",
                                         "_Cancel");

    g_signal_connect(native, "response", G_CALLBACK(on_response), interface);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(native));
}

static void create_header_buttons(Interface *interface) {
    GtkWidget *header = gtk_header_bar_new();
    GtkWidget *settings_button =
        gtk_button_new_from_icon_name("emblem-system-symbolic");  // help-about-symbolic,
                                                                  // open-menu-symbolic, tab-new-symbolic
    GtkWidget *menu_button = gtk_menu_button_new();

    create_buttons_in_menu(menu_button, interface);

    g_signal_connect(GTK_BUTTON(settings_button), "clicked", G_CALLBACK(show_settings_window), interface);
    gtk_window_set_titlebar(GTK_WINDOW(interface->main_window), header);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), settings_button);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), menu_button);
}

static void destroy_main_window(GtkWidget *main_window, Interface *interface) {
    if (interface->settings_window != NULL) {
        gtk_widget_show(interface->settings_window);
        gtk_window_close(GTK_WINDOW(interface->settings_window));
        interface->edge_width_entry = NULL;
        interface->vertex_size_entry = NULL;
        interface->projection_type_cbox = NULL;
        interface->edge_display_type_cbox = NULL;
        interface->save_settings_switch = NULL;
        interface->settings_box = NULL;
        interface->vertex_display_method_cbox = NULL;
        interface->settings_window = NULL;
    }

    if (interface->about_program_window != NULL) {
        gtk_window_close(GTK_WINDOW(interface->about_program_window));
        interface->about_program_window = NULL;
    }

    exit(0);
}

static void create_main_window(GtkApplication *app, Interface *interface) {
    interface->main_window = gtk_application_window_new(interface->app);
    initialize_settings(interface);
    interface->settings_window = NULL;
    interface->about_program_window = NULL;
    interface->file = NULL;
    interface->data = NULL;
    interface->vertex_display_method = (char *)malloc(sizeof(char) * 5);
    interface->picture_value = 350;

    // for (int i = 0; i < 3; i++) {
    //     interface->vector_position[i] = 0;
    //     interface->vector_rotation[i] = 0;
    //     interface->vector_scale[i] = 1;
    // }
    create_header_buttons(interface);

    interface->main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
    interface->left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
    interface->right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);

    gtk_window_set_title(GTK_WINDOW(interface->main_window), "3D Viewer");
    gtk_window_set_default_size(GTK_WINDOW(interface->main_window), WIDTH, HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(interface->main_window), false);
    gtk_window_set_child(GTK_WINDOW(interface->main_window), interface->main_box);

    g_signal_connect(GTK_WINDOW(interface->main_window), "destroy", G_CALLBACK(destroy_main_window),
                     interface);

    create_main_elements(interface);
    add_boxes_to_general_box(interface);
    create_settings_window(interface);
    gtk_widget_show(interface->main_window);
}

int run_app(int argc, char **argv) {
    Interface interface = {0};
    int status;

    interface.app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(interface.app, "activate", G_CALLBACK(create_main_window), &interface);

    status = g_application_run(G_APPLICATION(interface.app), argc, argv);
    g_object_unref(interface.app);

    return status;
}

int main(int argc, char **argv) { return run_app(argc, argv); }

static void initialize_settings(Interface *interface) {
    interface->settings = (Settings *)calloc(1, sizeof(Settings));
    interface->settings->width_edge = 1.0;
    interface->count_file_in_gif = 1;
    read_settings_file(interface->settings);
}
