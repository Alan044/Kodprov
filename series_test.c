#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "series.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_basic(void)
{
  int source[] = {1, 2, 3, 4, 5};

  series_t *s1 = series_create(3, source, 0);
  CU_ASSERT_EQUAL(series_length(s1), 3);
  CU_ASSERT_EQUAL(series_get(s1, 0), 1);
  CU_ASSERT_EQUAL(series_get(s1, 1), 2);
  CU_ASSERT_EQUAL(series_get(s1, 2), 3);
  CU_ASSERT_EQUAL(series_get(s1, 3), -1);

  series_t *s2 = series_create(2, source, 3);
  CU_ASSERT_EQUAL(series_length(s2), 2);
  CU_ASSERT_EQUAL(series_get(s2, 0), 4);
  CU_ASSERT_EQUAL(series_get(s2, 1), 5);
  CU_ASSERT_EQUAL(series_get(s2, 2), -1);

  series_destroy(s1);
  series_destroy(s2);
}

void test_empty(void)
{
  int source[] = {1, 2, 3, 4, 5};
  series_t *s = series_create(0, source, 0);
  CU_ASSERT_EQUAL(series_length(s), 0);
  CU_ASSERT_EQUAL(series_get(s, 0), -1);

  series_destroy(s);
}

void test_waves_monotonic(void)
{
  int source[] = {1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9};

  int offset = 0;
  series_t *s1 = series_next_wave(source, &offset, 12);
  CU_ASSERT_EQUAL(offset, 2);
  CU_ASSERT_EQUAL(series_length(s1), 2);
  CU_ASSERT_EQUAL(series_get(s1, 0), 1);
  CU_ASSERT_EQUAL(series_get(s1, 1), 4);

  series_t *s2 = series_next_wave(source, &offset, 12);
  CU_ASSERT_EQUAL(offset, 5);
  CU_ASSERT_EQUAL(series_length(s2), 3);
  CU_ASSERT_EQUAL(series_get(s2, 0), 1);
  CU_ASSERT_EQUAL(series_get(s2, 1), 5);
  CU_ASSERT_EQUAL(series_get(s2, 2), 9);

  series_t *s3 = series_next_wave(source, &offset, 12);
  CU_ASSERT_EQUAL(offset, 7);
  CU_ASSERT_EQUAL(series_length(s3), 2);
  CU_ASSERT_EQUAL(series_get(s3, 0), 2);
  CU_ASSERT_EQUAL(series_get(s3, 1), 6);

  series_t *s4 = series_next_wave(source, &offset, 12);
  CU_ASSERT_EQUAL(offset, 8);
  CU_ASSERT_EQUAL(series_length(s4), 1);
  CU_ASSERT_EQUAL(series_get(s4, 0), 5);

  series_t *s5 = series_next_wave(source, &offset, 12);
  CU_ASSERT_EQUAL(offset, 12);
  CU_ASSERT_EQUAL(series_length(s5), 4);
  CU_ASSERT_EQUAL(series_get(s5, 0), 3);
  CU_ASSERT_EQUAL(series_get(s5, 1), 5);
  CU_ASSERT_EQUAL(series_get(s5, 2), 8);
  CU_ASSERT_EQUAL(series_get(s5, 3), 9);

  series_t *s6 = series_next_wave(source, &offset, 12);
  CU_ASSERT_EQUAL(offset, 12);
  CU_ASSERT_EQUAL(series_length(s6), 0);

  series_destroy(s1);
  series_destroy(s2);
  series_destroy(s3);
  series_destroy(s4);
  series_destroy(s5);
  series_destroy(s6);
}

void test_waves_repeating(void)
{
  int source[] = {1, 2, 2, 3, 3, 3, 1, 1, 1, 2, 2};

  int offset = 0;
  series_t *s1 = series_next_wave(source, &offset, 11);
  CU_ASSERT_EQUAL(offset, 6);
  CU_ASSERT_EQUAL(series_length(s1), 6);
  CU_ASSERT_EQUAL(series_get(s1, 0), 1);
  CU_ASSERT_EQUAL(series_get(s1, 1), 2);
  CU_ASSERT_EQUAL(series_get(s1, 2), 2);
  CU_ASSERT_EQUAL(series_get(s1, 3), 3);
  CU_ASSERT_EQUAL(series_get(s1, 4), 3);
  CU_ASSERT_EQUAL(series_get(s1, 5), 3);

  series_t *s2 = series_next_wave(source, &offset, 11);
  CU_ASSERT_EQUAL(offset, 11);
  CU_ASSERT_EQUAL(series_length(s2), 5);
  CU_ASSERT_EQUAL(series_get(s2, 0), 1);
  CU_ASSERT_EQUAL(series_get(s2, 1), 1);
  CU_ASSERT_EQUAL(series_get(s2, 2), 1);
  CU_ASSERT_EQUAL(series_get(s2, 3), 2);
  CU_ASSERT_EQUAL(series_get(s2, 4), 2);

  series_t *s3 = series_next_wave(source, &offset, 11);
  CU_ASSERT_EQUAL(offset, 11);
  CU_ASSERT_EQUAL(series_length(s3), 0);

  series_destroy(s1);
  series_destroy(s2);
  series_destroy(s3);
}

void test_long_wave(void) {
  int source[1024] = {0};
  int offset = 0;
  series_t *s = series_next_wave(source, &offset, 1024);
  bool result = true;
  for (int i = 0; i < 1024; ++i){
    result &= series_get(s, i) == 0;
  }
  CU_ASSERT_TRUE(result);

  series_destroy(s);
}

int main()
{
  CU_pSuite list_test_suite = NULL;
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  list_test_suite = CU_add_suite("Series Test Suite", init_suite, clean_suite);
  if (NULL == list_test_suite)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (
    (NULL == CU_add_test(list_test_suite, "Test basic functions", test_basic)) ||
    (NULL == CU_add_test(list_test_suite, "Test empty series", test_empty)) ||
    (NULL == CU_add_test(list_test_suite, "Test monotonic waves", test_waves_monotonic)) ||
    (NULL == CU_add_test(list_test_suite, "Test non-monotonic waves", test_waves_repeating)) ||
    (NULL == CU_add_test(list_test_suite, "Test long wave", test_long_wave)) ||
    false
  )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
