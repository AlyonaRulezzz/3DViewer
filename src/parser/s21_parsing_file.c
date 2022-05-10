#include "s21_parsing_file.h"

st_matrix s21_create_matrix(int rows, int columns) {
    st_matrix neo;
    neo.rows = rows;
    neo.columns = columns;
    neo.matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        neo.matrix[i] = (double *)calloc(columns, sizeof(double));
    }
    return neo;
}

void s21_remove_matrix(st_matrix *A) {
    for (int i = 0; i < A->rows; i++) {
        free(A->matrix[i]);
    }
    free(A->matrix);
}

void s21_remove_struct_matrix(st_data *data) {
    for (int i = 0; i < data->count_vert; ++i) {
        s21_remove_matrix(&data->matrix_3d[i]);
    }
    free(data->matrix_3d);
}

st_matrix s21_mult_matrix(st_matrix *A, st_matrix *B) {
    st_matrix neo;
    neo = s21_create_matrix(A->rows, B->columns);
    for (int i = 0; i < neo.rows; ++i) {
        for (int j = 0; j < neo.columns; ++j) {
            for (int k = 0; k < A->columns;) {
                neo.matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
                k++;
            }
        }
    }

    return neo;
}

int s21_counter_v(st_data *data, FILE *file) {
    if (file != NULL) {
        fseek(file, 0, SEEK_SET);
        data->count_vert = 0;
        data->count_facets = 0;

        while (!feof(file)) {
            size_t len;
            char *line = (char *)calloc(255, sizeof(char));

            getline(&line, &len, file);

            if (strncmp(line, "v ", 2) == 0) {
                data->count_vert++;
            } else if (strncmp(line, "f ", 2) == 0) {
                data->count_facets++;
            }
            free(line);
        }
    }
    data->polygons = (st_facets *)calloc(data->count_facets, sizeof(st_facets));
    data->matrix_3d = (st_matrix *)calloc(data->count_vert, sizeof(st_matrix));

    for (int i = 0; i < data->count_vert; ++i) {
        data->matrix_3d[i] = s21_create_matrix(4, 1);
    }

    return data->count_vert;
}

void s21_filling_v(st_data *data, FILE *file) {
    fseek(file, 0, SEEK_SET);
    int i = 0;
    while (!feof(file)) {
        size_t len;
        char *line = (char *)calloc(255, sizeof(char));
        getline(&line, &len, file);
        if (strncmp(line, "v ", 2) == 0) {
            sscanf(line, "v %lf %lf %lf", &data->matrix_3d[i].matrix[0][0], &data->matrix_3d[i].matrix[1][0],
                   &data->matrix_3d[i].matrix[2][0]);
            data->matrix_3d[i].matrix[3][0] = 1;
            i++;
        }

        free(line);
    }
}

void s21_counter_f(st_data *data, FILE *file) {
    fseek(file, 0, SEEK_SET);
    int j = 0;
    data->count_edge = 0;
    while (!feof(file)) {
        size_t len;
        char *line = (char *)calloc(255, sizeof(char));
        getline(&line, &len, file);

        if (strncmp(line, "f ", 2) == 0) {
            int count_f_num = 0;
            char *pch = strtok(line, "f ");
            while (pch != NULL) {
                int chislo = atoi(pch);
                if (chislo > 0) {
                    count_f_num++;
                }
                pch = strtok(NULL, "f ");
            }
            data->count_edge += count_f_num;
            data->polygons[j].count_number_vert = count_f_num;
            data->polygons[j].vert = (int *)calloc(data->polygons[j].count_number_vert, sizeof(int));
            j++;
        }

        free(line);
    }
}

void s21_filling_f(st_data *data, FILE *file) {
    fseek(file, 0, SEEK_SET);
    int j_v = 0;
    while (!feof(file)) {
        size_t len;
        char *line = (char *)calloc(255, sizeof(char));
        getline(&line, &len, file);
        if (strncmp(line, "f ", 2) == 0) {
            int i = 0;
            char *pch = strtok(line, "f ");
            while (pch != NULL) {
                int chislo = atoi(pch);
                if (chislo != 0) {
                    data->polygons[j_v].vert[i] = chislo;
                }
                pch = strtok(NULL, "f ");
                i++;
            }
            j_v++;
        }
        free(line);
    }
}

void s21_parsing_file(st_data *data, FILE *file) {
    s21_counter_v(data, file);
    s21_filling_v(data, file);
    s21_counter_f(data, file);
    s21_filling_f(data, file);
}

void s21_remove_polygons(st_data *data) {
    for (int i = 0; i < data->count_facets; ++i) {
        free(data->polygons[i].vert);
    }
    free(data->polygons);
}
