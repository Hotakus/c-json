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

static void cjson_array_foreach_should_loop_over_arrays(void)
{
    cjson_t array[1];
    cjson_t elements[10];
    cjson_t *element_pointer = NULL;
    size_t i = 0;

    memset(array, 0, sizeof(array));
    memset(elements, 0, sizeof(elements));

    /* create array */
    array[0].child = &elements[0];
    elements[0].prev = NULL;
    elements[9].next = NULL;
    for (i = 0; i < 9; i++)
    {
        elements[i].next = &elements[i + 1];
        elements[i + 1].prev = &elements[i];
    }

    i = 0;
    CJSON_ARRAY_FOREACH(element_pointer, array)
    {
        TEST_ASSERT_TRUE_MESSAGE(element_pointer == &elements[i], "Not iterating over array properly");
        i++;
    }
}

static void cjson_array_foreach_should_not_dereference_null_pointer(void)
{
    cjson_t *array = NULL;
    cjson_t *element = NULL;
    CJSON_ARRAY_FOREACH(element, array);
}

static void cjson_get_object_item_should_get_object_items(void)
{
    cjson_t *item = NULL;
    cjson_t *found = NULL;

    item = cjson_parse("{\"one\":1, \"Two\":2, \"tHree\":3}");

    found = cjson_get_object_item(NULL, "test");
    TEST_ASSERT_NULL_MESSAGE(found, "Failed to fail on NULL pointer.");

    found = cjson_get_object_item(item, NULL);
    TEST_ASSERT_NULL_MESSAGE(found, "Failed to fail on NULL string.");

    found = cjson_get_object_item(item, "one");
    TEST_ASSERT_NOT_NULL_MESSAGE(found, "Failed to find first item.");
    TEST_ASSERT_EQUAL_DOUBLE(found->valuedouble, 1);

    found = cjson_get_object_item(item, "tWo");
    TEST_ASSERT_NOT_NULL_MESSAGE(found, "Failed to find first item.");
    TEST_ASSERT_EQUAL_DOUBLE(found->valuedouble, 2);

    found = cjson_get_object_item(item, "three");
    TEST_ASSERT_NOT_NULL_MESSAGE(found, "Failed to find item.");
    TEST_ASSERT_EQUAL_DOUBLE(found->valuedouble, 3);

    found = cjson_get_object_item(item, "four");
    TEST_ASSERT_NULL_MESSAGE(found, "Should not find something that isn't there.");

    cjson_delete(item);
}

static void cjson_get_object_item_case_sensitive_should_get_object_items(void)
{
    cjson_t *item = NULL;
    cjson_t *found = NULL;

    item = cjson_parse("{\"one\":1, \"Two\":2, \"tHree\":3}");

    found = cjson_get_object_item_case_sensitive(NULL, "test");
    TEST_ASSERT_NULL_MESSAGE(found, "Failed to fail on NULL pointer.");

    found = cjson_get_object_item_case_sensitive(item, NULL);
    TEST_ASSERT_NULL_MESSAGE(found, "Failed to fail on NULL string.");

    found = cjson_get_object_item_case_sensitive(item, "one");
    TEST_ASSERT_NOT_NULL_MESSAGE(found, "Failed to find first item.");
    TEST_ASSERT_EQUAL_DOUBLE(found->valuedouble, 1);

    found = cjson_get_object_item_case_sensitive(item, "Two");
    TEST_ASSERT_NOT_NULL_MESSAGE(found, "Failed to find first item.");
    TEST_ASSERT_EQUAL_DOUBLE(found->valuedouble, 2);

    found = cjson_get_object_item_case_sensitive(item, "tHree");
    TEST_ASSERT_NOT_NULL_MESSAGE(found, "Failed to find item.");
    TEST_ASSERT_EQUAL_DOUBLE(found->valuedouble, 3);

    found = cjson_get_object_item_case_sensitive(item, "One");
    TEST_ASSERT_NULL_MESSAGE(found, "Should not find something that isn't there.");

    cjson_delete(item);
}

static void cjson_get_object_item_should_not_crash_with_array(void)
{
    cjson_t *array = NULL;
    cjson_t *found = NULL;
    array = cjson_parse("[1]");

    found = cjson_get_object_item(array, "name");
    TEST_ASSERT_NULL(found);

    cjson_delete(array);
}

static void cjson_get_object_item_case_sensitive_should_not_crash_with_array(void)
{
    cjson_t *array = NULL;
    cjson_t *found = NULL;
    array = cjson_parse("[1]");

    found = cjson_get_object_item_case_sensitive(array, "name");
    TEST_ASSERT_NULL(found);

    cjson_delete(array);
}

static void typecheck_functions_should_check_type(void)
{
    cjson_t invalid[1];
    cjson_t item[1];
    invalid->type = CJSON_INVALID;
    invalid->type |= CJSON_STRING_IS_CONST;
    item->type = CJSON_FALSE;
    item->type |= CJSON_STRING_IS_CONST;

    TEST_ASSERT_FALSE(cjson_is_invalid(NULL));
    TEST_ASSERT_FALSE(cjson_is_invalid(item));
    TEST_ASSERT_TRUE(cjson_is_invalid(invalid));

    item->type = CJSON_FALSE | CJSON_STRING_IS_CONST;
    TEST_ASSERT_FALSE(cjson_is_false(NULL));
    TEST_ASSERT_FALSE(cjson_is_false(invalid));
    TEST_ASSERT_TRUE(cjson_is_false(item));
    TEST_ASSERT_TRUE(cjson_is_bool(item));

    item->type = CJSON_TRUE | CJSON_STRING_IS_CONST;
    TEST_ASSERT_FALSE(cjson_is_true(NULL));
    TEST_ASSERT_FALSE(cjson_is_true(invalid));
    TEST_ASSERT_TRUE(cjson_is_true(item));
    TEST_ASSERT_TRUE(cjson_is_bool(item));

    item->type = CJSON_NULL | CJSON_STRING_IS_CONST;
    TEST_ASSERT_FALSE(cjson_is_null(NULL));
    TEST_ASSERT_FALSE(cjson_is_null(invalid));
    TEST_ASSERT_TRUE(cjson_is_null(item));

    item->type = CJSON_NUMBER | CJSON_STRING_IS_CONST;
    TEST_ASSERT_FALSE(cjson_is_number(NULL));
    TEST_ASSERT_FALSE(cjson_is_number(invalid));
    TEST_ASSERT_TRUE(cjson_is_number(item));

    item->type = CJSON_STRING | CJSON_STRING_IS_CONST;
    TEST_ASSERT_FALSE(cjson_is_string(NULL));
    TEST_ASSERT_FALSE(cjson_is_string(invalid));
    TEST_ASSERT_TRUE(cjson_is_string(item));

    item->type = CJSON_ARRAY | CJSON_STRING_IS_CONST;
    TEST_ASSERT_FALSE(cjson_is_array(NULL));
    TEST_ASSERT_FALSE(cjson_is_array(invalid));
    TEST_ASSERT_TRUE(cjson_is_array(item));

    item->type = CJSON_OBJECT | CJSON_STRING_IS_CONST;
    TEST_ASSERT_FALSE(cjson_is_object(NULL));
    TEST_ASSERT_FALSE(cjson_is_object(invalid));
    TEST_ASSERT_TRUE(cjson_is_object(item));

    item->type = CJSON_RAW | CJSON_STRING_IS_CONST;
    TEST_ASSERT_FALSE(cjson_is_raw(NULL));
    TEST_ASSERT_FALSE(cjson_is_raw(invalid));
    TEST_ASSERT_TRUE(cjson_is_raw(item));
}

static void cjson_should_not_parse_to_deeply_nested_jsons(void)
{
    char deep_json[CJSON_NESTING_LIMIT + 1];
    size_t position = 0;

    for (position = 0; position < sizeof(deep_json); position++)
    {
        deep_json[position] = '[';
    }
    deep_json[sizeof(deep_json) - 1] = '\0';

    TEST_ASSERT_NULL_MESSAGE(cjson_parse(deep_json), "To deep JSONs should not be parsed.");
}

static void cjson_set_number_value_should_set_numbers(void)
{
    cjson_t number[1] = {{NULL, NULL, NULL, CJSON_NUMBER, NULL, 0, 0, NULL}};

    CJSON_SET_NUMBER_VALUE(number, 1.5);
    TEST_ASSERT_EQUAL(1, number->valueint);
    TEST_ASSERT_EQUAL_DOUBLE(1.5, number->valuedouble);

    CJSON_SET_NUMBER_VALUE(number, -1.5);
    TEST_ASSERT_EQUAL(-1, number->valueint);
    TEST_ASSERT_EQUAL_DOUBLE(-1.5, number->valuedouble);

    CJSON_SET_NUMBER_VALUE(number, 1 + (double)INT_MAX);
    TEST_ASSERT_EQUAL(INT_MAX, number->valueint);
    TEST_ASSERT_EQUAL_DOUBLE(1 + (double)INT_MAX, number->valuedouble);

    CJSON_SET_NUMBER_VALUE(number, -1 + (double)INT_MIN);
    TEST_ASSERT_EQUAL(INT_MIN, number->valueint);
    TEST_ASSERT_EQUAL_DOUBLE(-1 + (double)INT_MIN, number->valuedouble);
}

static void cjson_detach_item_via_pointer_should_detach_items(void)
{
    cjson_t list[4];
    cjson_t parent[1];

    memset(list, '\0', sizeof(list));

    /* link the list */
    list[0].next = &(list[1]);
    list[1].next = &(list[2]);
    list[2].next = &(list[3]);

    list[3].prev = &(list[2]);
    list[2].prev = &(list[1]);
    list[1].prev = &(list[0]);
    list[0].prev = &(list[3]);

    parent->child = &list[0];

    /* detach in the middle (list[1]) */
    TEST_ASSERT_TRUE_MESSAGE(cjson_detach_item_via_pointer(parent, &(list[1])) == &(list[1]), "Failed to detach in the middle.");
    TEST_ASSERT_TRUE_MESSAGE((list[1].prev == NULL) && (list[1].next == NULL), "Didn't set pointers of detached item to NULL.");
    TEST_ASSERT_TRUE((list[0].next == &(list[2])) && (list[2].prev == &(list[0])));

    /* detach beginning (list[0]) */
    TEST_ASSERT_TRUE_MESSAGE(cjson_detach_item_via_pointer(parent, &(list[0])) == &(list[0]), "Failed to detach beginning.");
    TEST_ASSERT_TRUE_MESSAGE((list[0].prev == NULL) && (list[0].next == NULL), "Didn't set pointers of detached item to NULL.");
    TEST_ASSERT_TRUE_MESSAGE((list[2].prev == &(list[3])) && (parent->child == &(list[2])), "Didn't set the new beginning.");

    /* detach end (list[3])*/
    TEST_ASSERT_TRUE_MESSAGE(cjson_detach_item_via_pointer(parent, &(list[3])) == &(list[3]), "Failed to detach end.");
    TEST_ASSERT_TRUE_MESSAGE((list[3].prev == NULL) && (list[3].next == NULL), "Didn't set pointers of detached item to NULL.");
    TEST_ASSERT_TRUE_MESSAGE((list[2].next == NULL) && (parent->child == &(list[2])), "Didn't set the new end");

    /* detach single item (list[2]) */
    TEST_ASSERT_TRUE_MESSAGE(cjson_detach_item_via_pointer(parent, &list[2]) == &list[2], "Failed to detach single item.");
    TEST_ASSERT_TRUE_MESSAGE((list[2].prev == NULL) && (list[2].next == NULL), "Didn't set pointers of detached item to NULL.");
    TEST_ASSERT_NULL_MESSAGE(parent->child, "Child of the parent wasn't set to NULL.");
}

static void cjson_replace_item_via_pointer_should_replace_items(void)
{
    cjson_t replacements[3];
    cjson_t *beginning = NULL;
    cjson_t *middle = NULL;
    cjson_t *end = NULL;
    cjson_t *array = NULL;

    beginning = cjson_create_null();
    TEST_ASSERT_NOT_NULL(beginning);
    middle = cjson_create_null();
    TEST_ASSERT_NOT_NULL(middle);
    end = cjson_create_null();
    TEST_ASSERT_NOT_NULL(end);

    array = cjson_create_array();
    TEST_ASSERT_NOT_NULL(array);

    cjson_add_item_to_array(array, beginning);
    cjson_add_item_to_array(array, middle);
    cjson_add_item_to_array(array, end);

    memset(replacements, '\0', sizeof(replacements));

    /* replace beginning */
    TEST_ASSERT_TRUE(cjson_replace_item_via_pointer(array, beginning, &(replacements[0])));
    TEST_ASSERT_TRUE(replacements[0].prev == end);
    TEST_ASSERT_TRUE(replacements[0].next == middle);
    TEST_ASSERT_TRUE(middle->prev == &(replacements[0]));
    TEST_ASSERT_TRUE(array->child == &(replacements[0]));

    /* replace middle */
    TEST_ASSERT_TRUE(cjson_replace_item_via_pointer(array, middle, &(replacements[1])));
    TEST_ASSERT_TRUE(replacements[1].prev == &(replacements[0]));
    TEST_ASSERT_TRUE(replacements[1].next == end);
    TEST_ASSERT_TRUE(end->prev == &(replacements[1]));

    /* replace end */
    TEST_ASSERT_TRUE(cjson_replace_item_via_pointer(array, end, &(replacements[2])));
    TEST_ASSERT_TRUE(replacements[2].prev == &(replacements[1]));
    TEST_ASSERT_NULL(replacements[2].next);
    TEST_ASSERT_TRUE(replacements[1].next == &(replacements[2]));

    cjson_free(array);
}

static void cjson_replace_item_in_object_should_preserve_name(void)
{
    cjson_t root[1] = {{NULL, NULL, NULL, 0, NULL, 0, 0, NULL}};
    cjson_t *child = NULL;
    cjson_t *replacement = NULL;
    cjson_bool_t flag = false;

    child = cjson_create_number(1);
    TEST_ASSERT_NOT_NULL(child);
    replacement = cjson_create_number(2);
    TEST_ASSERT_NOT_NULL(replacement);

    flag = cjson_add_item_to_object(root, "child", child);
    TEST_ASSERT_TRUE_MESSAGE(flag, "add item to object failed");
    cjson_replace_item_in_object(root, "child", replacement);

    TEST_ASSERT_TRUE(root->child == replacement);
    TEST_ASSERT_EQUAL_STRING("child", replacement->string);

    cjson_delete(replacement);
}

static void cjson_functions_should_not_crash_with_null_pointers(void)
{
    char buffer[10];
    cjson_t *item = cjson_create_string("item");

    cjson_init_hooks(NULL);
    TEST_ASSERT_NULL(cjson_parse(NULL));
    TEST_ASSERT_NULL(cjson_parse_with_opts(NULL, NULL, true));
    TEST_ASSERT_NULL(cjson_print(NULL));
    TEST_ASSERT_NULL(cjson_print_unformatted(NULL));
    TEST_ASSERT_NULL(cjson_print_buffered(NULL, 10, true));
    TEST_ASSERT_FALSE(cjson_print_preallocated(NULL, buffer, sizeof(buffer), true));
    TEST_ASSERT_FALSE(cjson_print_preallocated(item, NULL, 1, true));
    cjson_delete(NULL);
    cjson_get_array_size(NULL);
    TEST_ASSERT_NULL(cjson_get_array_item(NULL, 0));
    TEST_ASSERT_NULL(cjson_get_object_item(NULL, "item"));
    TEST_ASSERT_NULL(cjson_get_object_item(item, NULL));
    TEST_ASSERT_NULL(cjson_get_object_item_case_sensitive(NULL, "item"));
    TEST_ASSERT_NULL(cjson_get_object_item_case_sensitive(item, NULL));
    TEST_ASSERT_FALSE(cjson_has_object_item(NULL, "item"));
    TEST_ASSERT_FALSE(cjson_has_object_item(item, NULL));
    TEST_ASSERT_FALSE(cjson_is_invalid(NULL));
    TEST_ASSERT_FALSE(cjson_is_false(NULL));
    TEST_ASSERT_FALSE(cjson_is_true(NULL));
    TEST_ASSERT_FALSE(cjson_is_bool(NULL));
    TEST_ASSERT_FALSE(cjson_is_null(NULL));
    TEST_ASSERT_FALSE(cjson_is_number(NULL));
    TEST_ASSERT_FALSE(cjson_is_string(NULL));
    TEST_ASSERT_FALSE(cjson_is_array(NULL));
    TEST_ASSERT_FALSE(cjson_is_object(NULL));
    TEST_ASSERT_FALSE(cjson_is_raw(NULL));
    TEST_ASSERT_NULL(cjson_create_string(NULL));
    TEST_ASSERT_NULL(cjson_create_raw(NULL));
    TEST_ASSERT_NULL(cjson_create_int_array(NULL, 10));
    TEST_ASSERT_NULL(cjson_create_float_array(NULL, 10));
    TEST_ASSERT_NULL(cjson_create_double_array(NULL, 10));
    TEST_ASSERT_NULL(cjson_create_string_array(NULL, 10));
    cjson_add_item_to_array(NULL, item);
    cjson_add_item_to_array(item, NULL);
    cjson_add_item_to_object(item, "item", NULL);
    cjson_add_item_to_object(item, NULL, item);
    cjson_add_item_to_object(NULL, "item", item);
    cjson_add_item_to_object_cs(item, "item", NULL);
    cjson_add_item_to_object_cs(item, NULL, item);
    cjson_add_item_to_object_cs(NULL, "item", item);
    cjson_add_item_reference_to_array(NULL, item);
    cjson_add_item_reference_to_array(item, NULL);
    cjson_add_item_reference_to_object(item, "item", NULL);
    cjson_add_item_reference_to_object(item, NULL, item);
    cjson_add_item_reference_to_object(NULL, "item", item);
    TEST_ASSERT_NULL(cjson_detach_item_via_pointer(NULL, item));
    TEST_ASSERT_NULL(cjson_detach_item_via_pointer(item, NULL));
    TEST_ASSERT_NULL(cjson_detach_item_from_array(NULL, 0));
    cjson_delete_item_from_array(NULL, 0);
    TEST_ASSERT_NULL(cjson_detach_item_from_object(NULL, "item"));
    TEST_ASSERT_NULL(cjson_detach_item_from_object(item, NULL));
    TEST_ASSERT_NULL(cjson_detach_item_from_object_case_sensitive(NULL, "item"));
    TEST_ASSERT_NULL(cjson_detach_item_from_object_case_sensitive(item, NULL));
    cjson_delete_item_from_object(NULL, "item");
    cjson_delete_item_from_object(item, NULL);
    cjson_delete_item_from_object_case_sensitive(NULL, "item");
    cjson_delete_item_from_object_case_sensitive(item, NULL);
    TEST_ASSERT_FALSE(cjson_insert_item_in_array(NULL, 0, item));
    TEST_ASSERT_FALSE(cjson_insert_item_in_array(item, 0, NULL));
    TEST_ASSERT_FALSE(cjson_replace_item_via_pointer(NULL, item, item));
    TEST_ASSERT_FALSE(cjson_replace_item_via_pointer(item, NULL, item));
    TEST_ASSERT_FALSE(cjson_replace_item_via_pointer(item, item, NULL));
    TEST_ASSERT_FALSE(cjson_replace_item_in_array(item, 0, NULL));
    TEST_ASSERT_FALSE(cjson_replace_item_in_array(NULL, 0, item));
    TEST_ASSERT_FALSE(cjson_replace_item_in_object(NULL, "item", item));
    TEST_ASSERT_FALSE(cjson_replace_item_in_object(item, NULL, item));
    TEST_ASSERT_FALSE(cjson_replace_item_in_object(item, "item", NULL));
    TEST_ASSERT_FALSE(cjson_replace_item_in_object_case_sensitive(NULL, "item", item));
    TEST_ASSERT_FALSE(cjson_replace_item_in_object_case_sensitive(item, NULL, item));
    TEST_ASSERT_FALSE(cjson_replace_item_in_object_case_sensitive(item, "item", NULL));
    TEST_ASSERT_NULL(cjson_duplicate(NULL, true));
    TEST_ASSERT_FALSE(cjson_compare(item, NULL, false));
    TEST_ASSERT_FALSE(cjson_compare(NULL, item, false));
    cjson_minify(NULL);
    /* skipped because it is only used via a macro that checks for NULL */
    /* cJSON_SetNumberHelper(NULL, 0); */

    cjson_delete(item);
}

static void *CJSON_CDECL failing_realloc(void *pointer, size_t size)
{
    (void)size;
    (void)pointer;
    return NULL;
}

static void ensure_should_fail_on_failed_realloc(void)
{
    printbuffer buffer = {NULL, 10, 0, 0, false, false, {&malloc, &free, &failing_realloc}};
    buffer.buffer = (unsigned char *)malloc(100);
    TEST_ASSERT_NOT_NULL(buffer.buffer);

    TEST_ASSERT_NULL_MESSAGE(ensure(&buffer, 200), "Ensure didn't fail with failing realloc.");
}

static void skip_utf8_bom_should_skip_bom(void)
{
    const unsigned char string[] = "\xEF\xBB\xBF{}";
    parse_buffer buffer = {0, 0, 0, 0, {0, 0, 0}};
    buffer.content = string;
    buffer.length = sizeof(string);
    buffer.hooks = global_hooks;

    TEST_ASSERT_TRUE(skip_utf8_bom(&buffer) == &buffer);
    TEST_ASSERT_EQUAL_UINT(3U, (unsigned int)buffer.offset);
}

static void skip_utf8_bom_should_not_skip_bom_if_not_at_beginning(void)
{
    const unsigned char string[] = " \xEF\xBB\xBF{}";
    parse_buffer buffer = {0, 0, 0, 0, {0, 0, 0}};
    buffer.content = string;
    buffer.length = sizeof(string);
    buffer.hooks = global_hooks;
    buffer.offset = 1;

    TEST_ASSERT_NULL(skip_utf8_bom(&buffer));
}

static void cjson_get_string_value_should_get_a_string(void)
{
    cjson_t *string = cjson_create_string("test");
    cjson_t *number = cjson_create_number(1);

    TEST_ASSERT_TRUE(cjson_get_string_value(string) == string->valuestring);
    TEST_ASSERT_NULL(cjson_get_string_value(number));
    TEST_ASSERT_NULL(cjson_get_string_value(NULL));

    cjson_delete(number);
    cjson_delete(string);
}

static void cjson_get_number_value_should_get_a_number(void)
{
    cjson_t *string = cjson_create_string("test");
    cjson_t *number = cjson_create_number(1);

    TEST_ASSERT_EQUAL_DOUBLE(cjson_get_number_value(number), number->valuedouble);
    TEST_ASSERT_DOUBLE_IS_NAN(cjson_get_number_value(string));
    TEST_ASSERT_DOUBLE_IS_NAN(cjson_get_number_value(NULL));

    cjson_delete(number);
    cjson_delete(string);
}

static void cjson_create_string_reference_should_create_a_string_reference(void)
{
    const char *string = "I am a string!";

    cjson_t *string_reference = cjson_create_string_reference(string);
    TEST_ASSERT_TRUE(string_reference->valuestring == string);
    TEST_ASSERT_EQUAL_INT(CJSON_IS_REFERENCE | CJSON_STRING, string_reference->type);

    cjson_delete(string_reference);
}

static void cjson_create_object_reference_should_create_an_object_reference(void)
{
    cjson_t *number_reference = NULL;
    cjson_t *number_object = cjson_create_object();
    cjson_t *number = cjson_create_number(42);
    const char key[] = "number";

    TEST_ASSERT_TRUE(cjson_is_number(number));
    TEST_ASSERT_TRUE(cjson_is_object(number_object));
    cjson_add_item_to_object_cs(number_object, key, number);

    number_reference = cjson_create_object_reference(number);
    TEST_ASSERT_TRUE(number_reference->child == number);
    TEST_ASSERT_EQUAL_INT(CJSON_OBJECT | CJSON_IS_REFERENCE, number_reference->type);

    cjson_delete(number_object);
    cjson_delete(number_reference);
}

static void cjson_create_array_reference_should_create_an_array_reference(void)
{
    cjson_t *number_reference = NULL;
    cjson_t *number_array = cjson_create_array();
    cjson_t *number = cjson_create_number(42);

    TEST_ASSERT_TRUE(cjson_is_number(number));
    TEST_ASSERT_TRUE(cjson_is_array(number_array));
    cjson_add_item_to_array(number_array, number);

    number_reference = cjson_create_array_reference(number);
    TEST_ASSERT_TRUE(number_reference->child == number);
    TEST_ASSERT_EQUAL_INT(CJSON_ARRAY | CJSON_IS_REFERENCE, number_reference->type);

    cjson_delete(number_array);
    cjson_delete(number_reference);
}

static void cjson_add_item_to_object_or_array_should_not_add_itself(void)
{
    cjson_t *object = cjson_create_object();
    cjson_t *array = cjson_create_array();
    cjson_bool_t flag = false;

    flag = cjson_add_item_to_object(object, "key", object);
    TEST_ASSERT_FALSE_MESSAGE(flag, "add an object to itself should fail");

    flag = cjson_add_item_to_array(array, array);
    TEST_ASSERT_FALSE_MESSAGE(flag, "add an array to itself should fail");

    cjson_delete(object);
    cjson_delete(array);
}

static void cjson_add_item_to_object_should_not_use_after_free_when_string_is_aliased(void)
{
    cjson_t *object = cjson_create_object();
    cjson_t *number = cjson_create_number(42);
    char *name = (char *)cJSON_strdup((const unsigned char *)"number", &global_hooks);

    TEST_ASSERT_NOT_NULL(object);
    TEST_ASSERT_NOT_NULL(number);
    TEST_ASSERT_NOT_NULL(name);

    number->string = name;

    /* The following should not have a use after free
     * that would show up in valgrind or with AddressSanitizer */
    cjson_add_item_to_object(object, number->string, number);

    cjson_delete(object);
}

static void cjson_delete_item_from_array_should_not_broken_list_structure(void)
{
    const char expected_json1[] = "{\"rd\":[{\"a\":\"123\"}]}";
    const char expected_json2[] = "{\"rd\":[{\"a\":\"123\"},{\"b\":\"456\"}]}";
    const char expected_json3[] = "{\"rd\":[{\"b\":\"456\"}]}";
    char *str1 = NULL;
    char *str2 = NULL;
    char *str3 = NULL;

    cjson_t *root = cjson_parse("{}");

    cjson_t *array = cjson_add_array_to_object(root, "rd");
    cjson_t *item1 = cjson_parse("{\"a\":\"123\"}");
    cjson_t *item2 = cjson_parse("{\"b\":\"456\"}");

    cjson_add_item_to_array(array, item1);
    str1 = cjson_print_unformatted(root);
    TEST_ASSERT_EQUAL_STRING(expected_json1, str1);
    free(str1);

    cjson_add_item_to_array(array, item2);
    str2 = cjson_print_unformatted(root);
    TEST_ASSERT_EQUAL_STRING(expected_json2, str2);
    free(str2);

    /* this should not broken list structure */
    cjson_delete_item_from_array(array, 0);
    str3 = cjson_print_unformatted(root);
    TEST_ASSERT_EQUAL_STRING(expected_json3, str3);
    free(str3);

    cjson_delete(root);
}

static void cjson_set_valuestring_to_object_should_not_leak_memory(void)
{
    cjson_t *root = cjson_parse("{}");
    const char *stringvalue = "valuestring could be changed safely";
    const char *reference_valuestring = "reference item should be freed by yourself";
    const char *short_valuestring = "shorter valuestring";
    const char *long_valuestring = "new valuestring which much longer than previous should be changed safely";
    cjson_t *item1 = cjson_create_string(stringvalue);
    cjson_t *item2 = cjson_create_string_reference(reference_valuestring);
    char *ptr1 = NULL;
    char *return_value = NULL;

    cjson_add_item_to_object(root, "one", item1);
    cjson_add_item_to_object(root, "two", item2);

    ptr1 = item1->valuestring;
    return_value = cjson_set_value_string(cjson_get_object_item(root, "one"), short_valuestring);
    TEST_ASSERT_NOT_NULL(return_value);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(ptr1, return_value, "new valuestring shorter than old should not reallocate memory");
    TEST_ASSERT_EQUAL_STRING(short_valuestring, cjson_get_object_item(root, "one")->valuestring);

    /* we needn't to free the original valuestring manually */
    ptr1 = item1->valuestring;
    return_value = cjson_set_value_string(cjson_get_object_item(root, "one"), long_valuestring);
    TEST_ASSERT_NOT_NULL(return_value);
    TEST_ASSERT_NOT_EQUAL_MESSAGE(ptr1, return_value, "new valuestring longer than old should reallocate memory")
    TEST_ASSERT_EQUAL_STRING(long_valuestring, cjson_get_object_item(root, "one")->valuestring);

    return_value = cjson_set_value_string(cjson_get_object_item(root, "two"), long_valuestring);
    TEST_ASSERT_NULL_MESSAGE(return_value, "valuestring of reference object should not be changed");
    TEST_ASSERT_EQUAL_STRING(reference_valuestring, cjson_get_object_item(root, "two")->valuestring);

    cjson_delete(root);
}

static void cjson_set_bool_value_must_not_break_objects(void)
{
    cjson_t *bobj, *sobj, *oobj, *refobj = NULL;

    TEST_ASSERT_TRUE((CJSON_SET_BOOL_VALUE(refobj, 1) == CJSON_INVALID));

    bobj = cjson_create_false();
    TEST_ASSERT_TRUE(cjson_is_false(bobj));
    TEST_ASSERT_TRUE((CJSON_SET_BOOL_VALUE(bobj, 1) == CJSON_TRUE));
    TEST_ASSERT_TRUE(cjson_is_true(bobj));
    CJSON_SET_BOOL_VALUE(bobj, 1);
    TEST_ASSERT_TRUE(cjson_is_true(bobj));
    TEST_ASSERT_TRUE((CJSON_SET_BOOL_VALUE(bobj, 0) == CJSON_FALSE));
    TEST_ASSERT_TRUE(cjson_is_false(bobj));
    CJSON_SET_BOOL_VALUE(bobj, 0);
    TEST_ASSERT_TRUE(cjson_is_false(bobj));

    sobj = cjson_create_string("test");
    TEST_ASSERT_TRUE(cjson_is_string(sobj));
    CJSON_SET_BOOL_VALUE(sobj, 1);
    TEST_ASSERT_TRUE(cjson_is_string(sobj));
    CJSON_SET_BOOL_VALUE(sobj, 0);
    TEST_ASSERT_TRUE(cjson_is_string(sobj));

    oobj = cjson_create_object();
    TEST_ASSERT_TRUE(cjson_is_object(oobj));
    CJSON_SET_BOOL_VALUE(oobj, 1);
    TEST_ASSERT_TRUE(cjson_is_object(oobj));
    CJSON_SET_BOOL_VALUE(oobj, 0);
    TEST_ASSERT_TRUE(cjson_is_object(oobj));

    refobj = cjson_create_string_reference("conststring");
    TEST_ASSERT_TRUE(cjson_is_string(refobj));
    TEST_ASSERT_TRUE(refobj->type & CJSON_IS_REFERENCE);
    CJSON_SET_BOOL_VALUE(refobj, 1);
    TEST_ASSERT_TRUE(cjson_is_string(refobj));
    TEST_ASSERT_TRUE(refobj->type & CJSON_IS_REFERENCE);
    CJSON_SET_BOOL_VALUE(refobj, 0);
    TEST_ASSERT_TRUE(cjson_is_string(refobj));
    TEST_ASSERT_TRUE(refobj->type & CJSON_IS_REFERENCE);
    cjson_delete(refobj);

    refobj = cjson_create_object_reference(oobj);
    TEST_ASSERT_TRUE(cjson_is_object(refobj));
    TEST_ASSERT_TRUE(refobj->type & CJSON_IS_REFERENCE);
    CJSON_SET_BOOL_VALUE(refobj, 1);
    TEST_ASSERT_TRUE(cjson_is_object(refobj));
    TEST_ASSERT_TRUE(refobj->type & CJSON_IS_REFERENCE);
    CJSON_SET_BOOL_VALUE(refobj, 0);
    TEST_ASSERT_TRUE(cjson_is_object(refobj));
    TEST_ASSERT_TRUE(refobj->type & CJSON_IS_REFERENCE);
    cjson_delete(refobj);

    cjson_delete(oobj);
    cjson_delete(bobj);
    cjson_delete(sobj);
}

int CJSON_CDECL main(void)
{
    UNITY_BEGIN();

    RUN_TEST(cjson_array_foreach_should_loop_over_arrays);
    RUN_TEST(cjson_array_foreach_should_not_dereference_null_pointer);
    RUN_TEST(cjson_get_object_item_should_get_object_items);
    RUN_TEST(cjson_get_object_item_case_sensitive_should_get_object_items);
    RUN_TEST(cjson_get_object_item_should_not_crash_with_array);
    RUN_TEST(cjson_get_object_item_case_sensitive_should_not_crash_with_array);
    RUN_TEST(typecheck_functions_should_check_type);
    RUN_TEST(cjson_should_not_parse_to_deeply_nested_jsons);
    RUN_TEST(cjson_set_number_value_should_set_numbers);
    RUN_TEST(cjson_detach_item_via_pointer_should_detach_items);
    RUN_TEST(cjson_replace_item_via_pointer_should_replace_items);
    RUN_TEST(cjson_replace_item_in_object_should_preserve_name);
    RUN_TEST(cjson_functions_should_not_crash_with_null_pointers);
    RUN_TEST(ensure_should_fail_on_failed_realloc);
    RUN_TEST(skip_utf8_bom_should_skip_bom);
    RUN_TEST(skip_utf8_bom_should_not_skip_bom_if_not_at_beginning);
    RUN_TEST(cjson_get_string_value_should_get_a_string);
    RUN_TEST(cjson_get_number_value_should_get_a_number);
    RUN_TEST(cjson_create_string_reference_should_create_a_string_reference);
    RUN_TEST(cjson_create_object_reference_should_create_an_object_reference);
    RUN_TEST(cjson_create_array_reference_should_create_an_array_reference);
    RUN_TEST(cjson_add_item_to_object_or_array_should_not_add_itself);
    RUN_TEST(cjson_add_item_to_object_should_not_use_after_free_when_string_is_aliased);
    RUN_TEST(cjson_delete_item_from_array_should_not_broken_list_structure);
    RUN_TEST(cjson_set_valuestring_to_object_should_not_leak_memory);
    RUN_TEST(cjson_set_bool_value_must_not_break_objects);

    return UNITY_END();
}
