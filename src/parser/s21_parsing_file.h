#ifndef SRC_S21_PARSINF_FILE_H_
#define SRC_S21_PARSINF_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix {  // структура матриц
    double **matrix;
    int rows;
    int columns;
} st_matrix;

typedef struct facets {     // хранения полигонов(f)
    int *vert;              // массив номеров вершин
    int count_number_vert;  // кол-во вершин необходимых для соединения
} st_facets;

typedef struct data {       // структура первого парсера (основная)
    unsigned count_vert;    // кол-во вершин
    unsigned count_facets;  // кол-во полигон
    unsigned count_edge;    // кол-во рёбер
    st_matrix *matrix_3d;
    st_facets *polygons;
} st_data;
#include "s21_affine_transformations.h"

st_matrix s21_create_matrix(int rows, int columns);
st_matrix s21_mult_matrix(st_matrix *A, st_matrix *B);
void s21_parsing_file(st_data *data, FILE *file);
void s21_remove_matrix(st_matrix *A);
void s21_counter_f(st_data *data, FILE *file);
void s21_filling_f(st_data *data, FILE *file);
int s21_counter_v(st_data *data, FILE *file);
void s21_filling_v(st_data *data, FILE *file);
void s21_remove_struct_matrix(st_data *data);
void s21_remove_polygons(st_data *data);

#endif  // SRC_S21_PARSINF_FILE_H_
