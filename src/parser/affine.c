#include "s21_parcer.h"

void resize_to_one(struct result_struct *data, double size_value) {
  double D_max = find_D_max(data);
  double scale = size_value * (size_value) / D_max;

  for (int i = 0; i < data->amount_points; i++) {
    data->points[i] *= scale;
  }
  for (int i = 0; i < 3; i++) {
    data->max[i] *= scale;
    data->min[i] *= scale;
  }
}

double find_D_max(struct result_struct *data) {
  double result = -1E28;
  for (int i = 0; i < 3; i++)
    if (data->max[i] - data->min[i] > result)
      result = data->max[i] - data->min[i];
  return result;
}

void affine_rotate(int axis, double angle, struct result_struct *result) {
  angle = angle * (3.14 / 180.0);
  for (int i = 0; i < result->amount_points; i += 3) {
    int x = -1;
    int y = -1;
    for (int j = 0; j < 3; j++) {
      if (axis != j && (x != -1)) y = i + j;
      if (axis != j && (x == -1)) x = i + j;
    }
    double temp_x = result->points[x], temp_y = result->points[y];
    result->points[x] = cos(angle) * temp_x + sin(angle) * temp_y;
    result->points[y] = -sin(angle) * temp_x + cos(angle) * temp_y;
  }
}

void affine_shift(int axis, double shift, struct result_struct *result) {
  for (int i = 0; i < result->amount_points; i += 3) {
    for (int j = 0; j < 3; j++) {
      if (j == axis) result->points[i + j] += shift;
    }
  }
}

void affine_scale(double size, struct result_struct *result) {
  for (int i = 0; i < result->amount_points; i++) result->points[i] *= size;
}

void normalize(struct result_struct *result) {
  double center[3];
  for (int i = 0; i < 3; i++)
    center[i] = result->min[i] + (result->max[i] - result->min[i]) / 2;
  for (int i = 0; i < result->amount_points; i += 3) {
    for (int j = 0; j < 3; j++) result->points[i + j] -= center[j];
  }
  //     for (int i = 0; i < result->amount_points; i += 3) {
  //        for (int j = 0; j < 3; j++) result->points[i + j] /= result->max[j];
  //    }
}
