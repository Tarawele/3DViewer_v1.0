#include "s21_parcer.h"

void res_remove(struct result_struct *result) {
  free(result->points);
  free(result->connections);
  free(result);
}

void res_init(struct result_struct *temp) {
  temp->amount_points = 0;
  temp->points = malloc(sizeof(double));

  temp->amount_connections = 0;
  temp->connections = malloc(sizeof(int));

  for (int i = 0; i < 3; i++) {
    temp->min[i] = 0;
    temp->max[i] = 0;
  }
}

char *get_points(char *temp, double *coord) {
  char *point_str = calloc(1024, sizeof(char));
  int i = 0;
  while (*temp != ' ' && *temp != '\n' && *temp != '\0' && *temp != '\r') {
    point_str[i] = *temp;
    temp++;
    i++;
  }
  *coord = strtod(point_str, NULL);
  free(point_str);
  return temp - 1;
}

void max_min_check(struct result_struct *result, int i) {
  if (result->amount_points < 4) {
    result->max[i] = result->points[result->amount_points - 1];
    result->min[i] = result->points[result->amount_points - 1];
  } else {
    result->max[i] =
        (result->max[i] < result->points[result->amount_points - 1])
            ? result->points[result->amount_points - 1]
            : result->max[i];
    result->min[i] =
        (result->min[i] > result->points[result->amount_points - 1])
            ? result->points[result->amount_points - 1]
            : result->min[i];
  }
}

char *get_connections(char *temp, size_t *value) {
  char *buffer = calloc(1024, sizeof(char));
  int i = 0;
  while (*temp == ' ') temp++;
  while (*temp >= '0' && *temp <= '9') {
    buffer[i] = *temp;
    temp++;
    i++;
  }
  if (i) *value = atoi(buffer) - 1;
  if (*temp == '/') {
    while (*temp != ' ' && *temp != '\0' && *temp != '\n' && *temp != '\r')
      temp++;
  }
  free(buffer);
  return temp;
}

void conn_add(struct result_struct *result, size_t value) {
  result->amount_connections++;
  result->connections =
      realloc(result->connections, result->amount_connections * sizeof(int));
  result->connections[result->amount_connections - 1] = value;
}

void values_for_result(size_t *values, int value_counter,
                       struct result_struct *result) {
  for (int i = 0; i < value_counter; i++) {
    conn_add(result, values[i]);
    if (i) {
      conn_add(result, values[i]);
    }
  }
  conn_add(result, values[0]);
}

struct result_struct *get_data(char *file_name) {
  struct result_struct *result = NULL;
  FILE *object_file = fopen(file_name, "r");
  if (object_file) {
    result = malloc(sizeof(struct result_struct));
    res_init(result);
    char *buffer_line = malloc(1024 * sizeof(char));
    while (fgets(buffer_line, 1024, object_file)) {
      size_t length = strlen(buffer_line);
      char *temp = buffer_line;
      if (length > 2 && temp[0] == 'v' && temp[1] == ' ') {
        for (int i = 0; i < 3; i++) {
          result->amount_points++;
          result->points =
              realloc(result->points, result->amount_points * sizeof(double));
          temp =
              get_points(temp + 2, &result->points[result->amount_points - 1]);
          max_min_check(result, i);
        }
      }
      if (length > 2 && temp[0] == 'f' && temp[1] == ' ') {
        temp += 2;
        int value_counter = 0;
        size_t *values = malloc(sizeof(size_t));
        while (*temp != '\n' && *temp != '\0' && *temp != '\r') {
          value_counter++;
          values = realloc(values, value_counter * sizeof(size_t));
          temp = get_connections(temp, &values[value_counter - 1]);
        }
        values_for_result(values, value_counter, result);
        free(values);
      }
    }
    free(buffer_line);
    fclose(object_file);
  }
  resize_to_one(result, 0.9);
  normalize(result);
  return result;
}

// int main() {
//     char c[100] = "/Users/steinbrp/C8_3DViewer_v1.0-1/src/file_obj/Cube.obj";
//     struct result_struct *result = get_data(c);
//     normalize(result);
//     affine_rotate(0, 90, result);
//     affine_shift(1, 10, result);
//     affine_scale(0.1, result);
//     for (size_t i = 0; i < result->amount_points; i+=3)
//         printf("%f %f %f\n", result->points[i], result->points[i + 1],
//                result->points[i + 2]);
//     for (size_t i = 0; i < result->amount_connections; i++) {
//         printf("%ld ", result->connections[i]);
//         if ((i + 1)% 6 == 0) printf("\n");
//     }
//     printf("%ld\n", result->amount_points);
//     printf("%ld\n", result->amount_connections);
//     for (int i = 0; i < 3; i++) {
//         printf("max: %f min: %f\n", result->max[i], result->min[i]);
//     }
//     res_remove(result);

// }
