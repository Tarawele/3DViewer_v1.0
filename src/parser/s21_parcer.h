#ifndef S21_PARCER_H_
#define S21_PARCER_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct result_struct {
  double min[3], max[3];
  int amount_points, amount_connections;
  double *points;
  int *connections;
};

void affine_rotate(int axis, double angle, struct result_struct *result);
void affine_scale(double size, struct result_struct *result);
void affine_shift(int axis, double shift, struct result_struct *result);
void normalize(struct result_struct *result);
void resize_to_one(struct result_struct *data, double size_value);
double find_D_max(struct result_struct *data);

void res_remove(struct result_struct *result);
void res_init(struct result_struct *temp);
char *get_points(char *temp, double *coord);
void max_min_check(struct result_struct *result, int i);
char *get_connections(char *temp, size_t *value);
void conn_add(struct result_struct *result, size_t value);
void values_for_result(size_t *values, int value_counter,
                       struct result_struct *result);
struct result_struct *get_data(char *file_name);

#endif  // S21_PARCER_H_
