/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unity/examples/unity_config.h"
#include "unity/src/unity.h"
#include "common.h"

static void * CJSON_CDECL failing_malloc(size_t size)
{
    (void)size;
    return NULL;
}

/* work around MSVC error C2322: '...' address of dillimport '...' is not static */
static void CJSON_CDECL normal_free(void *pointer)
{
    free(pointer);
}

static cjson_hooks_t failing_hooks = {
    failing_malloc,
    normal_free
};

static void cjson_add_null_should_add_null(void)
{
    cjson_t *root = cjson_create_object();
    cjson_t *null = NULL;

    cjson_add_null_to_object(root, "null");

    TEST_ASSERT_NOT_NULL(null = cjson_get_object_item_case_sensitive(root, "null"));
    TEST_ASSERT_EQUAL_INT(null->type, CJSON_NULL);

    cjson_delete(root);
}

static void cjson_add_null_should_fail_with_null_pointers(void)
{
    cjson_t *root = cjson_create_object();

    TEST_ASSERT_NULL(cjson_add_null_to_object(NULL, "null"));
    TEST_ASSERT_NULL(cjson_add_null_to_object(root, NULL));

    cjson_delete(root);
}

static void cjson_add_null_should_fail_on_allocation_failure(void)
{
    cjson_t *root = cjson_create_object();

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_add_null_to_object(root, "null"));

    cjson_init_hooks(NULL);

    cjson_delete(root);
}

static void cjson_add_true_should_add_true(void)
{
    cjson_t *root = cjson_create_object();
    cjson_t *true_item = NULL;

    cjson_add_true_to_object(root, "true");

    TEST_ASSERT_NOT_NULL(true_item = cjson_get_object_item_case_sensitive(root, "true"));
    TEST_ASSERT_EQUAL_INT(true_item->type, CJSON_TRUE);

    cjson_delete(root);
}

static void cjson_add_true_should_fail_with_null_pointers(void)
{
    cjson_t *root = cjson_create_object();

    TEST_ASSERT_NULL(cjson_add_true_to_object(NULL, "true"));
    TEST_ASSERT_NULL(cjson_add_true_to_object(root, NULL));

    cjson_delete(root);
}

static void cjson_add_true_should_fail_on_allocation_failure(void)
{
    cjson_t *root = cjson_create_object();

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_add_true_to_object(root, "true"));

    cjson_init_hooks(NULL);

    cjson_delete(root);
}

static void cjson_create_int_array_should_fail_on_allocation_failure(void)
{
    int numbers[] = {1, 2, 3};

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_create_int_array(numbers, 3));

    cjson_init_hooks(NULL);
}

static void cjson_create_float_array_should_fail_on_allocation_failure(void)
{
    float numbers[] = {1.0f, 2.0f, 3.0f};

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_create_float_array(numbers, 3));

    cjson_init_hooks(NULL);
}

static void cjson_create_double_array_should_fail_on_allocation_failure(void)
{
    double numbers[] = {1.0, 2.0, 3.0};

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_create_double_array(numbers, 3));

    cjson_init_hooks(NULL);
}

static void cjson_create_string_array_should_fail_on_allocation_failure(void)
{
    const char* strings[] = {"1", "2", "3"};

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_create_string_array(strings, 3));

    cjson_init_hooks(NULL);
}

static void cjson_add_false_should_add_false(void)
{
    cjson_t *root = cjson_create_object();
    cjson_t *false_item = NULL;

    cjson_add_false_to_object(root, "false");

    TEST_ASSERT_NOT_NULL(false_item = cjson_get_object_item_case_sensitive(root, "false"));
    TEST_ASSERT_EQUAL_INT(false_item->type, CJSON_FALSE);

    cjson_delete(root);
}

static void cjson_add_false_should_fail_with_null_pointers(void)
{
    cjson_t *root = cjson_create_object();

    TEST_ASSERT_NULL(cjson_add_false_to_object(NULL, "false"));
    TEST_ASSERT_NULL(cjson_add_false_to_object(root, NULL));

    cjson_delete(root);
}

static void cjson_add_false_should_fail_on_allocation_failure(void)
{
    cjson_t *root = cjson_create_object();

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_add_false_to_object(root, "false"));

    cjson_init_hooks(NULL);

    cjson_delete(root);
}

static void cjson_add_bool_should_add_bool(void)
{
    cjson_t *root = cjson_create_object();
    cjson_t *true_item = NULL;
    cjson_t *false_item = NULL;

    /* true */
    cjson_add_bool_to_object(root, "true", true);
    TEST_ASSERT_NOT_NULL(true_item = cjson_get_object_item_case_sensitive(root, "true"));
    TEST_ASSERT_EQUAL_INT(true_item->type, CJSON_TRUE);

    /* false */
    cjson_add_bool_to_object(root, "false", false);
    TEST_ASSERT_NOT_NULL(false_item = cjson_get_object_item_case_sensitive(root, "false"));
    TEST_ASSERT_EQUAL_INT(false_item->type, CJSON_FALSE);

    cjson_delete(root);
}

static void cjson_add_bool_should_fail_with_null_pointers(void)
{
    cjson_t *root = cjson_create_object();

    TEST_ASSERT_NULL(cjson_add_bool_to_object(NULL, "false", false));
    TEST_ASSERT_NULL(cjson_add_bool_to_object(root, NULL, false));

    cjson_delete(root);
}

static void cjson_add_bool_should_fail_on_allocation_failure(void)
{
    cjson_t *root = cjson_create_object();

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_add_bool_to_object(root, "false", false));

    cjson_init_hooks(NULL);

    cjson_delete(root);
}

static void cjson_add_number_should_add_number(void)
{
    cjson_t *root = cjson_create_object();
    cjson_t *number = NULL;

    cjson_add_number_to_object(root, "number", 42);

    TEST_ASSERT_NOT_NULL(number = cjson_get_object_item_case_sensitive(root, "number"));

    TEST_ASSERT_EQUAL_INT(number->type, CJSON_NUMBER);
    TEST_ASSERT_EQUAL_DOUBLE(number->valuedouble, 42);
    TEST_ASSERT_EQUAL_INT(number->valueint, 42);

    cjson_delete(root);
}

static void cjson_add_number_should_fail_with_null_pointers(void)
{
    cjson_t *root = cjson_create_object();

    TEST_ASSERT_NULL(cjson_add_number_to_object(NULL, "number", 42));
    TEST_ASSERT_NULL(cjson_add_number_to_object(root, NULL, 42));

    cjson_delete(root);
}

static void cjson_add_number_should_fail_on_allocation_failure(void)
{
    cjson_t *root = cjson_create_object();

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_add_number_to_object(root, "number", 42));

    cjson_init_hooks(NULL);

    cjson_delete(root);
}

static void cjson_add_string_should_add_string(void)
{
    cjson_t *root = cjson_create_object();
    cjson_t *string = NULL;

    cjson_add_string_to_object(root, "string", "Hello World!");

    TEST_ASSERT_NOT_NULL(string = cjson_get_object_item_case_sensitive(root, "string"));
    TEST_ASSERT_EQUAL_INT(string->type, CJSON_STRING);
    TEST_ASSERT_EQUAL_STRING(string->valuestring, "Hello World!");

    cjson_delete(root);
}

static void cjson_add_string_should_fail_with_null_pointers(void)
{
    cjson_t *root = cjson_create_object();

    TEST_ASSERT_NULL(cjson_add_string_to_object(NULL, "string", "string"));
    TEST_ASSERT_NULL(cjson_add_string_to_object(root, NULL, "string"));

    cjson_delete(root);
}

static void cjson_add_string_should_fail_on_allocation_failure(void)
{
    cjson_t *root = cjson_create_object();

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_add_string_to_object(root, "string", "string"));

    cjson_init_hooks(NULL);

    cjson_delete(root);
}

static void cjson_add_raw_should_add_raw(void)
{
    cjson_t *root = cjson_create_object();
    cjson_t *raw = NULL;

    cjson_add_raw_to_object(root, "raw", "{}");

    TEST_ASSERT_NOT_NULL(raw = cjson_get_object_item_case_sensitive(root, "raw"));
    TEST_ASSERT_EQUAL_INT(raw->type, CJSON_RAW);
    TEST_ASSERT_EQUAL_STRING(raw->valuestring, "{}");

    cjson_delete(root);
}

static void cjson_add_raw_should_fail_with_null_pointers(void)
{
    cjson_t *root = cjson_create_object();

    TEST_ASSERT_NULL(cjson_add_raw_to_object(NULL, "raw", "{}"));
    TEST_ASSERT_NULL(cjson_add_raw_to_object(root, NULL, "{}"));

    cjson_delete(root);
}

static void cjson_add_raw_should_fail_on_allocation_failure(void)
{
    cjson_t *root = cjson_create_object();

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_add_raw_to_object(root, "raw", "{}"));

    cjson_init_hooks(NULL);

    cjson_delete(root);
}

static void cJSON_add_object_should_add_object(void)
{
    cjson_t *root = cjson_create_object();
    cjson_t *object = NULL;

    cjson_add_object_to_object(root, "object");
    TEST_ASSERT_NOT_NULL(object = cjson_get_object_item_case_sensitive(root, "object"));
    TEST_ASSERT_EQUAL_INT(object->type, CJSON_OBJECT);

    cjson_delete(root);
}

static void cjson_add_object_should_fail_with_null_pointers(void)
{
    cjson_t *root = cjson_create_object();

    TEST_ASSERT_NULL(cjson_add_object_to_object(NULL, "object"));
    TEST_ASSERT_NULL(cjson_add_object_to_object(root, NULL));

    cjson_delete(root);
}

static void cjson_add_object_should_fail_on_allocation_failure(void)
{
    cjson_t *root = cjson_create_object();

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_add_object_to_object(root, "object"));

    cjson_init_hooks(NULL);

    cjson_delete(root);
}

static void cJSON_add_array_should_add_array(void)
{
    cjson_t *root = cjson_create_object();
    cjson_t *array = NULL;

    cjson_add_array_to_object(root, "array");
    TEST_ASSERT_NOT_NULL(array = cjson_get_object_item_case_sensitive(root, "array"));
    TEST_ASSERT_EQUAL_INT(array->type, CJSON_ARRAY);

    cjson_delete(root);
}

static void cjson_add_array_should_fail_with_null_pointers(void)
{
    cjson_t *root = cjson_create_object();

    TEST_ASSERT_NULL(cjson_add_array_to_object(NULL, "array"));
    TEST_ASSERT_NULL(cjson_add_array_to_object(root, NULL));

    cjson_delete(root);
}

static void cjson_add_array_should_fail_on_allocation_failure(void)
{
    cjson_t *root = cjson_create_object();

    cjson_init_hooks(&failing_hooks);

    TEST_ASSERT_NULL(cjson_add_array_to_object(root, "array"));

    cjson_init_hooks(NULL);

    cjson_delete(root);
}

int CJSON_CDECL main(void)
{
    UNITY_BEGIN();

    RUN_TEST(cjson_add_null_should_add_null);
    RUN_TEST(cjson_add_null_should_fail_with_null_pointers);
    RUN_TEST(cjson_add_null_should_fail_on_allocation_failure);

    RUN_TEST(cjson_add_true_should_add_true);
    RUN_TEST(cjson_add_true_should_fail_with_null_pointers);
    RUN_TEST(cjson_add_true_should_fail_on_allocation_failure);

    RUN_TEST(cjson_create_int_array_should_fail_on_allocation_failure);
    RUN_TEST(cjson_create_float_array_should_fail_on_allocation_failure);
    RUN_TEST(cjson_create_double_array_should_fail_on_allocation_failure);
    RUN_TEST(cjson_create_string_array_should_fail_on_allocation_failure);

    RUN_TEST(cjson_add_false_should_add_false);
    RUN_TEST(cjson_add_false_should_fail_with_null_pointers);
    RUN_TEST(cjson_add_false_should_fail_on_allocation_failure);

    RUN_TEST(cjson_add_bool_should_add_bool);
    RUN_TEST(cjson_add_bool_should_fail_with_null_pointers);
    RUN_TEST(cjson_add_bool_should_fail_on_allocation_failure);

    RUN_TEST(cjson_add_number_should_add_number);
    RUN_TEST(cjson_add_number_should_fail_with_null_pointers);
    RUN_TEST(cjson_add_number_should_fail_on_allocation_failure);

    RUN_TEST(cjson_add_string_should_add_string);
    RUN_TEST(cjson_add_string_should_fail_with_null_pointers);
    RUN_TEST(cjson_add_string_should_fail_on_allocation_failure);

    RUN_TEST(cjson_add_raw_should_add_raw);
    RUN_TEST(cjson_add_raw_should_fail_with_null_pointers);
    RUN_TEST(cjson_add_raw_should_fail_on_allocation_failure);

    RUN_TEST(cJSON_add_object_should_add_object);
    RUN_TEST(cjson_add_object_should_fail_with_null_pointers);
    RUN_TEST(cjson_add_object_should_fail_on_allocation_failure);

    RUN_TEST(cJSON_add_array_should_add_array);
    RUN_TEST(cjson_add_array_should_fail_with_null_pointers);
    RUN_TEST(cjson_add_array_should_fail_on_allocation_failure);

    return UNITY_END();
}
