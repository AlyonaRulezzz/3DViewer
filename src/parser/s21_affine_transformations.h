#ifndef SRC_S21_AFFINE_TRANSFORMATIONS_H_
#define SRC_S21_AFFINE_TRANSFORMATIONS_H_
#include "s21_parsing_file.h"

st_matrix create_af_matrix(double x, double y, double z);
st_matrix create_resize_matrix(double x, double y, double z);
st_matrix create_x_rotation_matrix(double x);
st_matrix create_y_rotation_matrix(double y);
st_matrix create_z_rotation_matrix(double z);
void moving(st_data *data, double x, double y, double z);
void resize(st_data *data, double x, double y, double z);
void rotation(st_data *data, double x, double y, double z);

#endif  // SRC_S21_AFFINE_TRANSFORMATIONS_H_
