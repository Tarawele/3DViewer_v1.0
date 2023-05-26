#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_parcer.h"

void run_testcase(Suite *testcase) {
  static int counter_testcase = 1;

  if (counter_testcase > 1) putchar('\n');
  printf("%s%d%s", "CURRENT TEST: ", counter_testcase, "\n");
  counter_testcase++;

  SRunner *sr = srunner_create(testcase);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  srunner_free(sr);
}

START_TEST(case_parser) {
  struct result_struct *res = get_data("file_obj/Cube.obj");
  int points = 24;
  int conn = 72;
  ck_assert_double_eq_tol(res->min[0], -0.405, 1E-6);
  ck_assert_double_eq_tol(res->min[1], -0.405, 1E-6);
  ck_assert_double_eq_tol(res->min[2], -0.405, 1E-6);
  ck_assert_double_eq_tol(res->max[0], 0.405, 1E-6);
  ck_assert_double_eq_tol(res->max[1], 0.405, 1E-6);
  ck_assert_double_eq_tol(res->max[2], 0.405, 1E-6);

  ck_assert_int_eq(res->amount_connections, conn);
  ck_assert_int_eq(res->amount_points, points);

  affine_rotate(0, 90, res);
  ck_assert_double_eq_tol(res->points[0], -0.405000, 1E-6);
  ck_assert_double_eq_tol(res->points[1], 0.404677, 1E-6);
  ck_assert_double_eq_tol(res->points[2], 0.405322, 1E-6);

  affine_shift(1, 10, res);
  ck_assert_double_eq_tol(res->points[0], -0.405000, 1E-6);
  ck_assert_double_eq_tol(res->points[1], 10.404677, 1E-6);
  ck_assert_double_eq_tol(res->points[2], 0.405322, 1E-6);

  affine_scale(0.1, res);
  ck_assert_double_eq_tol(res->points[0], -0.040500, 1E-6);
  ck_assert_double_eq_tol(res->points[1], 1.040468, 1E-6);
  ck_assert_double_eq_tol(res->points[2], 0.040532, 1E-6);

  res_remove(res);

  res = get_data("file_obj/1.obj");
  ck_assert_int_eq(res->amount_connections, 0);
  ck_assert_int_eq(res->amount_points, 0);
  res_remove(res);
}
END_TEST

Suite *suite_test(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("parser");
  tc = tcase_create("case_parser");

  tcase_add_test(tc, case_parser);
  // tcase_add_test(tc, calc_test_2);

  suite_add_tcase(s, tc);

  return s;
}

void run_tests(void) {
  Suite *list_cases[] = {suite_test(), NULL};

  for (Suite **current_testcase = list_cases; *current_testcase != NULL;
       current_testcase++) {
    run_testcase(*current_testcase);
  }
}

int main(void) {
  run_tests();

  return 0;
}
