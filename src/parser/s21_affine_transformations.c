#include "s21_affine_transformations.h"

#include <math.h>
#define PI 3.1415926535
st_matrix create_af_matrix(double x, double y, double z) {
    st_matrix neo = s21_create_matrix(4, 4);
    for (int i = 0; i < neo.rows; ++i) {
        neo.matrix[i][i] = 1;
    }
    neo.matrix[0][3] = x;
    neo.matrix[1][3] = y;
    neo.matrix[2][3] = z;
    return neo;
}
st_matrix create_resize_matrix(double x, double y, double z) {
    st_matrix neo = s21_create_matrix(4, 4);
    neo.matrix[0][0] = x;
    neo.matrix[1][1] = y;
    neo.matrix[2][2] = z;
    neo.matrix[3][3] = 1;
    return neo;
}
st_matrix create_x_rotation_matrix(double x) {
    st_matrix neo = s21_create_matrix(4, 4);
    x = (x * PI) / 180;
    neo.matrix[0][0] = 1;
    neo.matrix[1][1] = cos(x);
    neo.matrix[1][2] = sin(x);
    neo.matrix[2][2] = cos(x);
    neo.matrix[2][1] = -1 * sin(x);
    neo.matrix[3][3] = 1;
    return neo;
}
st_matrix create_y_rotation_matrix(double y) {
    st_matrix neo = s21_create_matrix(4, 4);
    y = (y * PI) / 180;
    neo.matrix[0][0] = cos(y);
    neo.matrix[1][1] = 1;
    neo.matrix[0][2] = -1 * sin(y);
    neo.matrix[2][2] = cos(y);
    neo.matrix[2][0] = sin(y);
    neo.matrix[3][3] = 1;
    return neo;
}
st_matrix create_z_rotation_matrix(double z) {
    st_matrix neo = s21_create_matrix(4, 4);
    z = (z * PI) / 180;
    neo.matrix[0][0] = cos(z);
    neo.matrix[1][1] = cos(z);
    neo.matrix[0][1] = sin(z);
    neo.matrix[2][2] = 1;
    neo.matrix[1][0] = -1 * sin(z);
    neo.matrix[3][3] = 1;
    return neo;
}

void moving(st_data *data, double x, double y, double z) { // 0
    st_matrix neo = create_af_matrix(x, y, z);
    for (int i = 0; i < data->count_vert; ++i) {
        st_matrix temp = data->matrix_3d[i];
        data->matrix_3d[i] = s21_mult_matrix(&neo, &(data->matrix_3d[i]));
        s21_remove_matrix(&temp);
    }
    s21_remove_matrix(&neo);
}

void resize(st_data *data, double x, double y, double z) {
    st_matrix neo = create_resize_matrix(x, y, z);
    for (int i = 0; i < data->count_vert; ++i) {
        st_matrix temp = data->matrix_3d[i];
        data->matrix_3d[i] = s21_mult_matrix(&neo, &(data->matrix_3d[i]));
        s21_remove_matrix(&temp);
    }
    s21_remove_matrix(&neo);
}

void rotation(st_data *data, double x, double y, double z) {
    st_matrix x_matrix = create_x_rotation_matrix(x);
    st_matrix y_matrix = create_y_rotation_matrix(y);
    st_matrix z_matrix = create_z_rotation_matrix(z);
    st_matrix temp_res = s21_mult_matrix(&x_matrix, &y_matrix);
    st_matrix res = s21_mult_matrix(&temp_res, &z_matrix);
    for (int i = 0; i < data->count_vert; ++i) {
        st_matrix temp = data->matrix_3d[i];
        data->matrix_3d[i] = s21_mult_matrix(&res, &(data->matrix_3d[i]));
        s21_remove_matrix(&temp);
    }

    s21_remove_matrix(&x_matrix);
    s21_remove_matrix(&y_matrix);
    s21_remove_matrix(&z_matrix);
    s21_remove_matrix(&temp_res);
    s21_remove_matrix(&res);
}
